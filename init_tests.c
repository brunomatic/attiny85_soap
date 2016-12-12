/*
 * init_tests.c
 *
 *  Created on: 12. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>
#include "config.h"

void sram_init_test(void) __attribute__((section(".init3"))) __attribute((naked));
void sram_init_test() {

	register uint8_t * data asm("r16");
	register uint8_t error asm("r17") = 0;

	/**
	 * Test everything to SRAM_END_ADR
	 */
	/* Test phase 1: write 0 to all bit locations. */
	for (data = (uint8_t *)SRAM_START_ADR; (uint16_t)data <= SRAM_END_ADR; data++) {
		*data = 0x00;
	}

	/* Test phase 2: read 0, write FF. */
	for (data = (uint8_t *)SRAM_START_ADR; (uint16_t)data <= SRAM_END_ADR; data++) {
		if (*data != 0x00) {
			error = 1;
		} else {
			*data = 0xFF;
		}
	}

	/* Test phase 3: read FF, write 0 (reverse order). */
	for (data = (uint8_t *)SRAM_END_ADR; (uint16_t)data >= SRAM_START_ADR; data--) {
		if (*data != 0xFF) {
			error = 1;
		} else {
			*data = 0x00;
		}
	}

	/* Test phase 4: read 0. */
	for (data = (uint8_t *)SRAM_START_ADR; (uint16_t)data <= SRAM_END_ADR; data++) {
		if (*data != 0x00) {
			error = 1;
		}
	}

	/* Intra-word march test. */
	for (data = (uint8_t *)SRAM_START_ADR; (uint16_t)data <= SRAM_END_ADR; data++) {
		*data = 0x55;
		if (*data != 0x55) {
			error = 1;
		}

		*data = 0xAA;
		if (*data != 0xAA) {
			error = 1;
		}

		*data = 0x33;
		if (*data != 0x33) {
			error = 1;
		}

		*data = 0xCC;
		if (*data != 0xCC) {
			error = 1;
		}

		*data = 0xF0;
		if (*data != 0xF0) {
			error = 1;
		}

		*data = 0x0F;
		if (*data != 0x0F) {
			error = 1;
		}
	}

	if(error){
		__asm(" 	ldi r24, 0x01 \n"
				"	push r24	\n");
	}
}
