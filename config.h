/*
 * config.h
 *
 *	This file contains all configurable parameters
 *
 *  Created on: 7. pro 2016.
 *      Author: Bruno
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * General settings
 * IMPORTANT: one cycle = ~125ms (WDT is used for MCU wake ups to conserve battery)
 */
#define DEBUG 1										/* Debug levels: 3-highest(+EEPROM dumping) 0-no debug info */
#define HALT_ON_ERROR 1								/* Halt on system self test failure */
#define CLEAR_EEPROM 0								/* 1 - erases EEPROM usage data on startup, 0 - EEPROM intacted */
#define SN		0x00000001							/* Serial number of device stored in EEPROM at 0 */
#define CRC16	0xDF5E								/* CRC16 stored in EEPROM at location 4 */

/**
 *  Pin layout definition
 */
#define IR_EMITTER	PB0
#define UART		PB1								/* IMPORTANT: must be PB1 */
#define IR_DETECTOR	PB2
#define PUMP		PB3
#define INFO_LED	PB4

/*
 * SRAM memory parameters
 */
#define SRAM_START_ADR	96
#define SRAM_END_ADR	RAMEND

/*
 * Flash memory parameters
 */
#define FLASH_END	FLASHEND

/**
 * EEPROM memory and control settings
 */
#define EEPROM_SIZE	512								/* Size of MCU EEPROM in bytes */
#define PARAMETERS_EEPROM	2						/* Number of parameters periodically stored to EEPROM by EEPROM control */

/**
 * IR gate settings
 */
#define IR_GATE_DELAY 8								/* Delay between activating IR emitter and reading receiver in us */

/**
 * Pump settings
 */
#define PUMP_CYCLES 24								/* Pump active intervals, default: 24(~3 seconds) */
#define PUMP_WAIT_CYCLES 160						/* Pump reactivation wait time( between activations ), default: 160(~20 seconds) */

/**
 * Battery settings
 */
#define LOW_BATTERY_VOLTAGE 5000					/* Low battery warning - info led blinks if voltage below this */
#define MIN_OPERATING_VOLTAGE 4500					/* Below this voltage system shuts down */
#define BATTERY_CHECK_CYCLES 4800					/* Battery check period - default: 4800(~10 minutes) */

/**
 * Power save system settings
 */
#define POWER_SAVE_MODE	SLEEP_MODE_PWR_DOWN			/* MCU power save mode between wake ups */

/**
 * USI UART emulation
 */
#define BAUDRATE            19200					/* Possible baudrates at 8MHz: 9600bps or 19200bps */
#define STOPBITS            1						/* Number of stop bits */

/*
 * Timer0 parameters calculation for usage with USI to form UART
 */
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
