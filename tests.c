/*
 * tests.c
 *
 *	Testing functions executed on system start are here
 *
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
#include "usi_uart.h"

uint8_t test_battery(void) {
	uint16_t voltage;

	voltage = readVcc();
	if (voltage < MIN_OPERATING_VOLTAGE) {
		return BATT_LOW;
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

	usi_uart_send_u32((uint8_t *) "CRC generated: ", crc_calc, (uint8_t *) "\r\n");

	usi_uart_send_u32((uint8_t *) "CRC stored: ", crc_stored,
			(uint8_t *) "\r\n");

	return FLASH_ERR;
}

uint8_t test_sram(void) {
	uint8_t * init_test_result = (uint8_t *) SRAM_END_ADR;

	if (*init_test_result == 0x01)
		return SRAM_ERR;

	return NO_ERROR;
}

uint8_t system_self_test(void) {
	uint8_t test_output = 0;

	/* Ensure minimal operating voltage is available */
	test_output = test_battery();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	/* Check of IR gate */
	test_output = test_IR();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	/* CRC check of flash memory */
	test_output = test_flash();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	/* Check results of early SRAM testing */
	test_output = test_sram();
	if (NO_ERROR != test_output) {
		return test_output;
	}

	/* If everything OK signal/test info led */
	test_info_led();

	return NO_ERROR;
}
