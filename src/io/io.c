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

const uint8_t rowmap[ROWS][2] = ROWS_MAP;
const uint8_t colmap[COLUMNS][2] = COLS_MAP;

struct TLA2528 adcs[ADC_CNT] = { 0 };
const uint8_t adc_addrs[ADC_CNT] = ADC_ADDRS;

void init_pins(void)
{
	// init i2c0
	i2c_init(i2c0, 1000 * 1000); // 1000khz baud
	gpio_set_function(0, GPIO_FUNC_I2C);
	gpio_set_function(1, GPIO_FUNC_I2C);
	gpio_pull_up(0);
	gpio_pull_up(1);

	// init ADCs
	for (int i = 0; i < ADC_CNT; ++i) {
		adcs[i].address = adc_addrs[i];
		adcs[i].sda_pin = 0;
		adcs[i].scl_pin = 1;
		adcs[i].i2c_instance = i2c0;
		setup_TLA2528(&adcs[i]);
	}

	// init drivers
	for (int i = 0; i < DRIVER_CNT; ++i) {
	}
}

void read_col(uint n, uint16_t *buf)
{
	uint8_t adc = colmap[n][0];
	uint8_t pin = colmap[n][1];
	read_TLA2528(&adcs[adc], pin, buf);
}

void enable_row(uint n)
{
	uint8_t driver = rowmap[n][0];
	uint8_t pin = rowmap[n][1];
}
