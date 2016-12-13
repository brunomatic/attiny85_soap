/*
 * sth.c
 *
 *  Created on: 6. pro 2016.
 *      Author: Bruno
 */

/*
 Original Author: Mark Osborne, BecomingMaker.com
 Adaptation to ATtiny and AVR-GCC compiler: Bruno Matic
 */

/* Supported combinations:
 *   F_CPU 1000000   BAUDRATE 1200, 2400
 *   F_CPU 8000000   BAUDRATE 9600, 19200
 *   F_CPU 16000000  BAUDRATE 9600, 19200, 28800, 38400
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include "usi_uart.h"
#include "config.h"

enum USI_UART_SEND_STATE {
	AVAILABLE, FIRST, SECOND
};
static volatile enum USI_UART_SEND_STATE usi_uart_send_state = AVAILABLE;

static volatile uint8_t usi_uart_tx_data;

static uint8_t reverse_byte(uint8_t x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

/**
 * Configure timer0:
 * 		- CTC mode
 * 		- set prescaler based on baudrate
 * 		- fill compare(0CR0A) register
 * 		- start counting
 *
 * 	Configure USI:
 * 		- fill send register with start bit and 7 data bits
 * 		- set three wire mode to use PB1 as output pin
 * 		- set pin as output
 * 		- set bit counter to 8 bits
 */
void usi_uart_send_byte(uint8_t data) {
	while (usi_uart_send_state != AVAILABLE)
		;

	usi_uart_send_state = FIRST;
	usi_uart_tx_data = reverse_byte(data);

	TCCR0A = 2 << WGM00;
	TCCR0B = CLOCKSELECT;
	GTCCR |= 1 << PSR0;
	OCR0A = FULL_BIT_TICKS;
	TCNT0 = 0;

	USIDR = 0x00 | usi_uart_tx_data >> 1;
	USICR = (1 << USIOIE) | (0 << USIWM1) | (1 << USIWM0) | (0 << USICS1)
			| (1 << USICS0) | (0 << USICLK);
	DDRB |= (1 << UART);
	USISR = 1 << USIOIF | (16 - 8);
}

void usi_uart_send(uint8_t * data) {
	uint16_t i = 0;
	while (!(0x00 == data[i])) {
		usi_uart_send_byte(data[i]);
		i++;
	}
}

void usi_uart_send_u32(uint8_t before[], uint32_t number, uint8_t after[]) {
	uint8_t temp[11];
	utoa(number, (char *) temp, 10);
	usi_uart_send(before);
	usi_uart_send(temp);
	usi_uart_send(after);
}

uint8_t usi_uart_done() {
	if (usi_uart_send_state == AVAILABLE) {
		return 1;
	}
	return 0;
}

/**
 * Overflow interrupt handling:
 * 	- switch sending data: change state to second, send last bit of data and stop bits
 * 	- finish sending: change state to available, set port high and disable USI counting
 */
ISR (USI_OVF_vect) {
	if (usi_uart_send_state == FIRST) {
		usi_uart_send_state = SECOND;
		USIDR = usi_uart_tx_data << 7 | 0x7F;
		USISR = 1 << USIOIF | (16 - (1 + (STOPBITS)));
	} else {
		PORTB |= 1 << UART;
		USICR = 0;
		USISR |= 1 << USIOIF;

		usi_uart_send_state = AVAILABLE;
	}
}
