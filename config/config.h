// hand-written config.h for the 4x4 reference design
#include "class/hid/hid.h"

// parameters
#define THRESH 30
#define NKRO 6

// it is currently assumed all expanders are on i2c0
#define ADC_CNT 1
#define ADC_ADDRS { 0x17 }
#define DRIVER_CNT 1
#define DRIVER_ADDRS { 0x16 }

// dimensions
#define LAYERS 1
#define ROWS 4
#define COLUMNS 4

// {driver, pin}
#define ROWS_MAP { { 0, 7 }, { 0, 6 }, { 0, 5 }, { 0, 4 } }

// {adc, pin}
#define COLS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } }

// [layer][row][col]
#define KEYCODE_MAP                                                                                                \
	{                                                                                                          \
		{                                                                                                  \
			/* LAYER 1 */                                                                              \
			{ HID_KEY_KEYPAD_7, HID_KEY_KEYPAD_8, HID_KEY_KEYPAD_9, HID_KEY_KEYPAD_DIVIDE },           \
				{ HID_KEY_KEYPAD_4, HID_KEY_KEYPAD_5, HID_KEY_KEYPAD_6, HID_KEY_KEYPAD_MULTIPLY }, \
				{ HID_KEY_KEYPAD_1, HID_KEY_KEYPAD_2, HID_KEY_KEYPAD_3, HID_KEY_KEYPAD_SUBTRACT }, \
			{                                                                                          \
				HID_KEY_NUM_LOCK, HID_KEY_KEYPAD_0, HID_KEY_KEYPAD_DECIMAL, HID_KEY_KEYPAD_ADD     \
			}                                                                                          \
		}                                                                                                  \
	}
