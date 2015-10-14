/*
 * Usart_control.c
 *
 * Created: 03.08.2015 15:15:05
 *  Author: Roland-User
 */ 

#include <Usart_control.h>

static usart_rs232_options_t USART_SERIAL_OPTIONS;

bool init_usart()
{
	usart_rs232_options_t options = {.baudrate = USART_SERIAL_BAUDRATE, 
		.charlength = USART_SERIAL_CHAR_LENGTH, 
		.paritytype = USART_SERIAL_PARITY, 
		.stopbits = USART_SERIAL_STOP_BIT};
	USART_SERIAL_OPTIONS = options;
	sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	usart_module = &USARTD0;
	usart_init_rs232(usart_module, &USART_SERIAL_OPTIONS);
	return true;
}

bool send_data_8(char a)
{
	usart_putchar(usart_module, a);
	return true;
}

bool send_data_32(uint32_t * data32, size_t data32_size)
{
	size_t data_size = 4*data32_size;
	uint8_t data[data_size];
	for(unsigned int i = 0; i < data32_size; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			data[i*j+j] = (data32[i] << (j*8));
		}
	}
	usart_serial_write_packet(usart_module, data, data_size * sizeof(uint8_t));
	return true;
}

bool rec_data(uint32_t *data, size_t data_size)
{
	usart_serial_read_packet(usart_module, data, ((data_size < 32)?data_size:32)*sizeof(uint8_t));
	return true;
}