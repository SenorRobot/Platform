#include <avr/wdt.h>
#include <avr/power.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/LEDs.h>

#include "descriptors.h"

#define RECV_MSG_LEN 2


USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
	.Config = {
		.ControlInterfaceNumber         = 0,

		.DataINEndpointNumber           = CDC_TX_EPNUM,
		.DataINEndpointSize             = CDC_TXRX_EPSIZE,
		.DataINEndpointDoubleBank       = false,

		.DataOUTEndpointNumber          = CDC_RX_EPNUM,
		.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
		.DataOUTEndpointDoubleBank      = false,

		.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
		.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
		.NotificationEndpointDoubleBank = false,
	},
};

static FILE USBSerialStream;

unsigned char motor_set_l = 0;
unsigned char motor_set_r = 0;

void SetupHardware(void) {
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();

	/* PWM */
	TCCR0B  = (1 << CS00);
	TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
	OCR0A   = 0;
	TCCR1B  = (1 << CS00);
	TCCR1A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
	OCR1A   = 0;

	/* IO */
	TCCR3A  |= (1 << COM1A0) | (1 << COM1B0);
	DDRB    |= (1 << PB5) | (1 << PB6);

	/* Timer Initialization
	OCR0A  = 100;
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00);
	TIMSK0 = (1 << OCIE0A);*/
}

int main() {
	SetupHardware();

	CDC_Device_CreateBlockingStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	sei();

	while (1) {
		char msg[RECV_MSG_LEN];
		fgets(msg, RECV_MSG_LEN, &USBSerialStream);

		motor_set_l = msg[0];
		motor_set_r = msg[1];

		OCR0A = motor_set_l;
		OCR1A = motor_set_r;

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

