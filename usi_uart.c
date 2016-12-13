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

void usi_uart_send_byte(uint8_t data) {
	while (usi_uart_send_state != AVAILABLE)
		;

	usi_uart_send_state = FIRST;
	usi_uart_tx_data = reverse_byte(data);

	// Configure Timer0
	TCCR0A = 2 << WGM00;                      // CTC mode
	TCCR0B = CLOCKSELECT;                   // Set prescaler to clk or clk /8
	GTCCR |= 1 << PSR0;                     // Reset prescaler
	OCR0A = FULL_BIT_TICKS;                 // Trigger every full bit width
	TCNT0 = 0;                              // Count up from 0

	// Configure USI to send high start bit and 7 bits of data
	USIDR = 0x00 |                            // Start bit (low)
			usi_uart_tx_data >> 1;    // followed by first 7 bits of serial data
	USICR = (1 << USIOIE) |                // Enable USI Counter OVF interrupt.
			(0 << USIWM1) | (1 << USIWM0) | // Select three wire mode to ensure USI written to PB1
			(0 << USICS1) | (1 << USICS0) | (0 << USICLK); // Select Timer0 Compare match as USI Clock source.
	DDRB |= (1 << PB1);                        // Configure USI_DO as output.
	USISR = 1 << USIOIF |                  // Clear USI overflow interrupt flag
			(16 - 8);                     // and set USI counter to count 8 bits
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

// USI overflow interrupt indicates we have sent a buffer
ISR (USI_OVF_vect) {
	if (usi_uart_send_state == FIRST) {
		usi_uart_send_state = SECOND;
		USIDR = usi_uart_tx_data << 7  // Send last 1 bit of data
		| 0x7F;                           // and stop bits (high)
		USISR = 1 << USIOIF |              // Clear USI overflow interrupt flag
				(16 - (1 + (STOPBITS))); // Set USI counter to send last data bit and stop bits
	} else {
		PORTB |= 1 << PB1;                    // Ensure output is high
		USICR = 0;                            // Disable USI.
		USISR |= 1 << USIOIF;                   // clear interrupt flag

		usi_uart_send_state = AVAILABLE;
	}
}
