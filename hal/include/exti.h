#ifndef __EXTI_HAL_H__
#define __EXTI_HAL_H__

#include "hal.h"

/* EXTI Registers */
typedef struct {
	volatile uint32_t RTSR1, FTSR1, SWIER1, RPR1, FPR1, RESERVED1[19], EXTICR[4], RESERVED2[4], IMR1, EMR1;
} exti_struct_t;
#define EXTI ((exti_struct_t*)0x40021800)

#endif