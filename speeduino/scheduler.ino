/*
Speeduino - Simple engine management for the Arduino Mega 2560 platform
Copyright (C) Josh Stewart
A full copy of the license may be found in the projects root directory
*/

#include "globals.h"
#include "scheduler.h"
#include "scheduledIO.h"


void initialiseSchedulers()
{
    nullSchedule.Status = OFF;

    fuelSchedule1.Status = OFF;
    fuelSchedule2.Status = OFF;
    fuelSchedule3.Status = OFF;
    fuelSchedule4.Status = OFF;
    fuelSchedule5.Status = OFF;
    fuelSchedule6.Status = OFF;
    fuelSchedule7.Status = OFF;
    fuelSchedule8.Status = OFF;

    fuelSchedule1.schedulesSet = 0;
    fuelSchedule2.schedulesSet = 0;
    fuelSchedule3.schedulesSet = 0;
    fuelSchedule4.schedulesSet = 0;
    fuelSchedule5.schedulesSet = 0;
    fuelSchedule6.schedulesSet = 0;
    fuelSchedule7.schedulesSet = 0;
    fuelSchedule8.schedulesSet = 0;

    fuelSchedule1.counter = &FUEL1_COUNTER;
    fuelSchedule1.compare = &FUEL1_COMPARE;
    fuelSchedule2.counter = &FUEL2_COUNTER;
    fuelSchedule2.compare = &FUEL2_COMPARE;
    fuelSchedule3.counter = &FUEL3_COUNTER;
    fuelSchedule3.compare = &FUEL3_COMPARE;
    fuelSchedule4.counter = &FUEL4_COUNTER;
    fuelSchedule4.compare = &FUEL4_COMPARE;
    #if (INJ_CHANNELS >= 5)
    fuelSchedule5.counter = &FUEL5_COUNTER;
    fuelSchedule5.compare = &FUEL5_COMPARE;
    #endif
    #if (INJ_CHANNELS >= 6)
    fuelSchedule6.counter = &FUEL6_COUNTER;
    fuelSchedule6.compare = &FUEL6_COMPARE;
    #endif
    #if (INJ_CHANNELS >= 7)
    fuelSchedule7.counter = &FUEL7_COUNTER;
    fuelSchedule7.compare = &FUEL7_COMPARE;
    #endif
    #if (INJ_CHANNELS >= 8)
    fuelSchedule8.counter = &FUEL8_COUNTER;
    fuelSchedule8.compare = &FUEL8_COMPARE;
    #endif

    ignitionSchedule1.Status = OFF;
    ignitionSchedule2.Status = OFF;
    ignitionSchedule3.Status = OFF;
    ignitionSchedule4.Status = OFF;
    ignitionSchedule5.Status = OFF;
    ignitionSchedule6.Status = OFF;
    ignitionSchedule7.Status = OFF;
    ignitionSchedule8.Status = OFF;

    IGN1_TIMER_ENABLE();
    IGN2_TIMER_ENABLE();
    IGN3_TIMER_ENABLE();
    IGN4_TIMER_ENABLE();

    ignitionSchedule1.schedulesSet = 0;
    ignitionSchedule2.schedulesSet = 0;
    ignitionSchedule3.schedulesSet = 0;
    ignitionSchedule4.schedulesSet = 0;
    ignitionSchedule5.schedulesSet = 0;
    ignitionSchedule6.schedulesSet = 0;
    ignitionSchedule7.schedulesSet = 0;
    ignitionSchedule8.schedulesSet = 0;

    ignitionSchedule1.counter = &IGN1_COUNTER;
    ignitionSchedule1.compare = &IGN1_COMPARE;
    ignitionSchedule2.counter = &IGN2_COUNTER;
    ignitionSchedule2.compare = &IGN2_COMPARE;
    ignitionSchedule3.counter = &IGN3_COUNTER;
    ignitionSchedule3.compare = &IGN3_COMPARE;
    ignitionSchedule4.counter = &IGN4_COUNTER;
    ignitionSchedule4.compare = &IGN4_COMPARE;
    #if (INJ_CHANNELS >= 5)
    ignitionSchedule5.counter = &IGN5_COUNTER;
    ignitionSchedule5.compare = &IGN5_COMPARE;
    #endif
    #if (INJ_CHANNELS >= 6)
    ignitionSchedule6.counter = &IGN6_COUNTER;
    ignitionSchedule6.compare = &IGN6_COMPARE;
    #endif
    #if (INJ_CHANNELS >= 7)
    ignitionSchedule7.counter = &IGN7_COUNTER;
    ignitionSchedule7.compare = &IGN7_COMPARE;
    #endif
    #if (INJ_CHANNELS >= 8)
    ignitionSchedule8.counter = &IGN8_COUNTER;
    ignitionSchedule8.compare = &IGN8_COMPARE;
    #endif

}

