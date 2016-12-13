/*
 * timer.c
 *
 *	Watchdog timer initialization function
 *
 *  Created on: 7. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include "config.h"
#include "timer.h"

void timer_start(void) {

	MCUSR = 0;
	WDTCR = (1 << WDCE) | (1 << WDE);
	WDTCR = 0;
	WDTCR = (1 << WDIE) | (1 << WDP1) | (1 << WDP0);
}
