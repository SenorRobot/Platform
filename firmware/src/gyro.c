#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>


#include "gyro.h"
#include "spi.h"



//write to multiple registers on the device.
int L3G4200D_write(char* buffer, unsigned char address, int length){

	SS_ON;
	unsigned char address_byte=(0<<7); //specify that we are writing	

	if (address < 0x0E || address > 0x38){//check that the address is in range
		return -1;
	} 
	if (length > 1){
		address_byte |= (1<<6); //specify to advance the pointer
	}
	address_byte += address;
	SPI_MasterTransmit(address_byte);

	for (int i=0;i<length;i++){
		SPI_MasterTransmit(buffer[i]);
	}	

	SS_OFF;
	return length;
}

//write to one register on the device
int L3G4200D_writec(unsigned char address, char tx){
	SS_ON;
	unsigned char address_byte=(0<<7); //specify that we are writing	

	if (address < 0x0E || address > 0x38){//check that the address is in range
		SS_OFF;
		return 0;
	} 
	address_byte += address;
	SPI_MasterTransmit(address_byte);

	SPI_MasterTransmit(tx);
	SS_OFF;
	return 1;	

}
//initialize the device
int L3G4200D_init(void ){
	//check the device id
	if (L3G4200D_readc(0x0F) != 0xD3) return 0;

	//initialize device with all axes reporting at 800hz, low-pass @ 35 hz)
	if (!L3G4200D_writec(0x20,0b00011111)) return 0;

	//low pass at 30Hz
	if (!L3G4200D_writec(0x21,0b00000001)) return 0;

	return 1;
}
//read one register from the device
char L3G4200D_readc( unsigned char address){
	SS_ON;
	unsigned char address_byte=(1<<7); //specify that we are reading	

	if (address < 0x0E || address > 0x38){
		return -1;
	} 

	address_byte += address;	
	SPI_MasterTransmit(address_byte);

	char ret= SPI_MasterTransmit(0xFF); //doesn't matter what we transmit
	SS_OFF;
	return ret;

}
//read multiple registers from the device
int L3G4200D_read(char* buffer, unsigned char address, int length){

	SS_ON;

	unsigned char address_byte=(1<<7); //specify that we are reading	

	if (address < 0x0E || address > 0x38){
		return -1;
	} 

	if (length > 1){
		address_byte |= (1<<6); //specify to advance the pointer
	}
	address_byte += address;	
	SPI_MasterTransmit(address_byte);

	for (int i=0;i<length;i++){
		buffer[i]=SPI_MasterTransmit(0x00); //doesn't matter what we transmit
	}	

	SS_OFF;
	return length;

}
//get the x axis count
int L3G4200D_getX(void){
	char buf [2];
	L3G4200D_read(buf, 0x28, 2); //read the two bytes into buf;

	int ret=buf[0];
	ret += buf[1] << 8;

	return ret;


}
//get the y axis count
int L3G4200D_getY(void){

	char buf [2];
	L3G4200D_read(buf, 0x2A, 2); //read the two bytes into buf;

	int ret=buf[0];
	ret += buf[1] << 8;

	return ret;
}

//get the z axis count
int L3G4200D_getZ(void){
	char buf [2];
	L3G4200D_read(buf, 0x2C, 2); //read the two bytes into buf;

	int ret=buf[0];
	ret += buf[1] << 8;

	return ret;
}