/*
These 8 function turn a schedule on, provides the time to start and the duration and gives it callback functions.
All 8 functions operate the same, just on different schedules
Args:
startCallback: The function to be called once the timeout is reached
timeout: The number of uS in the future that the startCallback should be triggered
duration: The number of uS after startCallback is called before endCallback is called
endCallback: This function is called once the duration time has been reached
*/

#define DECLARE_SET_FUEL_SCHEDULE(n)\
void setFuelSchedule##n(unsigned long timeout, unsigned long duration){\
  /*Check whether timeout exceeds the maximum future time. This can potentially occur on sequential setups when below ~115rpm*/\
  if(timeout < MAX_TIMER_PERIOD){\
    /*The following must be enclosed in the noInterupts block to avoid contention caused if the relevant interrupt fires before the state is fully set*/\
    noInterrupts();\
    fuelSchedule##n.duration = duration;\
    if(fuelSchedule##n.Status != RUNNING){ /*Check that we're not already part way through a schedule*/\
      fuelSchedule##n.startCompare = FUEL##n##_COUNTER + uS_TO_TIMER_COMPARE(timeout);\
      fuelSchedule##n.endCompare = fuelSchedule##n.startCompare + uS_TO_TIMER_COMPARE(duration);\
      fuelSchedule##n.Status = PENDING; /*Turn this schedule on*/\
      fuelSchedule##n.schedulesSet++; /*Increment the number of times this schedule has been set*/\
      FUEL##n##_COMPARE = fuelSchedule##n.startCompare;\
      interrupts();\
      FUEL##n##_TIMER_ENABLE();\
    } else {\
      /*If the schedule is already running, we can set the next schedule so it is ready to go\
       *This is required in cases of high rpm and high DC where there otherwise would not be enough time to set the schedule*/\
      fuelSchedule##n.nextStartCompare = FUEL##n##_COUNTER + uS_TO_TIMER_COMPARE(timeout);\
      fuelSchedule##n.nextEndCompare = fuelSchedule##n.nextStartCompare + uS_TO_TIMER_COMPARE(duration);\
      fuelSchedule##n.hasNextSchedule = true;\
      interrupts();\
    } /*Schedule is RUNNING*/\
  } /*Timeout less than threshold*/\
}

DECLARE_SET_FUEL_SCHEDULE(1);
DECLARE_SET_FUEL_SCHEDULE(2);
DECLARE_SET_FUEL_SCHEDULE(3);
DECLARE_SET_FUEL_SCHEDULE(4);
DECLARE_SET_FUEL_SCHEDULE(5);
DECLARE_SET_FUEL_SCHEDULE(6);
DECLARE_SET_FUEL_SCHEDULE(7);
DECLARE_SET_FUEL_SCHEDULE(8);

