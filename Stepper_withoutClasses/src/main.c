/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/io.h>
#include <LED_control.h>
#include <Relay_control.h>
#include <AddFunctions.h>
//#include <Usart_control.h>
#include "usart_driver.h"
#include <SPI_control.h>
#include <StepperControl.h>

#ifndef USART_CONTROL_H_
#define USART_SERIAL &USARTD0
#define USART_SERIAL_BAUDRATE 9600
#define USART_SERIAL_CHAR_LENGTH USART_CHSIZE_9BIT_gc
#define USART_SERIAL_PARITY USART_PMODE_EVEN_gc
#define USART_SERIAL_STOP_BIT true
#endif

#ifndef EEPROM_CONTROL_H_
#define POSITION_ADDR EEPROM_PAGE_SIZE
#define SPEED_ADDR 2*EEPROM_PAGE_SIZE

#define NUMBER_CTRL_BYTES 4

uint8_t receiveArray[NUMBER_CTRL_BYTES];
uint8_t sendArray[NUMBER_CTRL_BYTES + 1];
#endif

USART_data_t USART_data;

void init_usart_driver(void)
{
	PORTD.DIRSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;
	USART_InterruptDriver_Initialize(&USART_data, &USARTD0, USART_DREINTLVL_LO_gc);
	
	USART_Format_Set(USART_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_EVEN_gc, false);
	USART_RxdInterruptLevel_Set(USART_data.usart, USART_RXCINTLVL_LO_gc);
	
	USART_Baudrate_Set(&USARTD0, 207, 0);
	USART_Rx_Enable(USART_data.usart);
	USART_Tx_Enable(USART_data.usart);
	
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
}

ISR(USARTD0_RXC_vect)
{
	USART_RXComplete(&USART_data);
}

