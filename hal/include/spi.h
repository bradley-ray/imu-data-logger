#ifndef __SPI_HAL_H__
#define __SPI_HAL_H__

#include "hal.h"

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

#endif