#define DECLARE_SET_IGN_SCHEDULE(n)\
void setIgnitionSchedule##n(void (*startCallback)(), unsigned long timeout, unsigned long duration, void(*endCallback)()){\
  /*Check whether timeout exceeds the maximum future time. This can potentially occur on sequential setups when below ~115rpm*/\
  if(timeout < MAX_TIMER_PERIOD){\
    /*The following must be enclosed in the noInterupts block to avoid contention caused if the relevant interrupt fires before the state is fully set*/\
    noInterrupts();\
    ignitionSchedule##n.duration = duration;\
    if(ignitionSchedule##n.Status != RUNNING){ /*Check that we're not already part way through a schedule*/\
      ignitionSchedule##n.StartCallback = startCallback; /*Name the start callback function*/\
      ignitionSchedule##n.EndCallback = endCallback; /*Name the end callback function*/\
      ignitionSchedule##n.startCompare = IGN##n##_COUNTER + uS_TO_TIMER_COMPARE(timeout);\
      if(ignitionSchedule##n.endScheduleSetByDecoder == false) {\
        /*The .endCompare value is also set by the per tooth timing in decoders.ino. The check here is so that it's not getting overridden.*/\
        ignitionSchedule##n.endCompare = ignitionSchedule##n.startCompare + uS_TO_TIMER_COMPARE(duration);\
      }\
      ignitionSchedule##n.Status = PENDING; /*Turn this schedule on*/\
      ignitionSchedule##n.schedulesSet++;\
      IGN##n##_COMPARE = ignitionSchedule##n.startCompare;\
      interrupts();\
      IGN##n##_TIMER_ENABLE();\
    } else {\
      /*If the schedule is already running, we can set the next schedule so it is ready to go\
       *This is required in cases of high rpm and high DC where there otherwise would not be enough time to set the schedule*/\
      ignitionSchedule##n.nextStartCompare = IGN##n##_COUNTER + uS_TO_TIMER_COMPARE(timeout);\
      ignitionSchedule##n.nextEndCompare = ignitionSchedule##n.nextStartCompare + uS_TO_TIMER_COMPARE(duration);\
      ignitionSchedule##n.hasNextSchedule = true;\
      interrupts();\
    } /*Schedule is RUNNING*/\
  } /*Timeout less than threshold*/\
}

DECLARE_SET_IGN_SCHEDULE(1);
DECLARE_SET_IGN_SCHEDULE(2);
DECLARE_SET_IGN_SCHEDULE(3);
DECLARE_SET_IGN_SCHEDULE(4);
DECLARE_SET_IGN_SCHEDULE(5);
DECLARE_SET_IGN_SCHEDULE(6);
DECLARE_SET_IGN_SCHEDULE(7);
DECLARE_SET_IGN_SCHEDULE(8);

static inline void refreshIgnitionSchedule1(unsigned long timeToEnd)
{
  if( (ignitionSchedule1.Status == RUNNING) && (timeToEnd < ignitionSchedule1.duration) )
  //Must have the threshold check here otherwise it can cause a condition where the compare fires twice, once after the other, both for the end
  //if( (timeToEnd < ignitionSchedule1.duration) && (timeToEnd > IGNITION_REFRESH_THRESHOLD) )
  {
    noInterrupts();
    ignitionSchedule1.endCompare = IGN1_COUNTER + uS_TO_TIMER_COMPARE(timeToEnd);
    IGN1_COMPARE = ignitionSchedule1.endCompare;
    interrupts();
  }
}

