#ifdef _SONAR_H_
#define _SONAR_H_

#define SONAR_COUNT 5

struct _sonar_t;
typedef _sonar_t sonar_t;

void sonar_init(sonar_t **sonar);
void take_readings(sonar_t *sonar);
uint8_t get_sonar(sonar_t *sonar, uint8_t index);

#endif

