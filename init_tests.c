/*
 * init_tests.c
 *
 *	These tests run right after SP initialization due to usage of .init3 section
 *	Also no variables are stored on stack due to register declaration used
 *
 *	Followed AVR998 guide, March B test is implemented with testing for stuck at faults
 *
 *	Original code:
 *
 *  Created on: 12. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>
#include "config.h"

void sram_init_test(void) __attribute__((section(".init3"))) __attribute__((naked));
void sram_init_test() {

	register uint8_t * data;
	register uint8_t error = 0;

	/* Test phase 1: write 0 any direction */
	for (data = (uint8_t *) SRAM_START_ADR; (uint16_t) data <= SRAM_END_ADR;
			data++) {
		*data = 0x00;
	}

	/* Test phase 2: read 0, write 1, read1, write 0, read 0, write 1 bottom up. */
	for (data = (uint8_t *) SRAM_END_ADR; (uint16_t) data >= SRAM_START_ADR;
			data--) {
		if (*data != 0x00)
			error = 1;

		*data = 0xFF;

		if (*data != 0xFF)
			error = 1;

		*data = 0x00;

		if (*data != 0x00)
			error = 1;

		*data = 0xFF;

	}

	/* Test phase 3: read 1, write 0, write 1 bottom up. */
	for (data = (uint8_t *) SRAM_END_ADR; (uint16_t) data >= SRAM_START_ADR;
			data--) {
		if (*data != 0xFF)
			error = 1;

		*data = 0x00;

		*data = 0xFF;

	}

	/* Test phase 4: read 1, write 0, write1, write 0 top down. */
	for (data = (uint8_t *) SRAM_START_ADR; (uint16_t) data <= SRAM_END_ADR;
			data++) {
		if (*data != 0xFF)
			error = 1;

		*data = 0x00;

		*data = 0xFF;

		*data = 0x00;

	}

	/* Test phase 5: read 0, write 1, write 0 top down. */
	for (data = (uint8_t *) SRAM_START_ADR; (uint16_t) data <= SRAM_END_ADR;
			data++) {
		if (*data != 0x00)
			error = 1;

		*data = 0xFF;

		*data = 0x00;

	}

	/* Test phase 6: Atemel's recommended additional test. */
	for (data = (uint8_t *) SRAM_START_ADR; (uint16_t) data <= SRAM_END_ADR;
			data++) {
		*data = 0x55;
		if (*data != 0x55)
			error = 1;

		*data = 0xAA;
		if (*data != 0xAA)
			error = 1;

	}

	/*
	 * Store error value on stack for later check
	 */
	if (error) {
		asm volatile("		push %0 	\n\t" : "=r" (error));
	}
}