/*******************************************************************************************************************************************************************************************************/
//This function (All 8 ISR functions that are below) gets called when either the start time or the duration time are reached
//This calls the relevant callback function (startCallback or endCallback) depending on the status of the schedule.
//If the startCallback function is called, we put the scheduler into RUNNING state
//Timer3A (fuel schedule 1) Compare Vector
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) //AVR chips use the ISR for this
ISR(TIMER3_COMPA_vect) //fuelSchedules 1 and 5
#else
static inline void fuelSchedule1Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (fuelSchedule1.Status == PENDING) //Check to see if this schedule is turn on
    {
      //To use timer queue, change fuelShedule1 to timer3Aqueue[0];
      if (configPage2.injLayout == INJ_SEMISEQUENTIAL) { openInjector1and4(); }
      else { openInjector1(); }
      fuelSchedule1.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      FUEL1_COMPARE = FUEL1_COUNTER + uS_TO_TIMER_COMPARE(fuelSchedule1.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (fuelSchedule1.Status == RUNNING)
    {
       //timer3Aqueue[0]->EndCallback();
       if (configPage2.injLayout == INJ_SEMISEQUENTIAL) { closeInjector1and4(); }
       else { closeInjector1(); }
       fuelSchedule1.Status = OFF; //Turn off the schedule
       fuelSchedule1.schedulesSet = 0;
       //FUEL1_COMPARE = fuelSchedule1.endCompare;

       //If there is a next schedule queued up, activate it
       if(fuelSchedule1.hasNextSchedule == true)
       {
         FUEL1_COMPARE = fuelSchedule1.nextStartCompare;
         fuelSchedule1.endCompare = fuelSchedule1.nextEndCompare;
         fuelSchedule1.Status = PENDING;
         fuelSchedule1.schedulesSet = 1;
         fuelSchedule1.hasNextSchedule = false;
       }
       else { FUEL1_TIMER_DISABLE(); }
    }
    else if (fuelSchedule1.Status == OFF) { FUEL1_TIMER_DISABLE(); } //Safety check. Turn off this output compare unit and return without performing any action
  }

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) //AVR chips use the ISR for this
ISR(TIMER3_COMPB_vect) //fuelSchedule2
#else
static inline void fuelSchedule2Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (fuelSchedule2.Status == PENDING) //Check to see if this schedule is turn on
    {
      //fuelSchedule2.StartCallback();
      if (configPage2.injLayout == INJ_SEMISEQUENTIAL) { openInjector2and3(); }
      else { openInjector2(); }
      fuelSchedule2.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      FUEL2_COMPARE = FUEL2_COUNTER + uS_TO_TIMER_COMPARE(fuelSchedule2.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (fuelSchedule2.Status == RUNNING)
    {
       //fuelSchedule2.EndCallback();
       if (configPage2.injLayout == INJ_SEMISEQUENTIAL) { closeInjector2and3(); }
       else { closeInjector2(); }
       fuelSchedule2.Status = OFF; //Turn off the schedule
       fuelSchedule2.schedulesSet = 0;

       //If there is a next schedule queued up, activate it
       if(fuelSchedule2.hasNextSchedule == true)
       {
         FUEL2_COMPARE = fuelSchedule2.nextStartCompare;
         fuelSchedule2.endCompare = fuelSchedule2.nextEndCompare;
         fuelSchedule2.Status = PENDING;
         fuelSchedule2.schedulesSet = 1;
         fuelSchedule2.hasNextSchedule = false;
       }
       else { FUEL2_TIMER_DISABLE(); }
    }
  }

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) //AVR chips use the ISR for this
ISR(TIMER3_COMPC_vect) //fuelSchedule3
#else
static inline void fuelSchedule3Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (fuelSchedule3.Status == PENDING) //Check to see if this schedule is turn on
    {
      //fuelSchedule3.StartCallback();
      //Hack for 5 cylinder
      if(channel5InjEnabled) { openInjector3and5(); }
      else { openInjector3(); }
      fuelSchedule3.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      FUEL3_COMPARE = FUEL3_COUNTER + uS_TO_TIMER_COMPARE(fuelSchedule3.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (fuelSchedule3.Status == RUNNING)
    {
       //fuelSchedule3.EndCallback();
       //Hack for 5 cylinder
       if(channel5InjEnabled) { closeInjector3and5(); }
       else { closeInjector3and5(); }
       fuelSchedule3.Status = OFF; //Turn off the schedule
       fuelSchedule3.schedulesSet = 0;

       //If there is a next schedule queued up, activate it
       if(fuelSchedule3.hasNextSchedule == true)
       {
         FUEL3_COMPARE = fuelSchedule3.nextStartCompare;
         fuelSchedule3.endCompare = fuelSchedule3.nextEndCompare;
         fuelSchedule3.Status = PENDING;
         fuelSchedule3.schedulesSet = 1;
         fuelSchedule3.hasNextSchedule = false;
       }
       else { FUEL3_TIMER_DISABLE(); }
    }
  }

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) //AVR chips use the ISR for this
ISR(TIMER4_COMPB_vect) //fuelSchedule4
#else
static inline void fuelSchedule4Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (fuelSchedule4.Status == PENDING) //Check to see if this schedule is turn on
    {
      //fuelSchedule4.StartCallback();
      openInjector4();
      fuelSchedule4.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      FUEL4_COMPARE = FUEL4_COUNTER + uS_TO_TIMER_COMPARE(fuelSchedule4.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (fuelSchedule4.Status == RUNNING)
    {
       //fuelSchedule4.EndCallback();
       closeInjector4();
       fuelSchedule4.Status = OFF; //Turn off the schedule
       fuelSchedule4.schedulesSet = 0;

       //If there is a next schedule queued up, activate it
       if(fuelSchedule4.hasNextSchedule == true)
       {
         FUEL4_COMPARE = fuelSchedule4.nextStartCompare;
         fuelSchedule4.endCompare = fuelSchedule4.nextEndCompare;
         fuelSchedule4.Status = PENDING;
         fuelSchedule4.schedulesSet = 1;
         fuelSchedule4.hasNextSchedule = false;
       }
       else { FUEL4_TIMER_DISABLE(); }
    }
  }

#if (INJ_CHANNELS >= 5)
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPC_vect) //fuelSchedule5
#else
static inline void fuelSchedule5Interrupt() //Most ARM chips can simply call a function
#endif
{
  if (fuelSchedule5.Status == PENDING) //Check to see if this schedule is turn on
  {
    openInjector5();
    fuelSchedule5.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
    FUEL5_COMPARE = fuelSchedule5.endCompare;
  }
  else if (fuelSchedule5.Status == RUNNING)
  {
     closeInjector5();
     fuelSchedule5.Status = OFF; //Turn off the schedule
     fuelSchedule5.schedulesSet = 0;

     //If there is a next schedule queued up, activate it
     if(fuelSchedule5.hasNextSchedule == true)
     {
       FUEL5_COMPARE = fuelSchedule5.nextStartCompare;
       fuelSchedule5.endCompare = fuelSchedule5.nextEndCompare;
       fuelSchedule5.Status = PENDING;
       fuelSchedule5.schedulesSet = 1;
       fuelSchedule5.hasNextSchedule = false;
     }
     else { FUEL5_TIMER_DISABLE(); }
  }
}
#endif

