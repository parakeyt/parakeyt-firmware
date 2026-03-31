#pragma once

void init_pins(void);

void read_col(uint n, uint16_t *buf);

void enable_row(uint n);

void init_onboard_led(); // special

void set_onboard_led(uint8_t r, uint8_t g, uint8_t b);
