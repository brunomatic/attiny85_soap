/*
 * main.c
 *
 *  Created on: 6. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"
#include "usi_uart.h"
#include "timer.h"
#include "ir_gate.h"
#include "eeprom_control.h"
#include "battery.h"
#include "power_control.h"
#include "tests.h"

uint32_t data[PARAMETERS_EEPROM];

uint16_t pump_cycles;
uint16_t pump_wait_cycles;
uint16_t battery_check_cycles;
uint16_t battery;
uint8_t battery_low;
volatile uint8_t timer_wake;

enum states {
	IDLE, PUMP_ACTIVE
};

enum states state;

ISR(WDT_vect) {
	timer_wake = 1;
	WDTCR |= (1 << WDIE);
}

int main(void) {
	OSCCAL = 108;

	uint8_t test_output = 0;

	DDRB |= (1 << UART) | (1 << INFO_LED) | (1 << IR_EMITTER) | (1 << PUMP);

	DDRB &= ~(1 << IR_DETECTOR);

	PORTB |= (1 << UART);

	sei();

	_delay_ms(10000);

#if CLEAR_EEPROM
	clear_eeprom();
#endif

#if DEBUG > 2
	dump_eeprom();
#endif

	test_output = system_self_test();

	if (1 != test_output) {
		usi_uart_send_u32((uint8_t *) "System self-test: FAIL, error: ",
				test_output, (uint8_t *) "\r\n");
#if (HALT_ON_ERROR == 1)
		return 0;
#endif
	} else {
		usi_uart_send((uint8_t *) "System self-test: OKAY\r\n");
	}

	init_eeprom_control();

	data[0] = read_serial_eeprom();
	usi_uart_send_u32((uint8_t *) "Serial number: ", data[0],
			(uint8_t *) " \r\n");

	data[0] = read_from_eeprom(0);
	usi_uart_send_u32((uint8_t *) "Use count: ", data[0], (uint8_t *) " \r\n");

	data[1] = read_from_eeprom(1);
	usi_uart_send_u32((uint8_t *) "On count: ", data[1], (uint8_t *) " \r\n");

	timer_wake = 0;
	state = IDLE;
	pump_cycles = 0;
	pump_wait_cycles = 0;
	battery_check_cycles = 0;

	data[1]++;
	write_to_eeprom(data);

	timer_start();

	while (1) {
		if (1 == timer_wake) {

			/**
			 * State machine for pump and info led control
			 */
			switch (state) {
			case IDLE:
				if (0 == pump_wait_cycles) {
					if (ir_gate_detect()) {
#if DEBUG > 0
						usi_uart_send((uint8_t *) "Starting pump\r\n");
						usi_uart_send((uint8_t *) "Storing data to EEPROM\r\n");
#endif
						data[0]++;
						write_to_eeprom(data);
						PORTB |= (1 << INFO_LED) | (1 << PUMP);
						pump_cycles = PUMP_CYCLES;
						state = PUMP_ACTIVE;
					}
				} else {
					pump_wait_cycles--;
				}
				break;
			case PUMP_ACTIVE:
				if (0 == pump_cycles) {
#if DEBUG > 0
					usi_uart_send((uint8_t *) "Stopping pump\r\n");
#endif
					PORTB &= ~((1 << INFO_LED) | (1 << PUMP));
					pump_wait_cycles = PUMP_WAIT_CYCLES;
					state = IDLE;
				} else {
					pump_cycles--;
					if (battery_low) {
						PORTB ^= (1 << INFO_LED);
					} else {
						PORTB |= (1 << INFO_LED);
					}
				}
				break;
			}

			/**
			 *  Battery check
			 */
			if (0 == battery_check_cycles) {
				battery = readVcc();
				if (battery < LOW_BATTERY_VOLTAGE) {
					battery_low = 1;
				} else {
					battery_low = 0;
				}
#if DEBUG > 0
				usi_uart_send_u32((uint8_t *) "Battery voltage: ", battery,
						(uint8_t *) " mV\r\n");
#endif
				battery_check_cycles = BATTERY_CHECK_CYCLES;
			} else {
				battery_check_cycles--;
			}

			timer_wake = 0;

			/*
			 * Wait for UART to finish sending since it is using interrupts from timer0
			 */
			while (!usi_uart_done())
				;

			power_save();
		}
	}
}