#if (INJ_CHANNELS >= 6)
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPA_vect) //fuelSchedule6
#else
static inline void fuelSchedule6Interrupt() //Most ARM chips can simply call a function
#endif
{
  if (fuelSchedule6.Status == PENDING) //Check to see if this schedule is turn on
  {
    //fuelSchedule4.StartCallback();
    openInjector6();
    fuelSchedule6.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
    FUEL6_COMPARE = fuelSchedule6.endCompare;
  }
  else if (fuelSchedule6.Status == RUNNING)
  {
     //fuelSchedule4.EndCallback();
     closeInjector6();
     fuelSchedule6.Status = OFF; //Turn off the schedule
     fuelSchedule6.schedulesSet = 0;

     //If there is a next schedule queued up, activate it
     if(fuelSchedule6.hasNextSchedule == true)
     {
       FUEL6_COMPARE = fuelSchedule6.nextStartCompare;
       fuelSchedule6.endCompare = fuelSchedule6.nextEndCompare;
       fuelSchedule6.Status = PENDING;
       fuelSchedule6.schedulesSet = 1;
       fuelSchedule6.hasNextSchedule = false;
     }
     else { FUEL6_TIMER_DISABLE(); }
  }
}
#endif

#if (INJ_CHANNELS >= 7)
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER5_COMPC_vect) //fuelSchedule7
#else
static inline void fuelSchedule7Interrupt() //Most ARM chips can simply call a function
#endif
{
  if (fuelSchedule7.Status == PENDING) //Check to see if this schedule is turn on
  {
    openInjector7();
    fuelSchedule7.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
    FUEL7_COMPARE = fuelSchedule7.endCompare;
  }
  else if (fuelSchedule7.Status == RUNNING)
  {
     closeInjector7();
     fuelSchedule7.Status = OFF; //Turn off the schedule
     fuelSchedule7.schedulesSet = 0;

     //If there is a next schedule queued up, activate it
     if(fuelSchedule7.hasNextSchedule == true)
     {
       FUEL7_COMPARE = fuelSchedule7.nextStartCompare;
       fuelSchedule7.endCompare = fuelSchedule7.nextEndCompare;
       fuelSchedule7.Status = PENDING;
       fuelSchedule7.schedulesSet = 1;
       fuelSchedule7.hasNextSchedule = false;
     }
     else { FUEL7_TIMER_DISABLE(); }
  }
}
#endif

#if (INJ_CHANNELS >= 8)
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER5_COMPB_vect) //fuelSchedule8
#else
static inline void fuelSchedule8Interrupt() //Most ARM chips can simply call a function
#endif
{
  if (fuelSchedule8.Status == PENDING) //Check to see if this schedule is turn on
  {
    //fuelSchedule4.StartCallback();
    openInjector8();
    fuelSchedule8.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
    FUEL8_COMPARE = fuelSchedule8.endCompare;
  }
  else if (fuelSchedule8.Status == RUNNING)
  {
     //fuelSchedule4.EndCallback();
     closeInjector8();
     fuelSchedule8.Status = OFF; //Turn off the schedule
     fuelSchedule8.schedulesSet = 0;

     //If there is a next schedule queued up, activate it
     if(fuelSchedule8.hasNextSchedule == true)
     {
       FUEL8_COMPARE = fuelSchedule8.nextStartCompare;
       fuelSchedule8.endCompare = fuelSchedule8.nextEndCompare;
       fuelSchedule8.Status = PENDING;
       fuelSchedule8.schedulesSet = 1;
       fuelSchedule8.hasNextSchedule = false;
     }
     else { FUEL8_TIMER_DISABLE(); }
  }
}
#endif

