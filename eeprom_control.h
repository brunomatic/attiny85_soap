/*
 * eeprom_control.h
 *
 *  Created on: 8. pro 2016.
 *      Author: Bruno
 */

#ifndef EEPROM_CONTROL_H_
#define EEPROM_CONTROL_H_

void init_eeprom_control();
void write_to_eeprom(uint32_t value[PARAMETERS_EEPROM]);
uint32_t read_from_eeprom(uint8_t i);
void clear_eeprom();
uint32_t read_serial_eeprom();
uint16_t read_crc();
void dump_eeprom();

#endif /* EEPROM_CONTROL_H_ */
