#ifndef __I2C_HAL__
#define __I2C_HAL__

#include "hal.h"

/* I2C Helpers */
typedef struct {
	volatile uint32_t CR1, CR2, OAR1, OAR2, TIMINGR, TIMEOUTR, ISR, ICR, PECR, RXDR, TXDR;
} i2c_struct_t;
#define I2C ((i2c_struct_t*)0x40005400)

#define RCC_ENABLE_CLK_I2C1() RCC->APBENR1 |= BIT(21)

void i2c_write_byte(uint8_t byte);
uint8_t i2c_read_byte(uint8_t byte);
void i2c_write_buff_dma(uint8_t* tx_buff, uint8_t size);
void i2c_read_buff_dma(uint8_t tx_byte, uint8_t* rx_buff, uint8_t size);

#endif