#if IGN_CHANNELS >= 1
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER5_COMPA_vect) //ignitionSchedule1
#else
static inline void ignitionSchedule1Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule1.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule1.StartCallback();
      ignitionSchedule1.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule1.startTime = micros();
      if(ignitionSchedule1.endScheduleSetByDecoder == true) { IGN1_COMPARE = ignitionSchedule1.endCompare; }
      else { IGN1_COMPARE = IGN1_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule1.duration); } //Doing this here prevents a potential overflow on restarts
    }
    else if (ignitionSchedule1.Status == RUNNING)
    {
      ignitionSchedule1.EndCallback();
      //   *ign1_pin_port &= ~(ign1_pin_mask);
      ignitionSchedule1.Status = OFF; //Turn off the schedule
      ignitionSchedule1.schedulesSet = 0;
      ignitionSchedule1.hasNextSchedule = false;
      ignitionSchedule1.endScheduleSetByDecoder = false;
      ignitionCount += 1; //Increment the igintion counter
      IGN1_TIMER_DISABLE();
    }
    else if (ignitionSchedule1.Status == OFF)
    {
      //Catch any spurious interrupts. This really shouldn't ever be called, but there as a safety
      IGN1_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 2
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER5_COMPB_vect) //ignitionSchedule2
#else
static inline void ignitionSchedule2Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule2.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule2.StartCallback();
      ignitionSchedule2.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule2.startTime = micros();
      if(ignitionSchedule2.endScheduleSetByDecoder == true) { IGN2_COMPARE = ignitionSchedule2.endCompare; } //If the decoder has set the end compare value, assign it to the next compare
      else { IGN2_COMPARE = IGN2_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule2.duration); } //If the decoder based timing isn't set, doing this here prevents a potential overflow that can occur at low RPMs
    }
    else if (ignitionSchedule2.Status == RUNNING)
    {
      ignitionSchedule2.Status = OFF; //Turn off the schedule
      ignitionSchedule2.EndCallback();
      ignitionSchedule2.schedulesSet = 0;
      ignitionSchedule2.endScheduleSetByDecoder = false;
      ignitionCount += 1; //Increment the igintion counter
      IGN2_TIMER_DISABLE();
    }
    else if (ignitionSchedule2.Status == OFF)
    {
      //Catch any spurious interrupts. This really shouldn't ever be called, but there as a safety
      IGN2_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 3
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER5_COMPC_vect) //ignitionSchedule3
#else
static inline void ignitionSchedule3Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule3.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule3.StartCallback();
      ignitionSchedule3.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule3.startTime = micros();
      if(ignitionSchedule3.endScheduleSetByDecoder == true) { IGN3_COMPARE = ignitionSchedule3.endCompare; } //If the decoder has set the end compare value, assign it to the next compare
      else { IGN3_COMPARE = IGN3_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule3.duration); } //If the decoder based timing isn't set, doing this here prevents a potential overflow that can occur at low RPMs
    }
    else if (ignitionSchedule3.Status == RUNNING)
    {
       ignitionSchedule3.Status = OFF; //Turn off the schedule
       ignitionSchedule3.EndCallback();
       ignitionSchedule3.schedulesSet = 0;
       ignitionSchedule3.endScheduleSetByDecoder = false;
       ignitionCount += 1; //Increment the igintion counter

       //If there is a next schedule queued up, activate it
       if(ignitionSchedule3.hasNextSchedule == true)
       {
         IGN3_COMPARE = ignitionSchedule3.nextStartCompare;
         ignitionSchedule3.endCompare = ignitionSchedule3.nextEndCompare;
         ignitionSchedule3.Status = PENDING;
         ignitionSchedule3.schedulesSet = 1;
         ignitionSchedule3.hasNextSchedule = false;
       }
       else { IGN3_TIMER_DISABLE(); }
    }
    else if (ignitionSchedule3.Status == OFF)
    {
      //Catch any spurious interrupts. This really shouldn't ever be called, but there as a safety
      IGN3_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 4
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPA_vect) //ignitionSchedule4
#else
static inline void ignitionSchedule4Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule4.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule4.StartCallback();
      ignitionSchedule4.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule4.startTime = micros();
      IGN4_COMPARE = IGN4_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule4.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (ignitionSchedule4.Status == RUNNING)
    {
       ignitionSchedule4.Status = OFF; //Turn off the schedule
       ignitionSchedule4.EndCallback();
       ignitionSchedule4.schedulesSet = 0;
       ignitionSchedule4.endScheduleSetByDecoder = false;
       ignitionCount += 1; //Increment the igintion counter

       //If there is a next schedule queued up, activate it
       if(ignitionSchedule4.hasNextSchedule == true)
       {
         IGN4_COMPARE = ignitionSchedule4.nextStartCompare;
         ignitionSchedule4.endCompare = ignitionSchedule4.nextEndCompare;
         ignitionSchedule4.Status = PENDING;
         ignitionSchedule4.schedulesSet = 1;
         ignitionSchedule4.hasNextSchedule = false;
       }
       else { IGN4_TIMER_DISABLE(); }
    }
    else if (ignitionSchedule4.Status == OFF)
    {
      //Catch any spurious interrupts. This really shouldn't ever be called, but there as a safety
      IGN4_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 5
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPC_vect) //ignitionSchedule5
#else
static inline void ignitionSchedule5Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule5.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule5.StartCallback();
      ignitionSchedule5.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule5.startTime = micros();
      IGN5_COMPARE = IGN5_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule5.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (ignitionSchedule5.Status == RUNNING)
    {
       ignitionSchedule5.Status = OFF; //Turn off the schedule
       ignitionSchedule5.EndCallback();
       ignitionSchedule5.schedulesSet = 0;
       ignitionSchedule5.endScheduleSetByDecoder = false;
       ignitionCount += 1; //Increment the igintion counter
       IGN5_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 6
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPC_vect) //ignitionSchedule6  NOT CORRECT!!!
#else
static inline void ignitionSchedule6Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule6.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule6.StartCallback();
      ignitionSchedule6.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule6.startTime = micros();
      IGN6_COMPARE = IGN6_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule6.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (ignitionSchedule6.Status == RUNNING)
    {
       ignitionSchedule6.Status = OFF; //Turn off the schedule
       ignitionSchedule6.EndCallback();
       ignitionSchedule6.schedulesSet = 0;
       ignitionSchedule6.endScheduleSetByDecoder = false;
       ignitionCount += 1; //Increment the igintion counter
       IGN6_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 7
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPC_vect) //ignitionSchedule6  NOT CORRECT!!!
#else
static inline void ignitionSchedule7Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule7.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule7.StartCallback();
      ignitionSchedule7.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule7.startTime = micros();
      IGN7_COMPARE = IGN7_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule7.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (ignitionSchedule7.Status == RUNNING)
    {
       ignitionSchedule7.Status = OFF; //Turn off the schedule
       ignitionSchedule7.EndCallback();
       ignitionSchedule7.schedulesSet = 0;
       ignitionSchedule7.endScheduleSetByDecoder = false;
       ignitionCount += 1; //Increment the igintion counter
       IGN7_TIMER_DISABLE();
    }
  }
