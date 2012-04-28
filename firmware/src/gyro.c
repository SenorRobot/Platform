#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>

#include "gyro.h"
#include "spi.h"

#define LOW_ADDRESS   0x0E
#define HIGH_ADDRESS  0x38
#define FLAG_WRITE   (0 << 7)
#define FLAG_READ    (1 << 7)
#define FLAG_INC_PTR (1 << 6)

bool gyro_write(char* buffer, unsigned char address, int length);
bool gyro_writec(unsigned char address, char tx);
char gyro_readc(unsigned char address);
bool gyro_read(char* buffer, unsigned char address, int length);

//write to multiple registers on the device.
bool gyro_write(char* buffer, unsigned char address, int length) {
	if (address < LOW_ADDRESS || address > HIGH_ADDRESS)   //check that the address is in range
		return false;

	SS_ON;

	spi_master_transmit(address | FLAG_WRITE | FLAG_INC_PTR);
	for (int i = 0; i < length; i++)
		spi_master_transmit(buffer[i]);

	SS_OFF;

	return true;
}

//write to one register on the device
bool gyro_writec(unsigned char address, char tx) {
	if (address < LOW_ADDRESS || address > HIGH_ADDRESS)   //check that the address is in range
		return false;

	SS_ON;

	spi_master_transmit(address | FLAG_WRITE);
	spi_master_transmit(tx);

	SS_OFF;

	return true;
}

//initialize the device
bool gyro_init() {
	//check the device id
	if (gyro_readc(0x0F) != 0xD3)
		return false;

	//initialize device with z axis reporting at 100hz, low-pass @ 25 hz)
	if (!gyro_writec(0x20, 0b00111100))
		return false;

	//high pass at 4Hz
	if (!gyro_writec(0x21, 0b00000100))
		return false;

	return true;
}
//read one register from the device
char gyro_readc(unsigned char address) {
	if (address < LOW_ADDRESS || address > HIGH_ADDRESS)
		return -1;

	SS_ON;

	spi_master_transmit(address | FLAG_READ);
	char ret = spi_master_transmit(0x00);      //doesn't matter what we transmit

	SS_OFF;

	return ret;
}

//read multiple registers from the device
bool gyro_read(char* buffer, unsigned char address, int length) {
	if (address < LOW_ADDRESS || address > HIGH_ADDRESS)
		return false;

	SS_ON;

	spi_master_transmit(address | FLAG_READ | FLAG_INC_PTR);
	for (int i = 0; i < length; i++)
		buffer[i] = spi_master_transmit(0x00);   //doesn't matter what we transmit

	SS_OFF;

	return true;
}

//get the x axis count
int gyro_getx() {
	char buf[2];
	gyro_read(buf, 0x28, 2);              //read the two bytes into buf;

	return (buf[0] | buf[1] << 8);
}

//get the y axis count
int gyro_gety() {
	char buf[2];
	gyro_read(buf, 0x2A, 2);              //read the two bytes into buf;

	return (buf[0] | buf[1] << 8);
}

//get the z axis count
int gyro_getz() {
	char buf[2];
	gyro_read(buf, 0x2C, 2);              //read the two bytes into buf;

	return (buf[0] | buf[1] << 8);
}

