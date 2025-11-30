//! TLA2528.c
//!
//! communication with TLA2528 ADC over i2c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "../../include/io/TLA2528.h"

void write_reg(struct TLA2528 *tla, uint8_t addr, uint8_t data)
{
	char buf[3] = { TLA2528_WRITE_REG, addr, data };

	i2c_write_blocking(tla->i2c_instance, tla->address, buf, sizeof(char) * 3, false);
}

void setup_TLA2528(struct TLA2528 *tla)
{
	printf("Setting up TLA2528 with SDA %i and SCL %i at %X\n", tla->sda_pin, tla->scl_pin, tla->address);

	write_reg(tla, TLA2528_GENERAL_CFG, 0b00000100); // set all channels to analog inputs
	write_reg(tla, TLA2528_DATA_CFG, 0b10000000); // set fixed pattern for adc output (testing)
	//  write_reg(tla,

	printf("Done setting up TLA2528\n");
}

void read_TLA2528(struct TLA2528 *tla)
{
}
