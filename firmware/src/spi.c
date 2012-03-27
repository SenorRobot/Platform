/**
 * AVR spi library
 *
 * @author Andrew LeCain
 */


#include <avr/io.h>
#include "spi.h"


void SPI_Init(void)
{
	UBRR1 = 0;
	// Setting the XCKn port pin as output, enables master mode. B1
	DDRB |= (1<<MOSI) | (1<<SCK);
	DDRC |= (1<<SS);
	
	DDRB &= ~(2<<MISO); //miso as input
	SS_OFF;

	SPCR = (1 <<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPR1);

	PRR0 &= ~(1<<PRSPI); //turn off low power mode

}

char SPI_MasterTransmit(volatile char cData){
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
