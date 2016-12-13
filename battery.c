/*
 * battery.c
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>
#include "config.h"
#include "battery.h"

/**
 * Measures battery voltage trough ADC
 */
uint16_t readVcc() {
	uint16_t result;
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
	ADMUX = (1 << MUX3) | (1 << MUX2);
	ADCSRA |= (1 << ADSC);
	while ( ADCSRA & (1 << ADSC))
		;
	result = ADCL;
	result |= ADCH << 8;
	result = 1126400L / result;
	ADCSRA &= ~(1 << ADEN);
	return result;
}
