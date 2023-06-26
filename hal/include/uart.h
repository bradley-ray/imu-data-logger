#ifndef __UART_HAL_H__
#define __UART_HAL_H__

#include "hal.h"

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

#endif