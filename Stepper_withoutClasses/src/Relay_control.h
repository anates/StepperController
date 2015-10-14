/*
 * Relay_control.h
 *
 * Created: 03.08.2015 14:29:09
 *  Author: Roland-User
 */ 


#ifndef RELAY_CONTROL_H_
#define RELAY_CONTROL_H_

#include <asf.h>
#include <delay.h>
#include <avr/io.h>

#define RELAY0 IOPORT_CREATE_PIN(PORTE, 0)
#define RELAY1 IOPORT_CREATE_PIN(PORTE, 1)


bool init_relay_pins(void);
bool relay_switch_mode(void);


#endif /* RELAY_CONTROL_H_ */