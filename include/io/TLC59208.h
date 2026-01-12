#pragma once
//! TLC59208.h
//!
//! communication with TLC59208 LED driver over i2c

#include "pico/stdlib.h"

#define TLC59208_RESET_ADDR 0b1001111

// registers
#define TLC59208_MODE1 0x00
#define TLC59208_MODE2 0x01
#define TLC59208_PWM0 0x02
#define TLC59208_PWM1 0x03
#define TLC59208_PWM2 0x04
#define TLC59208_PWM3 0x05
#define TLC59208_PWM4 0x06
#define TLC59208_PWM5 0x07
#define TLC59208_PWM6 0x08
#define TLC59208_PWM7 0x09
#define TLC59208_GRPPWM 0x0A
#define TLC59208_GRPFREQ 0x0B
#define TLC59208_LEDOUT0 0x0C
#define TLC59208_LEDOUT1 0x0D
#define TLC59208_SUBADR1 0x0E
#define TLC59208_SUBADR2 0x0F
#define TLC59208_SUBADR3 0x10
#define TLC59208_ALLCALLAD 0x11

struct TLC59208 {
	char address;
	char sda_pin;
	char scl_pin;
	i2c_inst_t *i2c_instance;
};

void setup_TLC59208(struct TLC59208 *mcp);

void update_TLC59208_state(struct TLC59208 *mcp, uint8_t pin);
