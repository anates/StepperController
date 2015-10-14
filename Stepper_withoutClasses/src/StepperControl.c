/*
 * StepperControl.c
 *
 * Created: 09.08.2015 12:26:44
 *  Author: Roland-User
 */ 

#include <StepperControl.h>
#include <SPI_control.h>

bool init_stepper_pins(void)
{
	ioport_set_pin_dir(STDBY, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(STCK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(STDBY, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(STCK, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(FLAG, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(BUSY, IOPORT_DIR_INPUT);
	ioport_set_pin_level(STDBY, true);
	ioport_set_pin_level(STCK, false);
	return true;
}

bool reset_stepper(void)
{
	ioport_set_pin_level(STDBY, false);
	delay_ms(100);
	ioport_set_pin_level(STDBY, true);
	delay_ms(100);
	return ioport_get_pin_level(BUSY);
}

bool get_error_status(void)
{
	return false;
}

bool doStep(uint32_t delay_time)//delay_time in us
{
	ioport_set_pin_level(STCK, true);
	delay_us(delay_time);
	ioport_set_pin_level(STCK, false);
	delay_us(delay_time);
	return true;
}