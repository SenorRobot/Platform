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

struct _motor_t;
typedef struct _motor_t motor_t;

void motor_set_speed(motor_t *motor, int8_t speed);
void motor_init(motor_t **motor, volatile uint8_t *port, char output_1_pin, char output_2_pin, bool pwmPin);
void motor_disable(motor_t *motor);
void motor_enable(motor_t *motor);

#endif /* MOTOR_H_ */
