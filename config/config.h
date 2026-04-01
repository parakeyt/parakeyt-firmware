// this file generated for parakeyt
#pragma once
#include "class/hid/hid.h"

// parameters
#define THRESH 50
#define NKRO 6
#define POLL 1

// IO expanders
#define ADC_CNT 4
#define DRIVER_CNT 4
#define ADC_ADDRS { 0x17 }
#define DRIVER_ADDRS { 0x16 }

// dimensions
#define LAYERS 1
#define ROWS 4
#define COLUMNS 4

// rows: {driver, pin}
#define ROWS_MAP { { 0, 7 }, { 0, 6 }, { 0, 5 }, { 0, 4 } }

// cols: {driver, pin}
#define COLS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } }

// keymap[layers][rows][cols]
#define KEYCODE_MAP { { \
{ HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_SLASH }, \
{ HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_KEYPAD_MULTIPLY }, \
{ HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_KEYPAD_SUBTRACT }, \
{ HID_KEY_RETURN, HID_KEY_0, HID_KEY_PERIOD, HID_KEY_KEYPAD_ADD }  \
} }
