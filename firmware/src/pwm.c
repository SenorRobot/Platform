/*
** pwm.c
**
**  atmega PWM driver
**
**  Author: Andrew LeCain
**  Author: Alex Crawford
*/

#include <avr/io.h>
#include "pwm.h"
#include "iodefs.h"

struct _pwm_args_t {
	volatile uint8_t *cnt_port;              //output compare pointer
};

void pwm_init(pwm_args_t *pwmin, bool pin) {
	TCCR0A |= (1 << WGM00) | (1 << WGM01);   // enable fast PWM mode
	TCCR0B  = (1<< CS00);                    // no clcok divider

	if (pin) {                               //if pin is 1, then use OC0B
		DDRD   |= (1 << 0);
		TCCR0A |= (1 << COM0B1);             //set to clear OC0B
		OCR0B   = 0;
		pwmin->cnt_port = &OCR0B;
	} else{                                  //OC0A if pin is 0, use OC0A
		DDRB   |= (1 << 7);
		TCCR0A |= (1 << COM0A1);             //set to clear OC0A
		OCR0A   = 0;
		pwmin->cnt_port = &OCR0A;
	}
}

void pwm_set_duty(pwm_args_t *pwmin, unsigned char compare) {
	*(pwmin->cnt_port) = compare;
}

