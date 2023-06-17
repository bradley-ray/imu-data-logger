#include "hal.h"
#include "mpu6050.h"
#include <stdio.h>

#define FREQ 48000000
#define LED_TOGGLE() gpio_toggle(GPIOA, 5)

void gpio_led_init(void);
void uart2_init(uint32_t baud);
void i2c_init(uint8_t addr);
void dma_init(void);
void exti_init(void);

volatile uint8_t dma_rx_done = 0;
volatile uint8_t dma_tx_done = 0;
volatile uint8_t exti_done = 0;
volatile uint8_t nack = 0;
volatile uint8_t berr = 0;
volatile uint8_t arlo = 0;


// main loop
int main() {
	gpio_led_init();
	uart2_init(115200);
	exti_init();
	dma_init();
	i2c_init(0x68);

	// i2c interrupt
	nvic_irq_set_priority(23,5);
	NVIC_IRQ_enable(23);

	// dma ch1 (i2c rx) interrupt
	nvic_irq_set_priority(9,0);
	NVIC_IRQ_enable(9);

	// dma ch2 (i2c tx) interrupt
	nvic_irq_set_priority(10,5);
	NVIC_IRQ_enable(10);

	// exti (mpu6050) interrupt
	nvic_irq_set_priority(5,10);
	NVIC_IRQ_enable(5);

	uint8_t tx_buff[8] = {0};
	uint8_t accel_buff[6] = {0};
	uint8_t gyro_buff[6] = {0};

	mpu_init(MPU_ACCEL_8g, MPU_GYRO_1000deg, tx_buff);

	uint8_t err;
	while (1)
	{
		while(!exti_done);
		exti_done = 0;

		// read int status
		err = mpu_int_status(tx_buff);
		if (err)
			continue;
		// read acceleration value
		err = mpu_read_accel(accel_buff);
		if (err)
			continue;
		// read gyroscope value
		err = mpu_read_gyro(gyro_buff);
		if (err)
			continue;

		uart2_write_byte('$');
		uart2_write_byte(0);
		uart2_write_buff(accel_buff, 6);

		uart2_write_byte('$');
		uart2_write_byte(1);
		uart2_write_buff(gyro_buff, 6);
	}
	
	return 0;
}

uint8_t mpu_write_reg(mpu_reg_t reg, uint8_t* tx_buff, uint8_t size) {
	uint8_t tmp1 = tx_buff[0];
	for(uint32_t i = 0; i < size; i++) {
		uint8_t tmp2 = tx_buff[i+1];
		tx_buff[i+1] = tmp1;
		tmp1 = tmp2;
	}
	tx_buff[0] = reg;
	i2c_write_buff_dma(tx_buff, size+1);
	uint8_t err = nack;
	nack = 0;
	if (err)
		return 1;
	while(!dma_tx_done);
	dma_tx_done = 0;

	return 0;
}

uint8_t mpu_read_reg(mpu_reg_t reg, uint8_t* rx_buff, uint8_t size) {
	i2c_read_buff_dma(reg, rx_buff, size);
	uint8_t err = nack;
	nack = 0;
	if (err)
		return 1;
	while(!dma_rx_done);
	dma_rx_done = 0;

	return 0;
}

void gpio_led_init(void) {
	RCC_ENABLE_CLK_GPIOA();
	gpio_set_mode(GPIOA, 5, GPIO_MODE_OUTPUT);
	gpio_set_output_type(GPIOA, 5, GPIO_OUT_TYPE_PP);
	gpio_set_speed(GPIOA, 5, GPIO_SPEED_VERY_HIGH);
	gpio_set_pull(GPIOA, 5, GPIO_PULL_NONE);
}

