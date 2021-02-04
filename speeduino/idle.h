#ifndef IDLE_H
#define IDLE_H

#include "globals.h"
#include "table.h"
#include BOARD_H //Note that this is not a real file, it is defined in globals.h. 

#define IAC_ALGORITHM_NONE    0
#define IAC_ALGORITHM_ONOFF   1
#define IAC_ALGORITHM_PWM_OL  2
#define IAC_ALGORITHM_PWM_CL  3
#define IAC_ALGORITHM_STEP_OL 4
#define IAC_ALGORITHM_STEP_CL 5
#define IAC_ALGORITHM_PWM_OLCL  6 //Openloop plus closedloop IAC control

#define STEPPER_FORWARD 0
#define STEPPER_BACKWARD 1
#define IDLE_TABLE_SIZE 10

enum StepperStatus {SOFF, STEPPING, COOLING}; //The 2 statuses that a stepper can have. STEPPING means that a high pulse is currently being sent and will need to be turned off at some point.

struct StepperIdle
{
  int curIdleStep; //Tracks the current location of the stepper
  int targetIdleStep; //What the targetted step is
  volatile StepperStatus stepperStatus;
  volatile unsigned long stepStartTime; //The time the curren
  byte lessAirDirection;
  byte moreAirDirection;
};

extern table2D iacClosedLoopTable;
extern table2D iacPWMTable;
extern table2D iacStepTable;
//Open loop tables specifically for cranking
extern table2D iacCrankStepsTable;
extern table2D iacCrankDutyTable;

extern StepperIdle idleStepper;
extern bool idleOn; //Simply tracks whether idle was on last time around
extern byte idleInitComplete; //TRacks which idle method was initialised. 99 is a method that will never exist
extern unsigned int iacStepTime_uS;
extern unsigned int iacCoolTime_uS;
extern unsigned int completedHomeSteps;

extern volatile PORT_TYPE *idle_pin_port;
extern volatile PINMASK_TYPE idle_pin_mask;
extern volatile PORT_TYPE *idle2_pin_port;
extern volatile PINMASK_TYPE idle2_pin_mask;
extern volatile PORT_TYPE *idleUpOutput_pin_port;
extern volatile PINMASK_TYPE idleUpOutput_pin_mask;

extern volatile bool idle_pwm_state;
extern unsigned int idle_pwm_max_count; //Used for variable PWM frequency
extern volatile unsigned int idle_pwm_cur_value;
extern long idle_pid_target_value;
extern long FeedForwardTerm;
extern unsigned long idle_pwm_target_value;
extern long idle_cl_target_rpm;
extern byte idleCounter; //Used for tracking the number of calls to the idle control function

extern byte idleUpOutputHIGH; // Used to invert the idle Up Output 
extern byte idleUpOutputLOW;   // Used to invert the idle Up Output 

void initialiseIdle();
void initialiseIdleUpOutput();
void disableIdle();
void enableIdle();
byte isStepperHomed();
byte checkForStepping();
void doStep();
void idleInterrupt();

#endif
