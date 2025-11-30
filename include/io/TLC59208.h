#pragma once
//! TLC59208.h
//!
//! communication with TLC59208 LED driver over i2c

#include "pico/stdlib.h"

struct TLC59208 {
	char address;
	char sda_pin;
	char scl_pin;
	i2c_inst_t *i2c_instance;
};

void setup_TLC59208(struct TLC59208 *mcp);

void update_TLC59208_state(struct TLC59208 *mcp);
