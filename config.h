/*
 * config.h
 *
 *  Created on: 7. pro 2016.
 *      Author: Bruno
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * General settings
 */
#define DEBUG 1								// debug levels 4-highest(+EEPROM dumping) 0-no debug info
#define CLEAR_EEPROM 0						// 1 - writes serial number and erases EEPROM on startup, 0 - EEPROM intacted


/**
 *  Pin layout definition
 */

#define INFO_LED	PB4
#define IR_EMITTER	PB0
#define IR_DETECTOR	PB2
#define PUMP		PB3
#define UART		PB1


/**
 * EEPROM settings
 */
#define EEPROM_SIZE	512						// Size of MCU EEPROM in bytes
#define EEPROM_OFFSET 4						// Initial offset of EEPROM, if specified 0-EEPROM_OFFSET is not used by MCU EEPROM control
#define SN		0x00000001					// Serial number of device
#define PARAMETERS_EEPROM	2				// Number of parameters periodically stored to EEPROM by EEPROM control
#define STORE_EEPROM_CYCLES	3000			// Storing period ( each cycle 100ms) default: 3000(five minutes),max: 2^16


/**
 * IR gate settings
 */
#define IR_GATE_DELAY 8						// Delay between activating IR emitter and reading receiver in us



#define PUMP_CYCLES 30						// Pump active intervals
#define PUMP_WAIT_CYCLES 200				// Pump reactivation wait time( between activations )

/**
 *  Timer settings
 */
#define TIMER WDT							// Select between timer1 and WDT as interrupt source for sampling
#if (TIMER == TIMER1)						// If WDT selected a cycle is ~125ms if TIMER1 it can be set
#define SAMPLE_INTERVAL 100000				// Sample interval in us
#define TIMER_PRESCALER 8192
#define TIMER_COUNTER ((SAMPLE_INTERVAL)/(1000000/(F_CPU/TIMER_PRESCALER)))
#endif

/**
 * Battery settings
 */
#define MIN_VOLTAGE	1750					// Minimal operating voltage in mV
#define LOW_BATTERY_VOLTAGE 2000			// Low battery warning - info led blinks if voltage below this
#define BATTERY_CHECK_CYCLES 6000			// Battery check period - default: 6000(ten minutes), max:2^16


/**
 * Power save system settings
 */
#if (TIMER == WDT)
#define POWER_SAVE_MODE	SLEEP_MODE_PWR_DOWN		// MCU power save mode between wake ups
#else
#define POWER_SAVE_MODE	SLEEP_MODE_IDLE
#endif

/**
 * USI UART emulation
 */
#define BAUDRATE            9600
#define STOPBITS            1

#define CYCLES_PER_BIT       ( (F_CPU) / (BAUDRATE) )
#if (CYCLES_PER_BIT > 255)
#define DIVISOR             8
#define CLOCKSELECT         2
#else
#define DIVISOR             1
#define CLOCKSELECT         1
#endif
#define FULL_BIT_TICKS      ( (CYCLES_PER_BIT) / (DIVISOR) )


#endif /* CONFIG_H_ */
