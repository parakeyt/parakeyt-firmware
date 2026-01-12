//! TLC59208.c
//!
//! communication with TLC59208 LED driver over i2c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "pico/time.h"

#include "../../include/io/TLC59208.h"

void setup_TLC59208(struct TLC59208 *tlc)
{
	printf("Setting up TLC59109 with SDA %i and SCL %i at 0x%X\n", tlc->sda_pin, tlc->scl_pin, tlc->address);

	uint8_t reset[2] = {
		0xa5,
		0x5a,
	};
	i2c_write_blocking(tlc->i2c_instance, TLC59208_RESET_ADDR, reset, sizeof(uint8_t) * 2, false);

	sleep_ms(10);
	uint8_t buf[19] = {
		0b10000000, // write all regs sequentially starting at mode1
		0b00000001, // mode1 disable sleep
		0b00000000, // mode2 no change
		0b00000000, // pwm0
		0b00000000, // pwm1
		0b00000000, // pwm2
		0b00000000, // pwm3
		0b00000000, // pwm4
		0b00000000, // pwm5
		0b00000000, // pwm6
		0b00000000, // pwm7
		0b00000000, // GRPPWM disable pwm
		0b00000000, // GRPFREQ unchanged
		0b00000000, // LEDOUT0 all off
		0b00000000, // LEDOUT1 all off
		0b10010010, // SUBADR1
		0b10010100, // SUBADR1
		0b10011000, // SUBADR1
		0b11010000, // ALLCALLADR
	};

	i2c_write_blocking(tlc->i2c_instance, tlc->address, buf, sizeof(uint8_t) * 19, false);

	printf("Done setting up TLC59208\n");
	sleep_ms(10);

	uint8_t buf2[2] = {
		TLC59208_LEDOUT1,
		0,
	};
	while (1) {
		buf2[1] = 0b01000000;
		i2c_write_blocking(tlc->i2c_instance, tlc->address, buf2, sizeof(uint8_t) * 2, false);
		printf("on\n");
		while (1) {
			printf("fuck you\n");
		}
		sleep_ms(500);
		buf2[1] = 0b00000000;
		i2c_write_blocking(tlc->i2c_instance, tlc->address, buf2, sizeof(uint8_t) * 2, false);
		printf("off\n");
		sleep_ms(500);
	}
}

void update_TLC59208_state(struct TLC59208 *tlc, uint8_t pin)
{
	uint16_t output = (0b01 << (pin * 2));
	uint8_t buf[3] = {
		0b10000000 & TLC59208_LEDOUT0, // write sequentially to LEDOUT0 and LEDOUT1
		(output >> 8),
		(output),

	};
	i2c_write_blocking(tlc->i2c_instance, tlc->address, buf, sizeof(uint8_t) * 3, false);
}
