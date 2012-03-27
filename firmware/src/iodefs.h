/*
 * iodefs.h
 *
 *  Created on: Feb 25, 2012
 *      Author: Andrew LeCain
 */

#include <avr/io.h>

#ifndef IODEFS_H
#define IODEFS_H


int initialize_ports(void);
void setPin(volatile uint8_t* port, char pin, char value);
volatile uint8_t* getDDR(volatile uint8_t* port);
#endif 
