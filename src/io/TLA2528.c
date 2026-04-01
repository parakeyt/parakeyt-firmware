//! TLA2528.c
//!
//! communication with TLA2528 ADC over i2c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"

#include "../../include/io/TLA2528.h"

void write_reg_TLA2528(struct TLA2528 *tla, uint8_t addr, uint8_t data)
{
	uint8_t buf[3] = { TLA2528_WRITE_REG, addr, data };

	i2c_write_blocking(tla->i2c_instance, tla->address, buf, sizeof(uint8_t) * 3, false);
}

uint8_t read_reg_TLA2528(struct TLA2528 *tla, uint8_t addr)
{
	uint8_t res = 0b1;
	uint8_t buf[2] = { TLA2528_READ_REG, addr };
	i2c_write_blocking(tla->i2c_instance, tla->address, buf, sizeof(uint8_t) * 2, false);
	i2c_read_blocking(tla->i2c_instance, tla->address, (uint8_t *)(&res), 1, false);
	return buf[0];
}

void status_check(struct TLA2528 *tla)
{
	while (true) {
		uint8_t res = read_reg_TLA2528(tla, TLA2528_SYSTEM_STATUS);
		printf("TLA 0x%X system good status: %X\n", tla->address, res);
		if (((res & 0b10000000) >> 7) == 1) {
			break;
		}
		break;
	}
}

void reset(struct TLA2528 *tla)
{
	write_reg_TLA2528(tla, TLA2528_GENERAL_CFG, 0b00000001); // reset
	// await reset
	while (true) {
		uint8_t res = read_reg_TLA2528(tla, TLA2528_GENERAL_CFG) & 0b1;
		printf("TLA reset status: %i\n", res);
		if (res == 0)
			break;
	}

	printf("TLA reset complete\n");
}

void setup_TLA2528(struct TLA2528 *tla, enum TLA2825_Mode mode)
{
	printf("Setting up TLA2528 with SDA %i and SCL %i at 0x%X in mode %i\n", tla->sda_pin, tla->scl_pin,
	       tla->address, mode);

	reset(tla);
	// check crc fuse
	if ((read_reg_TLA2528(tla, TLA2528_SYSTEM_STATUS) & 0b100) > 0) {
		printf("CRC fuse failure!\n");
	}

	if (mode == ADC) {
		printf("TLA as ADC\n");
		write_reg_TLA2528(tla, TLA2528_PIN_CFG, 0b00000000); // all pins as analog in
		//write_reg_TLA2528(tla, TLA2528_DATA_CFG, 0b10010000); // set fixed pattern for adc output (testing)
		/* write_reg_TLA2528(tla, TLA2528_OSR_CFG, 0b00000111); // disable OSR */
		/* write_reg_TLA2528(tla, TLA2528_OPMODE_CFG, */
		/* 		  0b0000000); // change sampling speed to fastest and fast oscillator */
		/* write_reg_TLA2528(tla, TLA2528_AUTO_SEQ_CH_SEL, 0b00000000); // no pins in sequencing mode */
		/* write_reg_TLA2528(tla, TLA2528_SEQUENCE_CFG, 0b00000000); // no auto sequencing */
		write_reg_TLA2528(tla, TLA2528_GENERAL_CFG,
				  0b00001100); // set all channels to analog inputs and start conversion
	} else if (mode == DRIVER) {
		reset(tla);
		printf("TLA as driver\n");
		// all pins as gpio
		write_reg_TLA2528(tla, TLA2528_PIN_CFG, 0b11111111);
		write_reg_TLA2528(tla, TLA2528_GPIO_CFG, 0b11111111);
		//write_reg_TLA2528(tla, TLA2528_GPIO_DRIVE_CFG, 0b00000000);
		write_reg_TLA2528(tla, TLA2528_GPIO_DRIVE_CFG, 0b11111111);
		// set as zero
		write_reg_TLA2528(tla, TLA2528_GPO_VALUE, 0b00000000);
	}
	status_check(tla);
	printf("Done setting up TLA2528\n");
}

void read_TLA2528(struct TLA2528 *tla, uint8_t pin, uint16_t *vals)
{
	write_reg_TLA2528(tla, TLA2528_CHANNEL_SEL, pin);
	uint8_t buf[2] = { 0 };
	i2c_read_blocking(tla->i2c_instance, tla->address, &buf[0], 2, false);
	*vals = ((buf[0] << 8) + buf[1]) >> 4;
}

void write_TLA2528(struct TLA2528 *tla, uint8_t pin)
{
	write_reg_TLA2528(tla, TLA2528_GPO_VALUE, (0b1 << pin));
}
