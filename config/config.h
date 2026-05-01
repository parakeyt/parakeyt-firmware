// this file generated for parakeyt
#pragma once
#include "class/hid/hid.h"

// parameters
#define THRESH 30
#define NKRO 6
#define POLL 1

// IO expanders
#define ADC_CNT 0 // # adcs
#define DRIVER_CNT 5 // # drivers
#define ADC_ADDRS { 0x17 }
#define DRIVER_ADDRS { 0x16 }

// dimensions
#define LAYERS 1
#define ROWS 5 // total # rows
#define COLUMNS 16 // total # cols

// rows: {driver, pin}
#define ROWS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 } }

// cols: {driver, pin}
#define COLS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 }, { 0, 9 }, { 0, 10 }, { 0, 11 }, { 0, 12 }, { 0, 13 }, { 0, 14 }, { 0, 15 } }

// keymap[layers][rows][cols]
#define KEYCODE_MAP { { \
{ HID_KEY_ESC, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0, HID_KEY_KEYPAD_SUBTRACT, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_HOME }, \
{ HID_KEY_TAB, HID_KEY_NONE, HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T, HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_END }, \
{ HID_KEY_NONE, HID_KEY_NONE, HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE, HID_KEY_ENTER, HID_KEY_NONE, HID_KEY_PGUP }, \
{ HID_KEY_NONE, HID_KEY_NONE, HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V, HID_KEY_B, HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH, HID_KEY_NONE, HID_KEY_RSHIFT, HID_KEY_UP, HID_KEY_PGDN }, \
{ HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_SPACE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_RALT, HID_KEY_FN1, HID_KEY_FN2, HID_KEY_LEFT, HID_KEY_DOWN, HID_KEY_RIGHT }  \
} }
