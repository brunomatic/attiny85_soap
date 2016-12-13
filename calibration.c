/*
 * calibration.c
 *
 * Function used for calibrating internal RC oscillator.
 * Using these parameters timer0 should produce interrupts
 * on 10kHz frequency. In interrupt routine pin is toggled.
 *
 * Usage:
 *  1. Set initial value for OSCCAL register
 *  2. Run the code on a targeted device with pin connected to oscilloscope
 *  3. Measure the frequency of signal on pin, it should be around 10kHz
 *  4. Adjust the OSCCAL value
 *  5. Repeat steps 3-4 until desired accuracy in achieved
 *
 *  Created on: 7. pro 2016.
 *      Original author: Mark Osborne, BecomingMaker.com
 *      Adaptation to AVR-GCC and ATtiny85V: Bruno
 */

#include <avr/io.h>
#include <avr/interrupt.h>

 // Timer/Counter0 Comparator A interrupt vector
 ISR (TIMER0_COMPA_vect) {
     PORTB ^= 1<< PINB4;         // Invert pin PB4
 }

void calibration(void) {
	OSCCAL = 108;	                // User calibration
	DDRB = 1 << PINB4;          // Set PB4 to output
	TCNT0 = 0;                  // Count up from 0
	TCCR0A = 2 << WGM00;          // CTC mode
	if (CLKPR == 3)             // If clock set to 1MHz
		TCCR0B = (1 << CS00);     // Set prescaler to /1 (1uS at 1Mhz)
	else
		// Otherwise clock set to 8MHz
		TCCR0B = (2 << CS00);     // Set prescaler to /8 (1uS at 8Mhz)
	GTCCR |= 1 << PSR0;         // Reset prescaler
	OCR0A = 49;                 // 49 + 1 = 50 microseconds (10KHz)
	TIFR = 1 << OCF0A;          // Clear output compare interrupt flag
	TIMSK |= 1 << OCIE0A;         // Enable output compare interrupt
	sei();                      // Enable global interrupts

	while (1) {
	}
}
