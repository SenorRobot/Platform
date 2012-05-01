#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "sonar.h"

#define SONAR_READ_RATE 20

struct _sonar_t {
	uint8_t readings[SONAR_COUNT];
};

sonar_t *volatile cur_sonar;
volatile uint8_t cur_index;

static const int muxmap[SONAR_COUNT] = {0x00};//, 0x01, 0x04, 0x05, 0x06};

void sonar_init(sonar_t **sonar) {
	*sonar = malloc(sizeof(sonar_t));

	ADCSRA = (1 << ADEN  |   // enable the ADC
	          1 << ADPS0 |   // set the prescaler to 128
	          1 << ADPS1 |
	          1 << ADPS2 |
	          1 << ADATE |
	          1 << ADIE);    // enable interrupts
}

void sonar_start_readings(sonar_t *sonar) {
	cur_sonar = sonar;
	cur_index = 0;

	ADMUX = (1 << REFS0 |          // use Vcc for Vref
	         muxmap[cur_index]);   // select the correct input pin

	OCR4A  = F_CPU / 1024 / SONAR_READ_RATE / SONAR_COUNT;
	TCCR4A = 0;
	TCCR4B = (1 << WGM12 |         // CTC
	          1 << COM3A1 |        // Clear on compare match
	          0x05);               // Set the pre-scaler to 1024

	ADCSRB = (1 << ADTS3 |   // set the auto trigger to timer 4 compare A
	          1 << ADTS0);
	//ADCSRA |= (1 << ADSC);         // start the conversion
}

uint8_t sonar_get_value(sonar_t *sonar, uint8_t index) {
	return sonar->readings[index];
}

ISR(ADC_vect, ISR_BLOCK) {
	uint16_t val = (uint16_t)ADCL | ((uint16_t)ADCH << 8);
	cur_sonar->readings[cur_index] = val >> 1;

	cur_index = (cur_index + 1) % SONAR_COUNT;
	ADMUX = (1 << REFS0 |
	         muxmap[cur_index]);   // select the correct input pin
	ADCSRA |= (1 << ADSC);         // start the conversion
}

