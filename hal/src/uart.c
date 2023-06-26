#include "uart.h"

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