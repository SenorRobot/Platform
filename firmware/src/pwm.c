/*
** pwm.c
**
**	atmega PWM driver
**
**	Author: Andrew LeCain
*/



#include <avr/io.h>
#include "pwm.h"

void pwm_init(pwm_args* pwmin, char pin){

	if (pin > 1) return;

	TCCR0A |= (1<<WGM00) | (1<<WGM01); // enable fast PWM mode
	TCCR0B = (1<< CS00); 	// no clcok divider

	if (pin){ //if pin is 1, then use OC0B

		DDRD |= (1<<0);
		TCCR0A |= (1 << COM0B1); //set to clear OC0B
		OCR0B = 0;
		pwmin->cnt_port = &OCR0B;
		
	}
	else{ //OC0A if pin is 0, use OC0A
		DDRB |= (1<<7);
		TCCR0A |= (1 << COM0A1); //set to clear OC0A
		OCR0A = 0;
		pwmin->cnt_port= &OCR0A;
	}
	
}

void pwm_setDuty(pwm_args* pwmin, unsigned char compare){
	*(pwmin->cnt_port)=compare;
}

