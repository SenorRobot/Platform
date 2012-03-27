/*
 * motor.c
 *
 *  Created on: Feb 24, 2012
 *      Author: aah9038
 */

#include <stdlib.h>
#include "motor.h"

/**
 * Set the motor speed
 *
 * single signed short from -127 to 127
 */
void motor_setSpeed(motor_t* motor, int8_t speed){

	if(speed <0){
		motor_setMode(motor, backward);
	}
	else{
		motor_setMode(motor, forward);
	}

	//Dead zone insertion
	if(abs(speed)>1){
		pwm_setDuty(&motor->pwm, abs(speed)*2);
	}else{
		motor_setMode(motor, brake);	
		pwm_setDuty(&motor->pwm, 0);
	}
}


/**
 * Set the motor mode
 *
 * mode=freewheel,forward,backward,brake
 */
void motor_setMode(motor_t* motor, motor_mode mode){

	if (motor->current_mode!=mode){
		motor->current_mode=mode;

		//set the appropraite bits of  the motor control.
		setPin(motor->cnt_port,motor->output_1_pin,(mode & 0b01));
		setPin(motor->cnt_port,motor->output_2_pin,(mode & 0b10));
	}
}

/**
 * Initializes a motor object
 *
 * mode=freewheel,forward,backward,brake
 */
void init_motor(motor_t* motor, volatile uint8_t* port, char output_1_pin, char output_2_pin, char pwmPin){

	motor->cnt_port=port;
	
	motor->output_1_pin=output_1_pin;
	motor->output_2_pin=output_2_pin;
	
	*getDDR(port) |= (1<<output_1_pin);
	*getDDR(port) |= (1<<output_2_pin);

	motor_setMode(motor,brake);
	pwm_init(&motor->pwm,pwmPin);

}

