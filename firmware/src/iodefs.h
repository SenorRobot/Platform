/*
 * iodefs.h
 *
 *  Created on: Feb 25, 2012
 *      Author: Andrew LeCain
 *      Modified: Alex Crawford
 */

#ifndef IODEFS_H
#define IODEFS_H

#include <stdint.h>
#include <avr/io.h>

static inline void SET_PIN(volatile uint8_t *port, char pin, char value) {
	if (value)
		*port |= (char)(1 << pin);
	else
		*port &= ~(char)(1 << pin);
}

static inline volatile uint8_t *GET_DDR(volatile uint8_t *port) {
	if (port == &PORTB)
		return &DDRB;
	if (port == &PORTC)
		return &DDRC;
	if (port == &PORTD)
		return &DDRD;

	return 0;
}

#endif
