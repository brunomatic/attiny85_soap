/*
 * power_control.c
 *
 *	Power control of MCU is handled here.
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */

#include <avr/sleep.h>
#include <avr/power.h>
#include "config.h"
#include "power_control.h"

void power_save(void) {
	set_sleep_mode(POWER_SAVE_MODE);
	sleep_enable();
	power_adc_disable();
	sleep_mode();

	sleep_disable();

	power_all_enable();
}
