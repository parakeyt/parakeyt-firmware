#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/watchdog.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#include "bsp/board.h"
#include "tusb.h"

#include "../include/io/io.h"
#include "../include/usb_descriptors.h"
#include "../config/config.h"

void run_core0();
void run_core1();
void hid_task(void);

int main()
{
	bi_decl(bi_program_description("Pico hall adc test"));

	// init
	set_sys_clock_khz(200000, 1); // overclock
	stdio_init_all();
	printf("starting\n");
	init_onboard_led();
	set_onboard_led(25, 25, 0);
	init_pins();
	tusb_init();
	set_onboard_led(0, 25, 0);

	// main loop
	multicore_launch_core1(&run_core1);
	run_core0();

	// reset the pico if event loop ends
	watchdog_enable(1, 1);
	while (1)
		;
}

static uint16_t matrix[ROWS][COLUMNS] = { 0 };
static uint8_t pressed[ROWS][COLUMNS] = { 0 };
static bool keys_pressed = false;
static uint8_t report_data[NKRO] = { HID_KEY_KEYPAD_DECIMAL }; // codes
static uint8_t layer = 0;
static uint8_t keymap[LAYERS][ROWS][COLUMNS] = KEYCODE_MAP;

static uint8_t report_cnt = 0;
void add_to_report(uint8_t i, uint8_t j)
{
}

void remove_from_report(uint8_t i, uint8_t j)
{
}

void update_report_data(uint16_t cur, uint16_t last, uint16_t i, uint16_t j)
{
	keys_pressed = false;
}

void run_core1()
{
	while (1) {
		// run
		for (int i = 0; i < ROWS; ++i) {
			enable_row(i);
			for (int j = 0; j < COLUMNS; ++j) {
				uint16_t last = matrix[i][j];
				read_col(j, &matrix[i][j]);
				update_report_data(matrix[i][j], last, i, j);
			}
		}
	}
}

void run_core0()
{
	while (1) {
		tud_task();
		hid_task();
	}
}

static void send_hid_report(bool keys_pressed)
{
	if (!tud_hid_ready()) {
		return;
	}

	static bool send_empty = false;

	if (keys_pressed) {
		tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, report_data);
		send_empty = true;
	} else {
		// send empty key report if previously has key pressed
		if (send_empty) {
			tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
		}
		send_empty = false;
	}
}

void hid_task(void)
{
	const uint32_t interval_ms = 1;
	static uint32_t start_ms = 0;

	if (board_millis() - start_ms < interval_ms) {
		return; // not enough time
	}
	start_ms += interval_ms;

	// Remote wakeup
	if (tud_suspended() && keys_pressed) {
		// Wake up host if we are in suspend mode
		// and REMOTE_WAKEUP feature is enabled by host
		tud_remote_wakeup();
	} else {
		// send a report
		send_hid_report(keys_pressed);
	}
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
	// not implemented, we only send REPORT_ID_KEYBOARD
	(void)instance;
	(void)len;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
			       uint16_t reqlen)
{
	// TODO not Implemented
	(void)instance;
	(void)report_id;
	(void)report_type;
	(void)buffer;
	(void)reqlen;

	return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer,
			   uint16_t bufsize)
{
	(void)instance;

	if (report_type == HID_REPORT_TYPE_OUTPUT) {
		// Set keyboard LED e.g Capslock, Numlock etc...
		if (report_id == REPORT_ID_KEYBOARD) {
			// bufsize should be (at least) 1
			if (bufsize < 1)
				return;

			uint8_t const kbd_leds = buffer[0];

			if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
				// Capslock On: disable blink, turn led on
				//				blink_interval_ms = 0;
				board_led_write(true);
			} else {
				// Caplocks Off: back to normal blink
				board_led_write(false);
				//				blink_interval_ms = BLINK_MOUNTED;
			}
		}
	}
}

void tud_mount_cb(void)
{
	set_onboard_led(25, 0, 25);
}

void tud_umount_cb(void)
{
	set_onboard_led(25, 0, 0);
}

// remote_wakeup_en : if host allow us to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
	(void)remote_wakeup_en;
	set_onboard_led(25, 25, 0);
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
	set_onboard_led(25, 0, 25);
}
