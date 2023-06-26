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

/* SysTick Registers */
#define STK_CSR *((volatile uint32_t*) (0xe000e010))
#define STK_RVR *((volatile uint32_t*) (0xe000e014))
#define STK_CVR *((volatile uint32_t*) (0xe000e018))
#define STK_CALIB *((volatile uint32_t*) (0xe000e01c))

void delay(uint32_t ms);

#endif