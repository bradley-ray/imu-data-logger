#include <stdint.h>
#include "hal.h"

volatile uint32_t s_ticks = 0;
extern volatile uint8_t dma_rx_done;
extern volatile uint8_t dma_tx_done;
extern volatile uint8_t exti_done;
extern volatile uint8_t nack;

void hardfault_handler(void) {
	while(1);
}

void systick_handler(void) {
	s_ticks++;
}

void exti0_1_irq_handler(void) {
	exti_done = 1;
	EXTI->RPR1 |= 1;
}

void dma1_channel1_irq_handler(void) {
	if (DMA->ISR & BIT(1)) {
		DMA->IFCR |= 7;
		dma_rx_done = 1;
		// I2C->CR2 |= BIT(14);
	}
}

void dma1_channel2_3_irq_handler(void) {
	if (DMA->ISR & BIT(4)) {
		DMA->IFCR |= 7 << 4;
		dma_tx_done = 1;
	}
}

void i2c1_irq_handler(void) {
	if (I2C->ISR & BIT(4)) {
		nack = 1;
		I2C->ICR |= BIT(4);
		I2C->ISR |= BIT(0);
	}
}