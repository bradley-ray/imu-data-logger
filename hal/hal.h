#ifndef __MPU_H__
#define __MPU_H__

#include <inttypes.h>
#include <stddef.h>

#define BIT(n) ((uint32_t)(1 << (n)))

/* NVIC Registers */
typedef struct {
	volatile uint32_t ISER, RESERVED1[31], ICER, RESERVED2[31], ISPR, RESERVED3[31], ICPR, RESERVED4[96], IPR[8];
} nvic_struct_t;
#define NVIC ((nvic_struct_t*)0xe000e100)

#define NVIC_IRQ_enable(n) NVIC->ISER |= BIT(n)
#define NVIC_IRQ_disable(n) NVIC->ICER |= BIT(n)

void nvic_irq_set_priority(uint8_t irq_n, uint8_t priority);


/* SYSCFG Registers */
typedef struct {
	volatile uint32_t CFGR1, RESERVED1[5], CFGR2, RESERVED2[8], CFGR3, RESERVED3[16],
										ITLINE0, RESERVED4, ITLINE2, ITLINE3, ITLINE4, ITLINE5, ITLINE6, ITLINE7,
										RESERVED5, ITLINE9, ITLINE10, ITLINE11, ITLINE12, ITLINE13, ITLINE14,
										RESERVED6, ITLINE16, RESERVED7[2], ITLINE19, RESERVED8, ITLINE20,
										ITLINE21, ITLINE22, ITLINE23, RESERVED9, ITLINE25, RESERVED10, ITLINE27,
										ITLINE28;
} syscfg_struct_t;

/* RCC Registers */
typedef struct {
    volatile uint32_t CR, ICSCR, CFGR, RESERVED1, RESERVED2, RESERVED3, CIER, CIFR,
                    CICR, IOPRSTR, AHBRSTR, APBRSTR1, APBRSTR2, IOPENR, AHBENR, APBENR1,
                    APBENR2, IOPSMENR, AHBSMENR, APBSMENR1, APBSMENR2, CCIPR, CSR1, CSR2;
} rcc_struct_t;
#define RCC ((rcc_struct_t*)0x40021000)

/* RCC Helpers */
#define RCC_ENABLE_CLK_GPIOA() RCC->IOPENR |= BIT(0)
#define RCC_ENABLE_CLK_GPIOB() RCC->IOPENR |= BIT(1)
#define RCC_ENABLE_CLK_GPIOC() RCC->IOPENR |= BIT(2)
#define RCC_ENABLE_CLK_GPIOD() RCC->IOPENR |= BIT(3)
#define RCC_ENABLE_CLK_GPIOF() RCC->IOPENR |= BIT(5)

/* SysTick Registers */
#define STK_CSR *((volatile uint32_t*) (0xe000e010))
#define STK_RVR *((volatile uint32_t*) (0xe000e014))
#define STK_CVR *((volatile uint32_t*) (0xe000e018))
#define STK_CALIB *((volatile uint32_t*) (0xe000e01c))

void delay(uint32_t ms);

/* DMA Registers */
typedef struct {
	volatile uint32_t ISR, IFCR, CCR1, CNDTR1, CPAR1, CMAR1, RESERVED1, 
					 CCR2, CNDTR2, CPAR2, CMAR2, RESERVED2, CCR3, CNDTR3, CPAR3, CMAR3;
} dma_struct_t;
#define DMA ((dma_struct_t*)0x40020000)

#define RCC_ENABLE_CLK_DMA1() RCC->AHBENR |= BIT(0)

/* DMAMUX Registers */
typedef struct {
	volatile uint32_t CCR[3], RESERVED1[29], CSR, CFR, RESERVED2[30], RGCR[4],
					RESERVED3[12], RGSR, RGCFR, RESERVED4[174];
} dmamux_struct_t;
#define DMAMUX ((dmamux_struct_t*)0x40020800)

#define RCC_ENABLE_CLK_DMA1() RCC->AHBENR |= BIT(0)

/* GPIO Registers */
typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH, BRR;
} gpio_struct_t;
#define GPIOA ((gpio_struct_t*)0x50000000)
#define GPIOB ((gpio_struct_t*)0x50000400)
#define GPIOC ((gpio_struct_t*)0x50000800)
#define GPIOD ((gpio_struct_t*)0x50000C00)
#define GPIOF ((gpio_struct_t*)0x50001400)

