/*
 * ir_gate.c
 *
 *	Implements simple IR gate driver for ease of use
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */


#include <util/delay.h>
#include <avr/io.h>
#include "config.h"
#include "ir_gate.h"

uint8_t ir_gate_detect(void) {

	uint8_t ret = 0;

	PORTB |= (1 << IR_EMITTER);

	_delay_us(IR_GATE_DELAY);

	if (PINB & (1 << IR_DETECTOR)) {
		ret = 1;
	} else {
		ret = 0;
	};

	PORTB &= ~(1 << IR_EMITTER);

	return ret;

}
