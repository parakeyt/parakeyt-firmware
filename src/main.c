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
	bi_decl(bi_program_description("Pico hall adc test"));

	// init
	set_sys_clock_khz(200000, 1); // overclock
	stdio_init_all();
	sleep_ms(3000);
	printf("starting\n");
	init_pins();

	// main loop
	//multicore_launch_core1(&run_core1);
	run_core0();

	// reset the pico in event loop ends
	watchdog_enable(1, 1);
	while (1)
		;
}

void run_core0()
{
	printf("starting to ball\n");

	/* Test all the thingies */
	while (1) {
		// collect (for time measure purpose)
		uint16_t matrix[ROWS][COLUMNS] = { 0 };
		uint64_t st = time_us_64();
		printf("performing 1000 polls AFAP\n");
		for (int z = 0; z < 1000; ++z) {
			for (int i = 0; i < ROWS; ++i) {
				enable_row(i);
				for (int j = 0; j < COLUMNS; ++j) {
					read_col(j, &matrix[i][j]);
				}
			}
		}
		uint64_t dt = time_us_64() - st;

		// print
		printf("final state at %lli ms:\n", st / 1000);
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLUMNS; ++j) {
				printf("[%04i] ", matrix[i][j]);
			}
			printf("\n");
		}
		printf("elapsed time on collection: %lli us\n", dt);
		printf("that's %lli.%lli s\n", dt / 1000000, dt % 1000000);
		sleep_ms(2000);
	}
}

void run_core1()
{
	/* while (1) { */
	/* } */
}
