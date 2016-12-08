/*
 * tests.c
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include "battery.h"
#include "config.h"
#include "tests.h"

uint8_t test_info_led(void) {

	PORTB |= (1 << INFO_LED);

	_delay_ms(1000);

	PORTB &= ~(1 << INFO_LED);

	return 1;
}

uint8_t test_pump(void) {

	PORTB |= (1 << PUMP);

	_delay_ms(2000);

	PORTB &= ~(1 << PUMP);

	return 1;
}

uint8_t test_IR(void) {

	uint8_t ret = 0;

	PORTB |= (1 << IR_EMITTER);

	_delay_us(IR_GATE_DELAY);

	if (PINB & (1 << IR_DETECTOR)) {
		ret = 0;

	} else {
		ret = 1;
	}
	PORTB &= ~(1 << IR_EMITTER);
	return ret;
}

uint8_t test_EEPROM(void) {

	if (SN == eeprom_read_dword(0)) {
		return 1;
	}
	return 0;
}

uint8_t test_battery(void) {
	uint16_t voltage;

	voltage = readVcc();
	if (MIN_VOLTAGE > voltage) {
		return 0;
	}
	return 1;
}

uint8_t system_self_test(void) {
	uint8_t test_output = 0;

	test_output = test_info_led();
	if (1 != test_output) {
		return 2;
	}

	test_output = test_pump();
	if (1 != test_output) {
		return 3;
	}

	test_output = test_IR();
	if (1 != test_output) {
		return 4;
	}

	test_output = test_EEPROM();
	if (1 != test_output) {
		return 5;
	}

	test_output = test_battery();
	if (1 != test_output) {
		return 6;
	}

	return 1;
}