#endif

#if IGN_CHANNELS >= 8
#if defined(CORE_AVR) //AVR chips use the ISR for this
ISR(TIMER4_COMPC_vect) //ignitionSchedule8  NOT CORRECT!!!
#else
static inline void ignitionSchedule8Interrupt() //Most ARM chips can simply call a function
#endif
  {
    if (ignitionSchedule8.Status == PENDING) //Check to see if this schedule is turn on
    {
      ignitionSchedule8.StartCallback();
      ignitionSchedule8.Status = RUNNING; //Set the status to be in progress (ie The start callback has been called, but not the end callback)
      ignitionSchedule8.startTime = micros();
      IGN8_COMPARE = IGN8_COUNTER + uS_TO_TIMER_COMPARE(ignitionSchedule8.duration); //Doing this here prevents a potential overflow on restarts
    }
    else if (ignitionSchedule8.Status == RUNNING)
    {
       ignitionSchedule8.Status = OFF; //Turn off the schedule
       ignitionSchedule8.EndCallback();
       ignitionSchedule8.schedulesSet = 0;
       ignitionSchedule8.endScheduleSetByDecoder = false;
       ignitionCount += 1; //Increment the igintion counter
       IGN8_TIMER_DISABLE();
    }
  }
#endif


#if defined(CORE_TEENSY35)
void ftm0_isr(void)
{
  //Use separate variables for each test to ensure conversion to bool
  bool interrupt1 = (FTM0_C0SC & FTM_CSC_CHF);
  bool interrupt2 = (FTM0_C1SC & FTM_CSC_CHF);
  bool interrupt3 = (FTM0_C2SC & FTM_CSC_CHF);
  bool interrupt4 = (FTM0_C3SC & FTM_CSC_CHF);
  bool interrupt5 = (FTM0_C4SC & FTM_CSC_CHF);
  bool interrupt6 = (FTM0_C5SC & FTM_CSC_CHF);
  bool interrupt7 = (FTM0_C6SC & FTM_CSC_CHF);
  bool interrupt8 = (FTM0_C7SC & FTM_CSC_CHF);

  if(interrupt1) { FTM0_C0SC &= ~FTM_CSC_CHF; fuelSchedule1Interrupt(); }
  else if(interrupt2) { FTM0_C1SC &= ~FTM_CSC_CHF; fuelSchedule2Interrupt(); }
  else if(interrupt3) { FTM0_C2SC &= ~FTM_CSC_CHF; fuelSchedule3Interrupt(); }
  else if(interrupt4) { FTM0_C3SC &= ~FTM_CSC_CHF; fuelSchedule4Interrupt(); }
  else if(interrupt5) { FTM0_C4SC &= ~FTM_CSC_CHF; ignitionSchedule1Interrupt(); }
  else if(interrupt6) { FTM0_C5SC &= ~FTM_CSC_CHF; ignitionSchedule2Interrupt(); }
  else if(interrupt7) { FTM0_C6SC &= ~FTM_CSC_CHF; ignitionSchedule3Interrupt(); }
  else if(interrupt8) { FTM0_C7SC &= ~FTM_CSC_CHF; ignitionSchedule4Interrupt(); }

}
void ftm3_isr(void)
{

#if (INJ_CHANNELS >= 5)
  bool interrupt1 = (FTM3_C0SC & FTM_CSC_CHF);
  if(interrupt1) { FTM3_C0SC &= ~FTM_CSC_CHF; fuelSchedule5Interrupt(); }
#endif
#if (INJ_CHANNELS >= 6)
  bool interrupt2 = (FTM3_C1SC & FTM_CSC_CHF);
  if(interrupt2) { FTM3_C1SC &= ~FTM_CSC_CHF; fuelSchedule6Interrupt(); }
#endif
#if (INJ_CHANNELS >= 7)
  bool interrupt3 = (FTM3_C2SC & FTM_CSC_CHF);
  if(interrupt3) { FTM3_C2SC &= ~FTM_CSC_CHF; fuelSchedule7Interrupt(); }
#endif
#if (INJ_CHANNELS >= 8)
  bool interrupt4 = (FTM3_C3SC & FTM_CSC_CHF);
  if(interrupt4) { FTM3_C3SC &= ~FTM_CSC_CHF; fuelSchedule8Interrupt(); }
#endif
#if (IGN_CHANNELS >= 5)
  bool interrupt5 = (FTM3_C4SC & FTM_CSC_CHF);
  if(interrupt5) { FTM3_C4SC &= ~FTM_CSC_CHF; ignitionSchedule5Interrupt(); }
#endif
#if (IGN_CHANNELS >= 6)
  bool interrupt6 = (FTM3_C5SC & FTM_CSC_CHF);
  if(interrupt6) { FTM3_C5SC &= ~FTM_CSC_CHF; ignitionSchedule6Interrupt(); }
#endif
#if (IGN_CHANNELS >= 7)
  bool interrupt7 = (FTM3_C6SC & FTM_CSC_CHF);
  if(interrupt7) { FTM3_C6SC &= ~FTM_CSC_CHF; ignitionSchedule7Interrupt(); }
#endif
#if (IGN_CHANNELS >= 8)
  bool interrupt8 = (FTM3_C7SC & FTM_CSC_CHF);
  if(interrupt8) { FTM3_C7SC &= ~FTM_CSC_CHF; ignitionSchedule8Interrupt(); }
#endif

}
#endif