ISR(USARTD0_DRE_vect)
{
	USART_DataRegEmpty(&USART_data);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	board_init();
	sysclk_init();
	ioport_init();
	sei();
	//Own variables
	//Error variable
	bool error = false;
	//Stepper variables
	uint32_t currentPosition = 0;
	bool dir; //True: Aufwärts, False: Abwärts
	uint32_t currentSpeed = 0;
	//Create command & data storage
	uint8_t commandBase[NUMBER_CTRL_BYTES];
	//Memory
	uint8_t write_page[EEPROM_PAGE_SIZE];
	uint8_t read_page[EEPROM_PAGE_SIZE];
	//Own init routine
	init_led_pins();
	init_relay_pins();
	init_usart_driver();
	/*//<USART Init>
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	sysclk_enable_module(SYSCLK_PORT_D, PR_USART0_bm);
	usart_init_rs232(USART_SERIAL, &USART_SERIAL_OPTIONS);
	//</USART Init>*/
	SPI_init_pins();
	SPI_init_module();
	/* Insert application code here, after the board has been initialized. */
	//Telling that I am alive
	switch_led_on(RUNNING);
	switch_led_on(MOVING);
	switch_led_on(RXTX);
	delay_ms(100);
	switch_led_off(RXTX);
	switch_led_off(MOVING);
	delay_ms(100);
	//Initial light show done, resetting stepper
	reset_stepper();
	delay_ms(100);
	//Testing relays
	relay_switch_mode();
	relay_switch_mode();
	//Getting data from memory, untested!
	memset(read_page, 0x0, EEPROM_PAGE_SIZE);
	nvm_eeprom_read_buffer(POSITION_ADDR, read_page, EEPROM_PAGE_SIZE);
	currentPosition = ((uint32_t)read_page[0] << 24) | ((uint32_t)read_page[1] << 16) | ((uint32_t)read_page[2] << 8) | read_page[3];
	memset(read_page, 0x0, EEPROM_PAGE_SIZE);
	nvm_eeprom_read_buffer(SPEED_ADDR, read_page, EEPROM_PAGE_SIZE);
	currentSpeed = ((uint32_t)read_page[0] << 24) | ((uint32_t)read_page[1] << 16) | ((uint32_t)read_page[2] << 8) | read_page[3];
	memset(read_page, 0x0, EEPROM_PAGE_SIZE);
	//blinking after fetching data from EEPROM, maybe include something to test data?
	switch_led_on(RXTX);
	delay_ms(50);
	switch_led_on(MOVING);
	delay_ms(50);
	switch_led_off(RXTX);
	delay_ms(50);
	switch_led_off(MOVING);
	delay_ms(50);
	//Stepper programming
	switch_led(MOVING, true);
	switch_led(RXTX, true);
	uint32_t program_stepper_commands[6];
	//Param 1: Set Current Position to zero, is not needed after RESET
	program_stepper_commands[0] = SetParam(0x01, 0x0, 0x0, 0x0);
	//Param 2: Set constant speed KValRun to x0C
	program_stepper_commands[1] = SetParam(0x0A, 0x0, 0x0, 0x0C);
	//Param 3: Set KValAcc to x0C
	program_stepper_commands[2] = SetParam(0x0B, 0x0, 0x0, 0x0C);
	//Param 4: Set KValDec to x0C
	program_stepper_commands[3] = SetParam(0x0C, 0x0, 0x0, 0x0C);
	//Param 5: Set StepMode to 0x01
	program_stepper_commands[4] = SetParam(0x16, 0x0, 0x0, 0x01);
	//Param 6: Set Alarm_EN to 0xE0
	program_stepper_commands[5] = SetParam(0x17, 0x0, 0xF, 0xE0);
	for(int i = 0; i < 6; i++)
		SPI_write_data_32(&program_stepper_commands[i]);
	uint8_t command = StepClock(1);
	SPI_write_data_8(&command);
	for(int i = 0; i < 100; i++)
		doStep(10000);
	command = GoHome();
	SPI_write_data_8(&command);
	switch_led(MOVING, false);
	switch_led(RXTX, false);	
	
	//Test routines
	uint8_t counter = 0;
	for(counter = 0; counter < 10; counter++)
	{
		blink_led(FAILURE, 100);
		delay_ms(100);
	}
	for(counter = 0; counter < 10; counter++)
	{
		relay_switch_mode();
		blink_led(RUNNING, 100);
		delay_ms(100);
	}
	for(counter = 0; counter < 100; counter++)
	{
		blink_led(RXTX, 100);
		//send_data_8('a');
		uint8_t *data = &counter;
		SPI_write_data_8(data);
		uint8_t byte = counter;
		usart_put(&USARTD0, byte);
		delay_ms(100);
	}
	
	//Tests done, writing into eeprom
	//possible light show?
	if(error != true)
	{
		memset(write_page, 0x0, EEPROM_PAGE_SIZE);
		write_page[0] = currentPosition >> 24;
		write_page[1] = currentPosition >> 16;
		write_page[2] = currentPosition >> 8;
		write_page[3] = currentPosition;
		nvm_eeprom_load_page_to_buffer(write_page);
		nvm_eeprom_atomic_write_page(POSITION_ADDR);
		memset(write_page, 0x0, EEPROM_PAGE_SIZE);
		write_page[0] = currentSpeed >> 24;
		write_page[1] = currentSpeed >> 16;
		write_page[2] = currentSpeed >> 8;
		write_page[3] = currentSpeed;
		nvm_eeprom_load_page_to_buffer(write_page);
		nvm_eeprom_atomic_write_page(POSITION_ADDR);
		memset(write_page, 0x0, EEPROM_PAGE_SIZE);
		switch_led(RUNNING, false);
		switch_led(RXTX, true);
		switch_led(MOVING, true);
	}
	else
	{
		cli();
		while(1)
		{
			blink_led(FAILURE, 100);
		}
	}
	//Next iteration would be a separate power control chip for shutdown
}
