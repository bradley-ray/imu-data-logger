#include "gpio.h"

/* GPIO Defintions */
void gpio_set_mode(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->MODER &= ~((uint32_t)0x03 << (2*pin));
    gpio->MODER |= mode << (2*pin);
}

void gpio_set_output_type(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->OTYPER &= ~BIT(5);
    gpio->OTYPER |= mode << pin;
}

void gpio_set_speed(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->OSPEEDR &= ~((uint32_t)0x03 << (2*pin));
    gpio->OSPEEDR |= mode << (2*pin);
}

void gpio_set_pull(gpio_struct_t* gpio, uint32_t pin, uint32_t mode) {
    gpio->PUPDR &= ~((uint32_t)0x03 << (2*pin));
    gpio->PUPDR |= mode << (2*pin);
}

void gpio_set_af(gpio_struct_t* gpio, uint32_t pin, uint32_t af) {
	if (pin >= 8) {
		gpio->AFRH &= (uint32_t)~(0xf << (4 * (pin-8)));
		gpio->AFRH |= af << (4 * (pin-8));
	} else {
		gpio->AFRL &= (uint32_t)~(0xf << (4*pin));
		gpio->AFRL |= af << (4*pin);
	}
}

void gpio_set(gpio_struct_t* gpio, uint32_t pin) {
    gpio->ODR |= BIT(pin);
}
void gpio_reset(gpio_struct_t* gpio, uint32_t pin) {
    gpio->ODR &= ~BIT(pin);
}

void gpio_toggle(gpio_struct_t* gpio, uint32_t pin) {
    gpio->ODR ^= BIT(pin);
}