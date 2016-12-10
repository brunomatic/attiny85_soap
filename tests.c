/*
 * tests.c
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <errno.h>
#include <util/crc16.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "battery.h"
#include "errors.h"
#include "eeprom_control.h"
#include "tests.h"

uint8_t test_battery(void) {
	uint16_t voltage;

	voltage = readVcc();
	if (voltage < LOW_BATTERY_VOLTAGE) {
		return 0;
	}
	return NO_ERROR;
}

void test_info_led(void) {

	PORTB |= (1 << INFO_LED);

	_delay_ms(1000);

	PORTB &= ~(1 << INFO_LED);

}

uint8_t test_IR(void) {

	uint8_t ret = 0;

	PORTB |= (1 << IR_EMITTER);

	_delay_us(IR_GATE_DELAY);

	if (PINB & (1 << IR_DETECTOR)) {
		ret = IR_ERR;

	} else {
		ret = NO_ERROR;
	}
	PORTB &= ~(1 << IR_EMITTER);
	return ret;
}

uint8_t test_flash(void) {
	uint8_t byte = 0x00;
	uint16_t crc_calc = 0;
	uint16_t crc_stored = 0;
	uint16_t flash_index = 0;

	for (flash_index = 0; flash_index <= FLASH_END; flash_index++) {
		byte = pgm_read_byte(flash_index);
		crc_calc = _crc_ccitt_update(crc_calc, byte);
	}

	crc_stored = read_crc();

	if (crc_stored == crc_calc)
		return NO_ERROR;

	return FLASH_ERR;
}


uint8_t test_sram(void) {
	volatile register uint8_t *p_val, i, sav;
	register uint8_t val;
	register uint16_t h;

	/* test all locations with 0x55, 0xAA and complement of address value */
	for (h = SRAM_START_ADR; h <= SRAM_END_ADR; h++) {
		// read and store location
		p_val = ((uint8_t *) (h));
		sav = *p_val;
		// write and verify
		*p_val = 0x55;
		i = *p_val;
		if (i != 0x55) {
			return SRAM_ERR;
		}
		// write and verify
		*p_val = 0xAA;
		i = *p_val;
		if (i != 0xAA) {
			return SRAM_ERR;
		}
		// address decoder check
		val = ~((uint16_t) p_val);
		*p_val = val;
		i = *p_val;
		if (i != val) {
			return SRAM_ERR;
		}

		// restore stored value
		*p_val = sav;
	}
	return NO_ERROR;
}

uint8_t system_self_test(void) {
	uint8_t test_output = 0;

	test_output = test_battery();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	test_info_led();

	test_output = test_IR();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	//test_output = test_flash();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	test_output = test_sram();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	return NO_ERROR;
}
