#include <avr/wdt.h>
#include <avr/power.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/LEDs.h>

#include "descriptors.h"
#include "motor.h"
#include "spi.h"
#include "gyro.h"
#include "sonar.h"

#define WRITE_RATE 10
#define GYRO_READ_RATE 100
#define GYRO_SCALE 1.332  // 1/1000 of a rad
#define GYRO_CAL_Z 25
#define SONAR_CLOSE_THRESHOLD 8
#define SONAR_FAR_THRESHOLD 20

void setup_hardware();
volatile bool send_readings;
volatile int32_t acc_x;
volatile int32_t acc_y;
volatile int32_t acc_z;
bool stopped;

int main() {
	motor_t *motor_l = NULL;
	motor_t *motor_r = NULL;
	sonar_t *sonar = NULL;
	send_readings = false;
	stopped = false;

	setup_hardware();

	motor_init(&motor_l, &PORTF, 6, 7, false);
	motor_init(&motor_r, &PORTD, 2, 3, true);
	sonar_init(&sonar);

	spi_init();
	while (!gyro_init());

	sei();

	acc_x = 0;
	acc_y = 0;
	acc_z = 0;

	sonar_start_readings(sonar);

	while (1) {
		Endpoint_SelectEndpoint(OUT_EPNUM);
		if (Endpoint_IsOUTReceived()) {
			motor_set_speed(motor_l, Endpoint_Read_8());
			motor_set_speed(motor_r, Endpoint_Read_8());
			Endpoint_ClearOUT();
		}
		if (send_readings) {
			send_readings = false;
			if (!stopped)
				LEDs_ToggleLEDs(LEDS_ALL_LEDS);

			Endpoint_SelectEndpoint(IN_EPNUM);

			Endpoint_Write_32_LE(acc_x);
			Endpoint_Write_32_LE(acc_y);
			Endpoint_Write_32_LE(acc_z);

			bool cleared = true;
			for (int i = 0; i < SONAR_COUNT; i++) {
				uint8_t val = sonar_get_value(sonar, i);
				if (val < SONAR_CLOSE_THRESHOLD && !stopped) {
					motor_disable(motor_l);
					motor_disable(motor_r);
					stopped = true;
				}

				if (val < SONAR_FAR_THRESHOLD)
					cleared = false;

				Endpoint_Write_8(val);
			}
			if (stopped && cleared) {
				motor_enable(motor_l);
				motor_enable(motor_r);
				stopped = false;
			}

			Endpoint_ClearIN();
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

	/* Timer Initialization */
	OCR1A  = F_CPU / 1024 / WRITE_RATE;
	OCR3A  = F_CPU / 1024 / GYRO_READ_RATE;
	TCCR1A = 0;
	TCCR3A = 0;
	TCCR1B = (1 << WGM12 |   // CTC
	          1 << COM1A1 |  // Clear on compare match
	          0x05);         // Set the pre-scaler to 1024
	TCCR3B = (1 << WGM12 |   // CTC
	          1 << COM3A1 |  // Clear on compare match
	          0x05);         // Set the pre-scaler to 1024
	TIMSK1 = (1 << OCIE1A);  // Enable interrupt at set point
	TIMSK3 = (1 << OCIE3A);  // Enable interrupt at set point

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}

ISR(TIMER1_COMPA_vect, ISR_BLOCK) {
	send_readings = true;
}

ISR(TIMER3_COMPA_vect, ISR_BLOCK) {
	//acc_x += GYRO_SCALE*gyro_getx()/GYRO_READ_RATE;
	//acc_y += GYRO_SCALE*gyro_gety()/GYRO_READ_RATE;
	acc_z += GYRO_SCALE*(gyro_getz() - GYRO_CAL_Z)/GYRO_READ_RATE;
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged() {
	Endpoint_ConfigureEndpoint(IN_EPNUM, EP_TYPE_INTERRUPT, ENDPOINT_DIR_IN, IN_EPSIZE, ENDPOINT_BANK_SINGLE);
	Endpoint_ConfigureEndpoint(OUT_EPNUM, EP_TYPE_BULK, ENDPOINT_DIR_OUT, OUT_EPSIZE, ENDPOINT_BANK_SINGLE);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest() {}

