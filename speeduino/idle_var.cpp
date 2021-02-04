#include "idle.h"

table2D iacClosedLoopTable;
table2D iacPWMTable;
table2D iacStepTable;
//Open loop tables specifically for cranking
table2D iacCrankStepsTable;
table2D iacCrankDutyTable;

StepperIdle idleStepper;
bool idleOn; //Simply tracks whether idle was on last time around
byte idleInitComplete = 99; //TRacks which idle method was initialised. 99 is a method that will never exist
unsigned int iacStepTime_uS;
unsigned int iacCoolTime_uS;
unsigned int completedHomeSteps;

volatile PORT_TYPE *idle_pin_port;
volatile PINMASK_TYPE idle_pin_mask;
volatile PORT_TYPE *idle2_pin_port;
volatile PINMASK_TYPE idle2_pin_mask;
volatile PORT_TYPE *idleUpOutput_pin_port;
volatile PINMASK_TYPE idleUpOutput_pin_mask;

volatile bool idle_pwm_state;
unsigned int idle_pwm_max_count; //Used for variable PWM frequency
volatile unsigned int idle_pwm_cur_value;
long idle_pid_target_value;
long FeedForwardTerm;
unsigned long idle_pwm_target_value;
long idle_cl_target_rpm;
byte idleCounter; //Used for tracking the number of calls to the idle control function

byte idleUpOutputHIGH = HIGH; // Used to invert the idle Up Output 
byte idleUpOutputLOW = LOW;   // Used to invert the idle Up Output 
