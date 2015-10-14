/*
 * Relay_control.c
 *
 * Created: 03.08.2015 14:28:56
 *  Author: Roland-User
 */ 

#include <Relay_control.h>

bool init_relay_pins()
{
	ioport_set_pin_dir(RELAY1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(RELAY0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(RELAY0, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(RELAY1, IOPORT_MODE_PULLUP);
	ioport_set_pin_level(RELAY1, true);
	ioport_set_pin_level(RELAY0, true);
	return true;
}

bool relay_switch_mode()
{
	bool cur_lvl = ioport_get_pin_level(RELAY1);
	cur_lvl = !cur_lvl;
	ioport_set_pin_level(RELAY1, cur_lvl);
	ioport_set_pin_level(RELAY0, cur_lvl);
	delay_ms(1000);
	return true;
}