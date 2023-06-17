#include "hal.h"
#include <stdio.h>

extern volatile uint32_t s_ticks;
extern volatile uint8_t nack;
volatile uint32_t i2c_irq = 0;

void system_init(void) {
	// HSIDIV = 1
	RCC->CR &= (uint32_t)~(0x7 << 11);

	// systick init
	STK_CSR |= BIT(2) + BIT(1) + BIT(0);
	STK_RVR = 48000-1;
	STK_CVR |= 1;
}

void delay(uint32_t ms) {
	s_ticks = 0;
	STK_CSR |= BIT(0);
	while(s_ticks < ms);
	STK_CSR &= ~BIT(0);
	STK_CVR |= 1;
}

void nvic_irq_set_priority(uint8_t irq_n, uint8_t priority) {
	uint32_t bits = priority << ((irq_n % 4) * 8);
	NVIC->IPR[irq_n / 4] &= (uint32_t)~bits;
	NVIC->IPR[irq_n / 4] |= bits;
}

/* GPIO Defintions */
void gpio_set_mode(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->MODER &= ~((uint32_t)0x03 << (2*pin));
    gpio->MODER |= mode << (2*pin);
}

void gpio_set_output_type(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->OTYPER &= ~BIT(5);
    gpio->OTYPER |= mode << pin;
}

void gpio_set_speed(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->OSPEEDR &= ~((uint32_t)0x03 << (2*pin));
    gpio->OSPEEDR |= mode << (2*pin);
}

void gpio_set_pull(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->PUPDR &= ~((uint32_t)0x03 << (2*pin));
    gpio->PUPDR |= mode << (2*pin);
}

void gpio_set_af(gpio_struct_t* gpio, uint32_t pin, uint32_t af) {
	if (pin >= 8) {
		gpio->AFRH &= (uint32_t)~(0xf << (4 * (pin-8)));
		gpio->AFRH |= af << (4 * (pin-8));
	} else {
		gpio->AFRL &= (uint32_t)~(0xf << (4*pin));
		gpio->AFRL |= af << (4*pin);
	}
}

void gpio_set(gpio_struct_t* gpio, uint32_t pin) {
    gpio->ODR |= BIT(pin);
}
void gpio_reset(gpio_struct_t* gpio, uint32_t pin) {
    gpio->ODR &= ~BIT(pin);
}

void gpio_toggle(gpio_struct_t* gpio, uint32_t pin) {
    gpio->ODR ^= BIT(pin);
}

/* UART Defintions */
void uart2_write_byte(uint8_t byte) {
	USART2->TDR = (uint32_t)byte;
	while(!(USART2->ISR & BIT(7)));
}

void uart2_write_buff(uint8_t* buff, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i)
		uart2_write_byte(buff[i]);
	while(!(USART2->ISR & BIT(6)));
}

uint8_t uart2_read_byte(void) {
	while(!(USART2->ISR & BIT(5)));
	return (uint8_t)USART2->RDR;
}

void uart2_read_buff(uint8_t* buff, uint32_t size) {
	for (uint32_t i = 0; i < size; ++i)
		buff[i] = uart2_read_byte();
}

uint8_t spi_read_write_byte(uint8_t byte) {
	while(!(SPI1->SR & BIT(1)));
	*((volatile uint8_t*)&(SPI1->DR)) = byte;
	while(!(SPI1->SR & BIT(0)));
	return *((volatile uint8_t*)&(SPI1->DR));
}

void spi_read_write_buff(uint8_t* tx_buff, uint8_t* rx_buff, uint32_t size) {
	uint8_t response;
	for (uint32_t i = 0; i < size; ++i) {
		response = spi_read_write_byte(tx_buff[i]);
		if (response != 0)
			rx_buff[i] = response;	
	}
}

void i2c_write_byte(uint8_t byte) {
	// set to Write mode
	I2C->CR2 &= (uint32_t)~BIT(10);

	// enable autoend
	I2C->CR2 |= BIT(25);

	// set NBYTES to be transfered
	I2C->CR2 &= (uint32_t)~(0xff << 16);
	I2C->CR2 |= BIT(16);

	// Send Start 	I2C->TXDR |= tx_byte;condition
	I2C->CR2 |= BIT(13);

	// Put byte in transfer register
	while(!(I2C->ISR & BIT(0)));
	I2C->TXDR = byte;

	// Wait for transfer complete
	while(!(I2C->ISR & BIT(6)));
}

