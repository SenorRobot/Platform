/*
 * pwm.h
 *
 *  Created on: Feb 24, 2012
 *      Author: aah9038
 *      Author: Alex Crawford
 */

#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <stdbool.h>

struct _pwm_args_t;
typedef struct _pwm_args_t pwm_args_t;

void pwm_init(pwm_args_t **pwmin, bool pin);
void pwm_set_duty(pwm_args_t *pwmin, unsigned char compare);

#endif /* PWM_H */

