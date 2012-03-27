#include <avr/wdt.h>
#include <avr/power.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/LEDs.h>

#include "descriptors.h"

#include "motor.h"

unsigned char motor_set_l = 0;
unsigned char motor_set_r = 0;

void SetupHardware(void) {
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
	
	/* PWM */
	/*
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B = (1 << CS01);
	OCR0A  = motor_set_l;
	OCR0B  = motor_set_r;
	*/
	/* IO */
	/*
	DDRB |= (1 << PB7);
	DDRD |= (1 << PD0);*/

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}

int main() {
	SetupHardware();
	motor_t motorL;	
	motor_t motorR;

	init_motor(&motorL, &PORTF, 6, 7, 0); //pwm0 
	init_motor(&motorR, &PORTD, 2, 3, 1); //pwm1

	sei();

	while (1) {
		Endpoint_SelectEndpoint(OUT_EPNUM);
		if (Endpoint_IsOUTReceived()) {
			motor_setSpeed(&motorL, Endpoint_Read_8());
			motor_setSpeed(&motorR, Endpoint_Read_8());
			//OCR0A = Endpoint_Read_8();
			//OCR0B = Endpoint_Read_8();
			Endpoint_ClearOUT();
		}

		USB_USBTask();
	}
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN, IN_EPSIZE, ENDPOINT_BANK_SINGLE);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT, OUT_EPSIZE, ENDPOINT_BANK_SINGLE);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
}

