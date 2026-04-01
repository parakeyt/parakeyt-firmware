#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/watchdog.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#include "../include/io/io.h"
#include "../config/config.h"

void run_core0();
void run_core1();

int main()
{
	bi_decl(bi_program_description("matrix printer"));

	// init
	set_sys_clock_khz(200000, 1); // overclock
	stdio_init_all();
	printf("starting\n");
	init_onboard_led();
	set_onboard_led(25, 25, 0);
	init_pins();
	set_onboard_led(0, 25, 0);

	// main loop
	run_core0();

	// reset the pico if event loop ends
	watchdog_enable(1, 1);
	while (1)
		;
}

#define POLLS 2
static uint16_t matrix[ROWS][COLUMNS] = { 0 };
void run_core0()
{
	while (1) {
		// collect (for time measure purpose)
		set_onboard_led(25, 0, 25);
		uint64_t st = time_us_64();
		for (int z = 0; z < POLLS; ++z) {
			for (int i = 0; i < ROWS; ++i) {
				enable_row(i);
				for (int j = 0; j < COLUMNS; ++j) {
					read_col(j, &matrix[i][j]);
				}
			}
		}
		uint64_t dt = time_us_64() - st;

		// print
		set_onboard_led(0, 25, 0);
		printf("final state at %lli ms:\n", st / 1000);
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLUMNS; ++j) {
				printf("[%i] ", matrix[i][j]);
			}
			printf("\n");
		}
		printf("elapsed time on collection: %lli us\n", dt);
		printf("that's %lli.%lli s\n", dt / 1000000, dt % 1000000);
	}
}
