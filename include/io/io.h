#pragma once

#define GPIO_PIN_CNT 3

#define ADC_PIN_CNT 3

void init_pins(void);

uint16_t read_col(uint n);

void enable_row(uint n);
