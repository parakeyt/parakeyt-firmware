//! input.c

#include <limits.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "../../include/io/io.h"
#include "../../include/io/mcp23008.h"

struct MCP23008 mcp = {
	.address = (0b0100000 | 0b010),
	.sda_pin = 0, // gp0
	.scl_pin = 1, // gp1
	.i2c_instance = i2c0,
	.state = 0b00000000,
};
const uint GPIO_PINS[GPIO_PIN_CNT] = { 0, 1, 2 }; // mcp23008 gp0-2
const uint ADC_PINS[ADC_PIN_CNT] = { 26, 27, 28 }; // adc0, adc1, adc2

void init_pins(void)
{
	// init GPIO
	/* for (int i = 0; i < GPIO_PIN_CNT; ++i) { */
	/* 	uint pin = GPIO_PINS[i]; */
	/* 	gpio_init(pin); */
	/* 	gpio_set_dir(pin, true); // set to output */
	/* 	gpio_put(pin, 0); */
	/* } */
	i2c_init(mcp.i2c_instance, 400 * 1000); // 400khz baud
	gpio_set_function(mcp.sda_pin, GPIO_FUNC_I2C);
	gpio_set_function(mcp.scl_pin, GPIO_FUNC_I2C);
	gpio_pull_up(mcp.sda_pin);
	gpio_pull_up(mcp.scl_pin);

	setup_mcp23008(&mcp);
	update_mcp23008_state(&mcp);

	// init adc
	adc_init();
	for (int i = 0; i < ADC_PIN_CNT; ++i) {
		uint pin = ADC_PINS[i];
		adc_gpio_init(pin);
	}
}

//const float conversion_factor = 3.3f  (1 << 12); // for 3.3v vref
uint16_t read_col(uint n)
{
	adc_select_input(n);
	return adc_read();
}

static uint last_gpio_set = UINT_MAX;
void enable_row(uint n)
{
	mcp.state = (0b1 << n);
	update_mcp23008_state(&mcp);
	/* uint pin = GPIO_PINS[n]; */
	/* if (last_gpio_set != UINT_MAX) { */
	/* 	gpio_put(last_gpio_set, 0); */
	/* } */
	/* gpio_put(pin, 1); */
	/* last_gpio_set = pin; */
}
