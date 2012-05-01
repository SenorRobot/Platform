#ifndef _SONAR_H_
#define _SONAR_H_

#include <stdint.h>

#define SONAR_COUNT 1

struct _sonar_t;
typedef struct _sonar_t sonar_t;

void sonar_init(sonar_t **sonar);
void sonar_start_readings(sonar_t *sonar);
uint8_t sonar_get_value(sonar_t *sonar, uint8_t index);

#endif

