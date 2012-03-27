/*
 *
 *  Generic H-bridge motor controller driver
 *
 * motor.h
 *
 *  Created on: March 14
 *      Author: Andrew LeCain
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <math.h>

#include "pwm.h"
#include "iodefs.h"

typedef enum {
	freewheel	= 0b00,
			forward 	= 0b01,
			backward	= 0b10,
			brake		= 0b11
}motor_mode;

typedef struct{
	volatile uint8_t* cnt_port; //motor control ouput port
	char output_1_pin;
	char output_2_pin;
	motor_mode current_mode;
	pwm_args pwm;
}motor_t;

void motor_setSpeed(motor_t* motor, int8_t speed);

/**
 * Set the motor mode
 *
 * mode=freewheel,forward,backward,brake
 */
void motor_setMode(motor_t* motor, motor_mode mode);
void init_motor(motor_t* motor, volatile uint8_t* port, char output_1_pin, char output_2_pin, char pwmPin);

#endif /* MOTOR_H_ */
