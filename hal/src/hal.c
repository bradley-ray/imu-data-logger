#include "hal.h"
#include <stdio.h>

extern volatile uint32_t s_ticks;

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