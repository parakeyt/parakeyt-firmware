#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/watchdog.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/vreg.h"

#include "../include/io/io.h"

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

	/* Test a single thingy */
	/* { */
	/* 	while (1) { */
	/* 		enable_row(0); */
	/* 		printf("ballin_single\n"); */
	/* 		printf("[0, 0]: %i\n", read_col(0)); */
	/* 		printf("[0, 1]: %i\n", read_col(1)); */
	/* 		printf("[0, 2]: %i\n", read_col(2)); */
	/* 		sleep_ms(1000); */
	/* 	} */
	/* } */

	/* Test all the thingies */
	while (1) {
		// collect (for time measure purpose)
		uint16_t matrix[GPIO_PIN_CNT][ADC_PIN_CNT] = { 0 };
		uint64_t st = time_us_64();
		for (int i = 0; i < GPIO_PIN_CNT; ++i) {
			enable_row(i);
			for (int j = 0; j < ADC_PIN_CNT; ++j) {
				uint16_t val = read_col(j);
				matrix[i][j] = val;
			}
		}
		uint64_t dt = time_us_64() - st;

		// print
		printf("state at %lli ms:\n", st / 1000);
		for (int i = 0; i < GPIO_PIN_CNT; ++i) {
			for (int j = 0; j < ADC_PIN_CNT; ++j) {
				printf("[%04i] ", matrix[i][j]);
			}
			printf("\n");
		}
		printf("elapsed time on collection: %lli us\n", dt);
		sleep_ms(2000);
	}
}

void run_core1()
{
	/* while (1) { */
	/* } */
}
