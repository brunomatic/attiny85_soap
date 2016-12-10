/*
 * eeprom_control.c
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "config.h"
#include "eeprom_control.h"
#include "usi_uart.h"

typedef struct {
	uint32_t serial;
	uint16_t crc16_add;
} eeprom_data;

const eeprom_data EEMEM initEEPROM = { SN, CRC16 };

struct {
	uint32_t sequence_no;
	uint32_t data[PARAMETERS_EEPROM];
} data_block;

#define EEPROM_OFFSET	sizeof(eeprom_data)
#define DATA_BLOCKS ((EEPROM_SIZE-EEPROM_OFFSET) / sizeof(data_block))

uint32_t last_sequence_no;
uint16_t queue_tail;
uint32_t current_value[PARAMETERS_EEPROM];

void clear_eeprom() {
	uint16_t i;
	for (i = EEPROM_OFFSET; i < EEPROM_SIZE; i++) {
		eeprom_write_byte((uint8_t *) i, 0xFF);
	}
}

void dump_eeprom() {
	uint16_t i;
	uint8_t sth;
	for (i = 0; i < EEPROM_SIZE; i++) {
		sth = eeprom_read_byte((uint8_t *) &i);
		usi_uart_send_byte(sth);
	}
}

uint32_t read_serial_eeprom() {
	return eeprom_read_dword((uint32_t *)0);
}

uint16_t read_crc(){
	return eeprom_read_word((uint16_t *)4);
}

void init_eeprom_control() {
	uint16_t i;
	uint8_t j;

	last_sequence_no = 0;
	queue_tail = 0;
	for (j = 0; j < PARAMETERS_EEPROM; j++) {
		current_value[j] = 0;
	}
	for (i = 0; i < DATA_BLOCKS; i++) {
		eeprom_read_block(&data_block,
				(uint8_t *) (i * sizeof(data_block) + EEPROM_OFFSET),
				sizeof(data_block));
		if ((data_block.sequence_no >= last_sequence_no)
				&& (data_block.sequence_no != 0xFFFFFFFF)) {
			queue_tail = i;
			last_sequence_no = data_block.sequence_no;
			for (j = 0; j < PARAMETERS_EEPROM; j++) {
				current_value[j] = data_block.data[j];
			}
		}
	}
}

void write_to_eeprom(uint32_t * value) {
	uint8_t j;
	queue_tail++;
	if (queue_tail >= DATA_BLOCKS)
		queue_tail = 0;
	last_sequence_no++;
	data_block.sequence_no = last_sequence_no;
	for (j = 0; j < PARAMETERS_EEPROM; j++) {
		data_block.data[j] = value[j];
	}
	eeprom_write_block(&data_block,
			(uint8_t *) (queue_tail * sizeof(data_block) + EEPROM_OFFSET),
			sizeof(data_block));
	for (j = 0; j < PARAMETERS_EEPROM; j++) {
		current_value[j] = value[j];
	}
}

uint32_t read_from_eeprom(uint8_t i) {
	return current_value[i];
}
