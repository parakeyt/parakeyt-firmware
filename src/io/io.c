//! input.c

#include <limits.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "pico/time.h"

#include "ws2812.pio.h"
#include "../../config/config.h"
#include "../../include/io/io.h"
#include "../../include/io/TLA2528.h"
#include "../../include/io/MCP23008.h"

#define BAUD (400 * 1000)

const uint8_t rowmap[ROWS][2] = ROWS_MAP;
const uint8_t colmap[COLUMNS][2] = COLS_MAP;

struct TLA2528 adcs[ADC_CNT] = { 0 };
const uint8_t adc_addrs[ADC_CNT] = ADC_ADDRS;

struct TLA2528 drivers[DRIVER_CNT] = { 0 };
const uint8_t driver_addrs[DRIVER_CNT] = DRIVER_ADDRS;

void init_pins(void)
{
	// init i2c0
	i2c_init(i2c0, BAUD);
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
		setup_TLA2528(&adcs[i], ADC);
	}

	// init drivers
	for (int i = 0; i < DRIVER_CNT; ++i) {
		drivers[i].address = driver_addrs[i];
		drivers[i].sda_pin = 0;
		drivers[i].scl_pin = 1;
		drivers[i].i2c_instance = i2c0;
		setup_TLA2528(&drivers[i], DRIVER);
	}
}

void read_col(uint n, uint16_t *buf)
{
	uint8_t adc = colmap[n][0];
	uint8_t pin = colmap[n][1];
	read_TLA2528(&adcs[adc], pin, buf);
	//sleep_ms(5); // TODO pull-down resistor?
}

void enable_row(uint n)
{
	uint8_t driver = rowmap[n][0];
	uint8_t pin = rowmap[n][1];
	write_TLA2528(&drivers[driver], pin);
	//	sleep_ms(2); // TODO pull-down resistor?
}

#define LED_PIN 16

static PIO pio;
static uint sm;
static uint offset;
void init_onboard_led()
{
	pio_claim_free_sm_and_add_program_for_gpio_range(&ws2812_program, &pio, &sm, &offset, LED_PIN, 1, true);

	ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);
}
void set_onboard_led(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t grb = ((g << 16) + (r << 8) + (b)) << 8;
	pio_sm_put_blocking(pio, sm, grb);
}
