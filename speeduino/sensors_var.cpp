#include "sensors.h"

volatile byte flexCounter = 0;
volatile unsigned long flexStartTime;
volatile unsigned long flexPulseWidth;

volatile byte knockCounter = 0;
volatile uint16_t knockAngle;

unsigned long MAPrunningValue; //Used for tracking either the total of all MAP readings in this cycle (Event average) or the lowest value detected in this cycle (event minimum)
unsigned long EMAPrunningValue; //As above but for EMAP
unsigned int MAPcount; //Number of samples taken in the current MAP cycle
uint32_t MAPcurRev; //Tracks which revolution we're sampling on
bool auxIsEnabled;
byte TPSlast; /**< The previous TPS reading */
unsigned long TPS_time; //The time the TPS sample was taken
unsigned long TPSlast_time; //The time the previous TPS sample was taken
byte MAPlast; /**< The previous MAP reading */
unsigned long MAP_time; //The time the MAP sample was taken
unsigned long MAPlast_time; //The time the previous MAP sample was taken
volatile unsigned long vssLastPulseTime; /**< The time of the last VSS pulse of the VSS */
volatile unsigned long vssLastMinusOnePulseTime; /**< The time of the last VSS_NUM_SAMPLES pulses of the VSS are stored in this array */
volatile unsigned long vssTotalTime; /**< Cumulative count of the last VSS_SAMPLES number of pulses */
volatile byte vssCount;


//These variables are used for tracking the number of running sensors values that appear to be errors. Once a threshold is reached, the sensor reading will go to default value and assume the sensor is faulty
byte mapErrorCount = 0;
byte iatErrorCount = 0;
byte cltErrorCount = 0;