void uart2_init(uint32_t baud) {
	RCC_ENABLE_CLK_USART2();
	USART2_SET_BAUD(FREQ,baud);
	USART2_ENABLE();
	USART2_ENABLE_TRANSMIT();
	USART2_ENABLE_RECEIVE();

	RCC_ENABLE_CLK_GPIOA();
	// setup GPIO PA2
	gpio_set_mode(GPIOA, 2, GPIO_MODE_AF);
	gpio_set_output_type(GPIOA, 2, GPIO_OUT_TYPE_PP);
	gpio_set_pull(GPIOA, 2, GPIO_PULL_NONE);
	gpio_set_speed(GPIOA, 2, GPIO_SPEED_VERY_LOW);
	gpio_set_af(GPIOA, 2, GPIO_AF_1);
	

	// setup GPIO PA3
	gpio_set_mode(GPIOA, 3, GPIO_MODE_AF);
	gpio_set_output_type(GPIOA, 3, GPIO_OUT_TYPE_PP);
	gpio_set_pull(GPIOA, 3, GPIO_PULL_NONE);
	gpio_set_speed(GPIOA, 3, GPIO_SPEED_VERY_LOW);
	gpio_set_af(GPIOA, 3, GPIO_AF_1);
}

void i2c_init(uint8_t addr) {
	RCC_ENABLE_CLK_I2C1();
	I2C->CR1 |= BIT(15) + BIT(14);
	// I2C->CR1 |= BIT(17);
	I2C->CR2 |= addr << 1;
	// I2C->CR2 |= BIT(25);

	// 100KHz
	I2C->TIMINGR |= 0x00B0DBFF;

	// enable NACKIE
	I2C->CR1 |= BIT(4);
	// enable ERRIE
	I2C->CR1 |= BIT(7);

	// enable i2c
	I2C->CR1 |= BIT(0);

	RCC_ENABLE_CLK_GPIOB();
	// setup GPIO PB8
	gpio_set_mode(GPIOB, 8, GPIO_MODE_AF);
	gpio_set_output_type(GPIOB, 8, GPIO_OUT_TYPE_OD);
	gpio_set_pull(GPIOB, 8, GPIO_PULL_UP);
	gpio_set_speed(GPIOB, 8, GPIO_SPEED_VERY_LOW);
	gpio_set_af(GPIOB, 8, GPIO_AF_6);
	// setup GPIO PB9
	gpio_set_mode(GPIOB, 9, GPIO_MODE_AF);
	gpio_set_output_type(GPIOB, 9, GPIO_OUT_TYPE_OD);
	gpio_set_pull(GPIOB, 9, GPIO_PULL_UP);
	gpio_set_speed(GPIOB, 9, GPIO_SPEED_VERY_LOW);
	gpio_set_af(GPIOB, 9, GPIO_AF_6);
}

void dma_init(void) {
	RCC_ENABLE_CLK_DMA1();

	// periphal 8 bit size
	DMA->CCR1 &= (uint32_t)~(0x3 << 8);
	// memory 8 bit size
	DMA->CCR1 &= (uint32_t)~(0x3 << 10);
	// periphal to memory (i2c rx)
	DMA->CCR1 &= (uint32_t)~(BIT(4));
	// enable transfer complete interrupt
	DMA->CCR1 |= BIT(1);
	// enable memory increment mode
	DMA->CCR1 |= BIT(7);
	// set i2c receive as periphal address
	DMA->CPAR1 = (uint32_t)&(I2C->RXDR);

	// periphal 8 bit size
	DMA->CCR2 &= (uint32_t)~(0x3 << 8);
	// memory 8 bit size
	DMA->CCR2 &= (uint32_t)~(0x3 << 10);
	// memory to periphial (i2c tx)
	DMA->CCR2 |= BIT(4);
	// enable transfer complete interrupt
	DMA->CCR2 |= BIT(1);
	// enable memory increment mode
	DMA->CCR2 |= BIT(7);
	// set i2c transmit as periphal address
	DMA->CPAR2 = (uint32_t)&(I2C->TXDR);

	// i2c1_rx_dma
	DMAMUX->CCR[0] |= 10;
	// i2c1_tx_dma
	DMAMUX->CCR[1] |= 11;
}

void exti_init(void) {
	// setup exti interupt for gpio pb5
	EXTI->RTSR1 |= BIT(0);
	EXTI->EXTICR[0] = 1;
	EXTI->IMR1 |= 1;
	
	RCC_ENABLE_CLK_GPIOB();
	// setup GPIO PB5
	gpio_set_mode(GPIOB, 0, GPIO_MODE_INPUT);
	gpio_set_pull(GPIOB, 0, GPIO_PULL_UP);
	// GPIOB->AFRL |= (uint32_t)1 << 8;
}
