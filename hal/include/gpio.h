#ifndef __GPIO_HAL_H__
#define __GPIO_HAL_H__

#include "hal.h"

/* GPIO Registers */
typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH, BRR;
} gpio_struct_t;
#define GPIOA ((gpio_struct_t*)0x50000000)
#define GPIOB ((gpio_struct_t*)0x50000400)
#define GPIOC ((gpio_struct_t*)0x50000800)
#define GPIOD ((gpio_struct_t*)0x50000C00)
#define GPIOF ((gpio_struct_t*)0x50001400)

/* GPIO Helpers */
#define GPIO_AF0 ((uint32_t)0x0)
#define GPIO_AF1 ((uint32_t)0x1)
#define GPIO_AF2 ((uint32_t)0x2)
#define GPIO_AF3 ((uint32_t)0x3)
#define GPIO_AF4 ((uint32_t)0x4)
#define GPIO_AF5 ((uint32_t)0x5)
#define GPIO_AF6 ((uint32_t)0x6)
#define GPIO_AF7 ((uint32_t)0x7)
#define GPIO_AF8 ((uint32_t)0x8)
#define GPIO_AF9 ((uint32_t)0x9)
#define GPIO_AF10 ((uint32_t)0xa)
#define GPIO_AF11 ((uint32_t)0xb)
#define GPIO_AF12 ((uint32_t)0xc)
#define GPIO_AF13 ((uint32_t)0xd)
#define GPIO_AF14 ((uint32_t)0xe)
#define GPIO_AF15 ((uint32_t)0xf)

#define GPIO_MODE_INPUT 0x00
#define GPIO_MODE_OUTPUT 0x01
#define GPIO_MODE_AF 0x02
#define GPIO_MODE_ANALOG 0x03
void gpio_set_mode(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_OUT_TYPE_PP 0x00
#define GPIO_OUT_TYPE_OD 0x01
void gpio_set_output_type(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_SPEED_VERY_LOW 0x00
#define GPIO_SPEED_LOW 0x01
#define GPIO_SPEED_HIGH 0x02
#define GPIO_SPEED_VERY_HIGH 0x03
void gpio_set_speed(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_PULL_NONE 0x00
#define GPIO_PULL_UP 0x01
#define GPIO_PULL_DOWN 0x02
#define GPIO_PULL_RESERVED 0x03
void gpio_set_pull(gpio_struct_t* gpio, uint32_t pin, uint32_t mode);

#define GPIO_AF_0 0
#define GPIO_AF_1 1
#define GPIO_AF_2 2
#define GPIO_AF_3 3
#define GPIO_AF_4 4
#define GPIO_AF_5 5
#define GPIO_AF_6 6
#define GPIO_AF_7 7
#define GPIO_AF_8 8
#define GPIO_AF_9 9
#define GPIO_AF_10 10
#define GPIO_AF_11 11
#define GPIO_AF_12 12
#define GPIO_AF_13 13
#define GPIO_AF_14 14
#define GPIO_AF_15 15
void gpio_set_af(gpio_struct_t* gpio, uint32_t pin, uint32_t af);

void gpio_set(gpio_struct_t* gpio, uint32_t pin);
void gpio_reset(gpio_struct_t* gpio, uint32_t pin);
void gpio_toggle(gpio_struct_t* gpio, uint32_t pin);

#endif