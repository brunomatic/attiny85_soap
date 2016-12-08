/*
 * timer.c
 *
 *  Created on: 7. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>�
#include <avr/wdt.h>        // Supplied Watch Dog Timer Macros
#include "config.h"
#include "timer.h"

void timer_start(void) {

#if (TIMER == WDT)
	MCUSR = 0;
	WDTCR = (1 << WDCE) | (1 << WDE);
	WDTCR = 0;
	WDTCR = (1 << WDIE) | (1 << WDP1) | (1 << WDP0);
#else
	OCR1A = TIMER_COUNTER;
	TIMSK = 1 << OCIE1A;
	TCCR1 = (1 << CS13) | (1 << CS12) | (1 << CS11);
#endif
}