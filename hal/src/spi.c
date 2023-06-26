#include "spi.h"

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