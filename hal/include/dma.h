#ifndef __DMA_HAL_H__
#define __DMA_HAL_H__

#include "hal.h"

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

#endif