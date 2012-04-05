#include <avr/wdt.h>
#include <avr/power.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/LEDs.h>

#include "descriptors.h"

#include "motor.h"

void setup_hardware();

int main() {
	motor_t *motor_l = NULL;
	motor_t *motor_r = NULL;

	setup_hardware();

	motor_init(motor_l, &PORTF, 6, 7, false);
	motor_init(motor_r, &PORTD, 2, 3, true);

	sei();

	while (1) {
		Endpoint_SelectEndpoint(OUT_EPNUM);
		if (Endpoint_IsOUTReceived()) {
			motor_set_speed(motor_l, Endpoint_Read_8());
			motor_set_speed(motor_r, Endpoint_Read_8());
			Endpoint_ClearOUT();
		}
		USB_USBTask();
	}
}

void setup_hardware() {
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged() {
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN, IN_EPSIZE, ENDPOINT_BANK_SINGLE);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT, OUT_EPSIZE, ENDPOINT_BANK_SINGLE);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest() {}

