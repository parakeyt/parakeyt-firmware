// this file generated for parakeyt
#pragma once
#include "class/hid/hid.h"

// parameters
#define THRESH 50
#define NKRO 6
#define POLL 1

// IO expanders
#define ADC_CNT 1
#define DRIVER_CNT 1
#define ADC_ADDRS { 0x16 }
#define DRIVER_ADDRS { 0x10 }

// dimensions
#define LAYERS 1
#define ROWS 3
#define COLUMNS 8

// rows: {driver, pin}
#define ROWS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 } }

// cols: {driver, pin}
#define COLS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 } }

// keymap[layers][rows][cols]
#define KEYCODE_MAP                                                                                                 \
	{                                                                                                           \
		{                                                                                                   \
			{ HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4 }, \
				{ HID_KEY_K, HID_KEY_L, HID_KEY_5, HID_KEY_6,                                       \
				  HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_NONE },                                  \
			{                                                                                           \
				HID_KEY_Z, HID_KEY_X, HID_KEY_NONE                                                  \
			}                                                                                           \
		}                                                                                                   \
	}
