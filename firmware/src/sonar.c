#include <stdlib.h>
#include "sonar.h"

struct _sonar_t {
	uint8_t readings[SONAR_COUNT];
};

static const muxmap[SONAR_COUNT] = {0x00, 0x01, 0x04, 0x05, 0x06};

void sonar_init(sonar_t **sonar) {
	*sonar = malloc(sizeof(sonar_t));

	ADCSRA = (1 << ADEN |      // enable the ADC
	          7 << ADPS0);     // set the prescaler
}

void take_readings(sonar_t *sonar) {
	for (int i = 0; i < SONAR_COUNT; i++) {
		ADMUX = muxmap[i];     // select the correct input pin
		ADMUX |= (1 << ADSC);  // start the conversion

		while (ADMUX & ADSC);  // wait for the conversion to complete

		sonar->readings[i] = ADCL;
	}
}

uint8_t get_sonar(sonar_t *sonar, uint8_t index) {
	return sonar->readings[index];
}

