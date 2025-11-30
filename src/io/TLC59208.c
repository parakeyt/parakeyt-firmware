//! TLC59208.c
//!
//! communication with TLC59208 LED driver over i2c

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "../../include/io/TLC59208.h"

void setup_TLC59208(struct TLC59208 *tlc)
{
	printf("Setting up TLC59109 with SDA %i and SCL %i at %X\n", tlc->sda_pin, tlc->scl_pin, tlc->address);

	char buf[2] = {

	};

	i2c_write_blocking(tlc->i2c_instance, tlc->address, buf, sizeof(char) * 2, false);

	printf("Done setting up TLC59208\n");
}

void update_TLC59208_state(struct TLC59208 *tlc)
{
}
