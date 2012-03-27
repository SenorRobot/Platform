/*
 * pwm.h
 *
 *  Created on: Feb 24, 2012
 *      Author: aah9038
 */

#ifndef PWM_H
#define PMW_H

#include <stdint.h>
#include <avr/io.h>
#include "iodefs.h"

typedef struct{
	volatile uint8_t* cnt_port;	//output compare pointer
}pwm_args;

void pwm_init(pwm_args* pwmin, char pin);

void pwm_setDuty(pwm_args* pwmin, unsigned char compare);

#endif /* PWM_H */
