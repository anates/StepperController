/*
 * StepperControl.h
 *
 * Created: 09.08.2015 12:26:58
 *  Author: Roland-User
 */ 


#ifndef STEPPERCONTROL_H_
#define STEPPERCONTROL_H_

#include <asf.h>
#include <avr/io.h>

#define STDBY IOPORT_CREATE_PIN(PORTA, 5)
#define STCK IOPORT_CREATE_PIN(PORTA, 6)
#define BUSY IOPORT_CREATE_PIN(PORTB, 0)
#define FLAG IOPORT_CREATE_PIN(PORTB, 1)

bool init_stepper_pins(void);
bool reset_stepper(void);
bool get_error_status(void);
bool doStep(uint32_t delay_time);
#endif /* STEPPERCONTROL_H_ */