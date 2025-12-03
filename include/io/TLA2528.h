#pragma once
//! TLA2528.h
//!
//! communication with TLA2528 ADC over i2c

#include "pico/stdlib.h"

#define TLA2528_WRITE_REG 0b00001000

#define TLA2528_SYSTEM_STATUS 0x0
#define TLA2528_GENERAL_CFG 0x1
#define TLA2528_DATA_CFG 0x2
#define TLA2528_OSR_CFG 0x3
#define TLA2528_OPMODE_CFG 0x4
#define TLA2528_PIN_CFG 0x5
#define TLA2528_GPIO_CFG 0x7
#define TLA2528_GPIO_DRIVE_CFG 0x9
#define TLA2528_GPIO_VALUE 0xB
#define TLA2528_GPI_VALUE 0xD
#define TLA2528_SEQUENCE_CFG 0x10
#define TLA2528_CHANNEL_SEL 0x11
#define TLA2528_AUTO_SEQ_CH_SEL 0x12

struct TLA2528 {
	char address;
	char sda_pin;
	char scl_pin;
	i2c_inst_t *i2c_instance;
};

void setup_TLA2528(struct TLA2528 *tla);

// read sensors from last to first
void read_TLA2528(struct TLA2528 *tla, uint8_t pin, uint16_t vals[]);
