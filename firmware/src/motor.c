/*
 * motor.c
 *
 *  Created on: Feb 24, 2012
 *      Author: aah9038
 *      Modified: Alex Crawford
 */

#include <stdlib.h>
#include <math.h>
#include "iodefs.h"
#include "motor.h"
#include "pwm.h"

struct _motor_t {
	volatile uint8_t *cnt_port;   //motor control output port
	char output_1_pin;
	char output_2_pin;
	motor_mode_t current_mode;
	pwm_args_t *pwm;
};

/**
 * Set the motor speed
 *
 * single signed short from -127 to 127
 */
void motor_set_speed(motor_t *motor, int8_t speed) {
	if (speed == 0) {
		motor_set_mode(motor, MOTOR_MODE_COAST);
		pwm_set_duty(motor->pwm, 0);
	} else if (speed < 0) {
		motor_set_mode(motor, MOTOR_MODE_BACKWARD);
		pwm_set_duty(motor->pwm, -speed*2);
	} else {
		motor_set_mode(motor, MOTOR_MODE_FORWARD);
		pwm_set_duty(motor->pwm, speed*2);
	}
}

/**
 * Set the motor mode
 *
 * mode=coast,forward,backward,brake
 */
void motor_set_mode(motor_t* motor, motor_mode_t mode) {
	if (motor->current_mode != mode) {
		motor->current_mode = mode;

		//set the appropraite bits of the motor control.
		SET_PIN(motor->cnt_port, motor->output_1_pin, (mode & 0b01));
		SET_PIN(motor->cnt_port, motor->output_2_pin, (mode & 0b10));
	}
}

/**
 * Initializes a motor object
 *
 * mode=coast,forward,backward,brake
 */
void motor_init(motor_t **motor, volatile uint8_t *port, char output_1_pin, char output_2_pin, bool pwm_pin) {
	motor_t *tmotor = malloc(sizeof(motor_t));

	tmotor->cnt_port = port;

	tmotor->output_1_pin = output_1_pin;
	tmotor->output_2_pin = output_2_pin;

	*GET_DDR(port) |= (1 << output_1_pin);
	*GET_DDR(port) |= (1 << output_2_pin);

	motor_set_mode(tmotor, MOTOR_MODE_BRAKE);
	pwm_init(&tmotor->pwm, pwm_pin);

	*motor = tmotor;
}

