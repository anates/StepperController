/*
 * Usart_control.h
 *
 * Created: 03.08.2015 15:14:54
 *  Author: Roland-User
 */ 


#ifndef USART_CONTROL_H_
#define USART_CONTROL_H_

#include <asf.h>
#include <avr/io.h>
#include <usart.h>
#include <sysclk.h>

#define USART_SERIAL &USARTD0
#define USART_SERIAL_BAUDRATE 9600
#define USART_SERIAL_CHAR_LENGTH USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT false


USART_t *usart_module;

bool init_usart(void);
bool send_data_8(char a);//Has to be extended for larger data input
bool send_data_32(uint32_t * data32, size_t data32_size);
bool rec_data(uint32_t *data, size_t data_size);

#endif /* USART_CONTROL_H_ */