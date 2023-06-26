#ifndef __RCC_HAL_H__
#define __RCC_HAL_H__

#include "hal.h"

/* RCC Registers */
typedef struct {
    volatile uint32_t CR, ICSCR, CFGR, RESERVED1, RESERVED2, RESERVED3, CIER, CIFR,
                    CICR, IOPRSTR, AHBRSTR, APBRSTR1, APBRSTR2, IOPENR, AHBENR, APBENR1,
                    APBENR2, IOPSMENR, AHBSMENR, APBSMENR1, APBSMENR2, CCIPR, CSR1, CSR2;
} rcc_struct_t;
#define RCC ((rcc_struct_t*)0x40021000)

/* RCC Helpers */
#define RCC_ENABLE_CLK_GPIOA() RCC->IOPENR |= BIT(0)
#define RCC_ENABLE_CLK_GPIOB() RCC->IOPENR |= BIT(1)
#define RCC_ENABLE_CLK_GPIOC() RCC->IOPENR |= BIT(2)
#define RCC_ENABLE_CLK_GPIOD() RCC->IOPENR |= BIT(3)
#define RCC_ENABLE_CLK_GPIOF() RCC->IOPENR |= BIT(5)

#endif