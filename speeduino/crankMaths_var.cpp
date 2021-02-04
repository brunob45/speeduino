#include "crankMaths.h"

volatile uint16_t timePerDegree;
volatile uint16_t timePerDegreex16;
volatile uint16_t degreesPeruSx2048;
volatile unsigned long degreesPeruSx32768;

//These are only part of the experimental 2nd deriv calcs
byte deltaToothCount = 0; //The last tooth that was used with the deltaV calc
int rpmDelta;
