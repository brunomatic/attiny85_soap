/*
 * usi_uart.h
 *
 *  Created on: 7. pro 2016.
 *      Author: Bruno
 */

#ifndef USI_UART_H_
#define USI_UART_H_

void usi_uart_send_byte(uint8_t data);
void usi_uart_send(uint8_t * data);
void usi_uart_send_u32(uint8_t before[], uint32_t number, uint8_t after[]);
uint8_t usi_uart_done();

#endif /* USI_UART_H_ */
