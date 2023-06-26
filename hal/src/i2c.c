#include "i2c.h"
#include "dma.h"

extern volatile uint8_t nack;
volatile uint32_t i2c_irq = 0;

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