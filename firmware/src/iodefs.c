/*
 * Implementation of common IO tasks
 *
 * iodefs.c
 *
 *  Created on: March 1, 2012
 *      Author: Andrew LeCain
 */

#include <avr/io.h>
#include "iodefs.h"


inline void setPin(volatile uint8_t* port, char pin, char value){
	
	if(value){
		*port |= (char)(1<<pin);
	}else{
		*port &= (char)~(1<<pin);
	}
}


inline volatile uint8_t* getDDR(volatile uint8_t* port){

	if (port == &PORTB)
		return &DDRB;
	if (port == &PORTC)
		return &DDRC;
	if (port == &PORTD)
		return &DDRD;

	return 0;

}


/* ******************************************************************
 * Initiaize register handles
 *****************************************************************/

int initialize_ports(){
	return 1;

}