/* GPIO Helpers */
#define GPIO_AF0 ((uint32_t)0x0)
#define GPIO_AF1 ((uint32_t)0x1)
#define GPIO_AF2 ((uint32_t)0x2)
#define GPIO_AF3 ((uint32_t)0x3)
#define GPIO_AF4 ((uint32_t)0x4)
#define GPIO_AF5 ((uint32_t)0x5)
#define GPIO_AF6 ((uint32_t)0x6)
#define GPIO_AF7 ((uint32_t)0x7)
#define GPIO_AF8 ((uint32_t)0x8)
#define GPIO_AF9 ((uint32_t)0x9)
#define GPIO_AF10 ((uint32_t)0xa)
#define GPIO_AF11 ((uint32_t)0xb)
#define GPIO_AF12 ((uint32_t)0xc)
#define GPIO_AF13 ((uint32_t)0xd)
#define GPIO_AF14 ((uint32_t)0xe)
#define GPIO_AF15 ((uint32_t)0xf)

#define GPIO_MODE_INPUT 0x00
#define GPIO_MODE_OUTPUT 0x01
#define GPIO_MODE_AF 0x02
#define GPIO_MODE_ANALOG 0x03
void gpio_set_mode(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_OUT_TYPE_PP 0x00
#define GPIO_OUT_TYPE_OD 0x01
void gpio_set_output_type(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_SPEED_VERY_LOW 0x00
#define GPIO_SPEED_LOW 0x01
#define GPIO_SPEED_HIGH 0x02
#define GPIO_SPEED_VERY_HIGH 0x03
void gpio_set_speed(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_PULL_NONE 0x00
#define GPIO_PULL_UP 0x01
#define GPIO_PULL_DOWN 0x02
#define GPIO_PULL_RESERVED 0x03
void gpio_set_pull(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_AF_0 0
#define GPIO_AF_1 1
#define GPIO_AF_2 2
#define GPIO_AF_3 3
#define GPIO_AF_4 4
#define GPIO_AF_5 5
#define GPIO_AF_6 6
#define GPIO_AF_7 7
#define GPIO_AF_8 8
#define GPIO_AF_9 9
#define GPIO_AF_10 10
#define GPIO_AF_11 11
#define GPIO_AF_12 12
#define GPIO_AF_13 13
#define GPIO_AF_14 14
#define GPIO_AF_15 15
void gpio_set_af(gpio_struct_t* gpio, uint32_t pin, uint32_t af);

void gpio_set(gpio_struct_t* gpio, uint32_t pin);
void gpio_reset(gpio_struct_t* gpio, uint32_t pin);
void gpio_toggle(gpio_struct_t* gpio, uint32_t pin);

/* USART Helpers */
typedef struct {
    volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR, TDR, PRESC;
} usart2_struct_t;
#define USART2 ((usart2_struct_t*)0x40004400)

#define RCC_ENABLE_CLK_USART2() RCC->APBENR1 |= BIT(17)
#define USART2_SET_BAUD(freq, baud) USART2->BRR |= freq/baud
#define USART2_ENABLE() USART2->CR1 |= BIT(0)
#define USART2_ENABLE_TRANSMIT() USART2->CR1 |= BIT(3)
#define USART2_ENABLE_RECEIVE() USART2->CR1 |= BIT(2)

void uart2_write_byte(uint8_t byte);
void uart2_write_buff(uint8_t* byte, uint32_t size);
uint8_t uart2_read_byte();
void uart2_read_buff(uint8_t* byte, uint32_t size);

/* SPI Helpers */
typedef struct {
    volatile uint32_t CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR, I2SPR;
} spi_struct_t;
#define SPI1 ((spi_struct_t*)0x40013000)

#define RCC_ENABLE_CLK_SPI1() RCC->APBENR2 |= BIT(12)

void spi_write_byte(uint8_t byte);
void spi_write_buff(uint8_t* buff, uint32_t size);
uint8_t spi_read_byte(void);
void spi_read_buff(uint8_t* buff, uint32_t size);
uint8_t spi_read_write_byte(uint8_t byte);
void spi_read_write_buff(uint8_t* tx_buff, uint8_t* rx_buff, uint32_t size);

/* I2C Helpers */
typedef struct {
	volatile uint32_t CR1, CR2, OAR1, OAR2, TIMINGR, TIMEOUTR, ISR, ICR, PECR, RXDR, TXDR;
} i2c_struct_t;
#define I2C ((i2c_struct_t*)0x40005400)

void i2c_write_byte(uint8_t byte);
uint8_t i2c_read_byte(uint8_t byte);
void i2c_write_buff_dma(uint8_t* tx_buff, uint8_t size);
void i2c_read_buff_dma(uint8_t tx_byte, uint8_t* rx_buff, uint8_t size);

#define RCC_ENABLE_CLK_I2C1() RCC->APBENR1 |= BIT(21)

/* EXTI Registers */
typedef struct {
	volatile uint32_t RTSR1, FTSR1, SWIER1, RPR1, FPR1, RESERVED1[19], EXTICR[4], RESERVED2[4], IMR1, EMR1;
} exti_struct_t;
#define EXTI ((exti_struct_t*)0x40021800)

#endif
