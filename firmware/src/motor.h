/*
 *
 *  Generic H-bridge motor controller driver
 *
 * motor.h
 *
 *  Created on: March 14
 *      Author: Andrew LeCain
 *      Modified: Alex Crawford
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "pwm.h"

typedef enum {
	MOTOR_MODE_COAST    = 0b00,
	MOTOR_MODE_FORWARD  = 0b01,
	MOTOR_MODE_BACKWARD = 0b10,
	MOTOR_MODE_BRAKE    = 0b11
} motor_mode_t;

struct _motor_t;
typedef struct _motor_t motor_t;

void motor_set_speed(motor_t *motor, int8_t speed);
void motor_set_mode(motor_t *motor, motor_mode_t mode);
void motor_init(motor_t *motor, volatile uint8_t *port, char output_1_pin, char output_2_pin, bool pwmPin);

#endif /* MOTOR_H_ */
