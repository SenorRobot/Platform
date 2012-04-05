/*
 * Gyro.h
 *
 * Author: Andrew LeCain
 * Modified: Alex Crawford
 *
 * Teensy 2.0 driver for gyro 3 axis accelerometer board
 *
 */

#ifndef GYRO_H
#define GYRO_H

#include <stdbool.h>

bool gyro_init();
int gyro_getx();
int gyro_gety();
int gyro_getz();

#endif
