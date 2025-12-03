//! TLA2528.c
//!
//! communication with TLA2528 ADC over i2c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"

#include "../../include/io/TLA2528.h"

void write_reg(struct TLA2528 *tla, uint8_t addr, uint8_t data)
{
	uint8_t buf[3] = { TLA2528_WRITE_REG, addr, data };

	i2c_write_blocking(tla->i2c_instance, tla->address, buf, sizeof(char) * 3, false);
}

void setup_TLA2528(struct TLA2528 *tla)
{
	printf("Setting up TLA2528 with SDA %i and SCL %i at 0x%X\n", tla->sda_pin, tla->scl_pin, tla->address);

	//write_reg(tla, TLA2528_GENERAL_CFG, 0b00000001); // reset
	write_reg(tla, TLA2528_PIN_CFG, 0b00000000); // all pins as analog in
	//write_reg(tla, TLA2528_DATA_CFG, 0b10010000); // set fixed pattern for adc output (testing)
	//write_reg(tla, TLA2528_OSR_CFG,     0b00000111); // highest OSR (testing)
	//write_reg(tla, TLA2528_OPMODE_CFG, 0b0000000); // change sampling speed to fastest and fast oscillator
	//write_reg(tla, TLA2528_AUTO_SEQ_CH_SEL, 0b00000000); // no pins in sequencing mode
	//write_reg(tla, TLA2528_SEQUENCE_CFG, 0b00000000); // enable auto sequencing
	write_reg(tla, TLA2528_GENERAL_CFG, 0b00001100); // set all channels to analog inputs and start conversion
	printf("Done setting up TLA2528\n");
}

void read_TLA2528(struct TLA2528 *tla, uint8_t pin, uint16_t vals[])
{
	write_reg(tla, TLA2528_CHANNEL_SEL, pin);
	i2c_read_blocking(tla->i2c_instance, tla->address, (uint8_t *)(&vals[0]), 2, false);
}
