

/*
 *Gyro.h
 *
 *Author: Andrew LeCain
 *
 *Teensy 2.0 driver for L3G4200D 3 axis accelerometer board
 *
 */

#ifndef GYRO_H
#define GYRO_H



//Helper functions to read and write to registers on the device
int L3G4200D_write(char* buffer, unsigned char address, int length);
int L3G4200D_writec(unsigned char address, char tx);
int L3G4200D_init(void );
char L3G4200D_readc( unsigned char address);
int L3G4200D_read(char* buffer, unsigned char address, int length);

//Functions to return the current count on each of these drivers
int L3G4200D_getX(void);
int L3G4200D_getY(void);
int L3G4200D_getZ(void);


#endif
