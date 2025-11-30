//! input.c

#include <limits.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "../../config/config.h"
#include "../../include/io/io.h"
#include "../../include/io/TLA2528.h"
#include "../../include/io/TLC59208.h"

const uint8_t rowmap[][] = ROWS_MAP;
const uint8_t colmap[][] = COLS_MAP;

void init_pins(void)
{
	// init i2c0
	i2c_init(i2c0, 400 * 1000); // 400khz baud
	gpio_set_function(0, GPIO_FUNC_I2C);
	gpio_set_function(1, GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);

	// init ADCs
	for (int i = 0; i < ADC_CNT; ++i) {
	}

	// init drivers
	for (int i = 0; i < DRIVER_CNT; ++i) {
	}
}

uint16_t read_col(uint n)
{
	uint8_t adc = colmap[n][0];
	uint8_t pin = colmap[n][1];
	return 0;
}

void enable_row(uint n)
{
	uint8_t driver = rowmap[n][0];
	uint8_t pin = rowmap[n][1];
}