uint8_t i2c_read_byte(uint8_t reg) {
	// set NBYTES to be transfered
	I2C->CR2 &= (uint32_t)~(0xff << 16);
	I2C->CR2 |= BIT(16);

	I2C->CR2 &= (uint32_t)~BIT(25);

	// set to Write mode
	I2C->CR2 &= (uint32_t)~BIT(10);
	
	// Send Start condition
	I2C->CR2 |= BIT(13);
	
	// Put byte in transfer register
	while(!(I2C->ISR & BIT(0)));
	I2C->TXDR = reg;

	// Wait for transfer complete
	while(!(I2C->ISR & BIT(6)));
	// Set stop bit
	I2C->CR2 |= BIT(14);

	// Set to Read mode
	while((I2C->CR2 & BIT(13)));
	I2C->CR2 |= BIT(10);
	
	// set NBYTES to be transfered
	I2C->CR2 &= (uint32_t)~(0xff << 16);
	I2C->CR2 |= BIT(16);

	// Send Start condition
	I2C->CR2 |= BIT(13);

	// Wait till transfer complete
	while(!(I2C->ISR & BIT(2)));

	// Read received byte
	uint8_t received = (uint8_t)(I2C->RXDR);

	// Set stop bit
	I2C->CR2 |= BIT(14);

	// Return byte from RXDR
	return received;
}


void i2c_write_buff_dma(uint8_t* tx_buff, uint8_t size) {
	// wait until i2c not busy
	while(I2C->ISR & BIT(15));

	// disable dma
	DMA->CCR2 &= (uint32_t)~BIT(0);
	// set size of transfer
	DMA->CNDTR2 = size;
	// set buffer as memory address
	DMA->CMAR2 = (uint32_t)tx_buff;
	// enable channel
	DMA->CCR2 |= BIT(0);

	// set to write mode
	I2C->CR2 &= (uint32_t)~BIT(10);
	// clear and set nbytes
	I2C->CR2 &= ~(uint32_t)(0xff << 16);
	I2C->CR2 |= (uint32_t)size << 16;
	// enable autoend
	I2C->CR2 |= BIT(25);
	// set start bit
	I2C->CR2 |= BIT(13);
}

void i2c_read_buff_dma(uint8_t tx_byte, uint8_t* rx_buff, uint8_t size) {
	// wait until i2c not busy
	while(I2C->ISR & BIT(15));

	// disable dma
	DMA->CCR1 &= (uint32_t)~BIT(0);
	DMA->CCR2 &= (uint32_t)~BIT(0);
	// set dma transfer size
	DMA->CNDTR1 = size;
	// set dma memory addr
	DMA->CMAR1 = (uint32_t)rx_buff;
	// set dma enable
	DMA->CCR1 |= BIT(0);

	// set to write mode
	I2C->CR2 &= (uint32_t)~BIT(10);
	// set i2c NBYTES to be transfered
	while(I2C->CR2 & BIT(13));
	I2C->CR2 &= (uint32_t)~(0xff << 16);
	I2C->CR2 |= BIT(16);
	// disable autoend
	I2C->CR2 &= (uint32_t)~BIT(25);
	// enable start bit
	I2C->CR2 |= BIT(13);
	// wait for TXIS
	while(!(I2C->ISR & BIT(1)) && !nack);
	if (nack)
		return;
	// move data to transmit register
	I2C->TXDR = tx_byte;
	// wait for transfer complete
	while(!(I2C->ISR & BIT(6)) && !nack);
	if (nack)
		return;

	// set i2c read mode
	I2C->CR2 |= BIT(10);
	// set i2c NBYTES to be transfered
	I2C->CR2 &= (uint32_t)~(0xff << 16);
	I2C->CR2 |= size << 16;
	// send start condition
	I2C->CR2 |= BIT(25) + BIT(13);
}