#ifndef SPI_H
#define SPI_H

#ifndef SS

#include <avr/io.h>

//PORT D
#define SS_PORT PORTB
#define SS    0

//PORT B
#define MISO  3
#define MOSI  2
#define SCK   1

#endif

#define SS_ON  (SS_PORT &= ~(1 << SS))
#define SS_OFF (SS_PORT |=  (1 << SS))


//SPI helper functions. Sets up registers and does that communication
void spi_init();
char spi_master_transmit(volatile char data);

#endif

