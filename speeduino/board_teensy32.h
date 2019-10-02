#ifndef TEENSY32_H
#define TEENSY32_H
#if defined(__MK20DX256__) // teensy 3.2

#define CORE_TEENSY32

/*
***********************************************************************************************************
* General
*/
  void initBoard();
  uint16_t freeRam();
  typedef uint8_t PORT_TYPE;
  typedef uint8_t PINMASK_TYPE;
  typedef uint16_t COMPARE_TYPE;
  typedef uint16_t COUNTER_TYPE;
  #define BOARD_DIGITAL_GPIO_PINS 34
  #define BOARD_NR_GPIO_PINS 34
  #define USE_SERIAL3
  #define EEPROM_LIB_H "src/i2cStorage/i2cEEPROM.h"
  #define UNUSED_DIGITAL_PIN (24)
  #define UNUSED_ANALOG_PIN (A12)

  #define micros_safe() micros() //timer5 method is not used on anything but AVR, the micros_safe() macro is simply an alias for the normal micros()

/*
***********************************************************************************************************
* Schedules
*/
  static int dummy_value;
  //http://shawnhymel.com/661/learning-the-teensy-lc-interrupt-service-routines/
  #define FUEL1_COUNTER FTM0_CNT
  #define FUEL2_COUNTER FTM0_CNT
  #define FUEL3_COUNTER FTM0_CNT
  #define FUEL4_COUNTER FTM0_CNT
  #define FUEL5_COUNTER dummy_value
  #define FUEL6_COUNTER dummy_value
  #define FUEL7_COUNTER dummy_value
  #define FUEL8_COUNTER dummy_value

  #define IGN1_COUNTER  FTM0_CNT
  #define IGN2_COUNTER  FTM0_CNT
  #define IGN3_COUNTER  FTM0_CNT
  #define IGN4_COUNTER  FTM0_CNT
  #define IGN5_COUNTER  dummy_value
  #define IGN6_COUNTER  dummy_value
  #define IGN7_COUNTER  dummy_value
  #define IGN8_COUNTER  dummy_value

  #define FUEL1_COMPARE FTM0_C0V
  #define FUEL2_COMPARE FTM0_C1V
  #define FUEL3_COMPARE FTM0_C2V
  #define FUEL4_COMPARE FTM0_C3V
  #define FUEL5_COMPARE dummy_value
  #define FUEL6_COMPARE dummy_value
  #define FUEL7_COMPARE dummy_value
  #define FUEL8_COMPARE dummy_value

  #define IGN1_COMPARE  FTM0_C4V
  #define IGN2_COMPARE  FTM0_C5V
  #define IGN3_COMPARE  FTM0_C6V
  #define IGN4_COMPARE  FTM0_C7V
  #define IGN5_COMPARE  dummy_value
  #define IGN6_COMPARE  dummy_value
  #define IGN7_COMPARE  dummy_value
  #define IGN8_COMPARE  dummy_value

  #define FUEL1_TIMER_ENABLE() FTM0_C0SC |= FTM_CSC_CHIE //Write 1 to the CHIE (Channel Interrupt Enable) bit of channel 0 Status/Control
  #define FUEL2_TIMER_ENABLE() FTM0_C1SC |= FTM_CSC_CHIE
  #define FUEL3_TIMER_ENABLE() FTM0_C2SC |= FTM_CSC_CHIE
  #define FUEL4_TIMER_ENABLE() FTM0_C3SC |= FTM_CSC_CHIE
  #define FUEL5_TIMER_ENABLE() ;
  #define FUEL6_TIMER_ENABLE() ;
  #define FUEL7_TIMER_ENABLE() ;
  #define FUEL8_TIMER_ENABLE() ;

  #define FUEL1_TIMER_DISABLE() FTM0_C0SC &= ~FTM_CSC_CHIE //Write 0 to the CHIE (Channel Interrupt Enable) bit of channel 0 Status/Control
  #define FUEL2_TIMER_DISABLE() FTM0_C1SC &= ~FTM_CSC_CHIE
  #define FUEL3_TIMER_DISABLE() FTM0_C2SC &= ~FTM_CSC_CHIE
  #define FUEL4_TIMER_DISABLE() FTM0_C3SC &= ~FTM_CSC_CHIE
  #define FUEL5_TIMER_DISABLE() ;
  #define FUEL6_TIMER_DISABLE() ;
  #define FUEL7_TIMER_DISABLE() ;
  #define FUEL8_TIMER_DISABLE() ;

  #define IGN1_TIMER_ENABLE() FTM0_C4SC |= FTM_CSC_CHIE
  #define IGN2_TIMER_ENABLE() FTM0_C5SC |= FTM_CSC_CHIE
  #define IGN3_TIMER_ENABLE() FTM0_C6SC |= FTM_CSC_CHIE
  #define IGN4_TIMER_ENABLE() FTM0_C7SC |= FTM_CSC_CHIE
  #define IGN5_TIMER_ENABLE() ;
  #define IGN6_TIMER_ENABLE() ;
  #define IGN7_TIMER_ENABLE() ;
  #define IGN8_TIMER_ENABLE() ;

  #define IGN1_TIMER_DISABLE() FTM0_C4SC &= ~FTM_CSC_CHIE
  #define IGN2_TIMER_DISABLE() FTM0_C5SC &= ~FTM_CSC_CHIE
  #define IGN3_TIMER_DISABLE() FTM0_C6SC &= ~FTM_CSC_CHIE
  #define IGN4_TIMER_DISABLE() FTM0_C7SC &= ~FTM_CSC_CHIE
  #define IGN5_TIMER_DISABLE() ;
  #define IGN6_TIMER_DISABLE() ;
  #define IGN7_TIMER_DISABLE() ;
  #define IGN8_TIMER_DISABLE() ;

  #define MAX_TIMER_PERIOD 130321 // 3.555555uS * 65535
  #define MAX_TIMER_PERIOD_SLOW 130321
  #define uS_TO_TIMER_COMPARE(uS) ((uS * 9) >> 5) //Converts a given number of uS into the required number of timer ticks until that time has passed.
  //Hack compatibility with AVR timers that run at different speeds
  #define uS_TO_TIMER_COMPARE_SLOW(uS) ((uS * 9) >> 5)

/*
***********************************************************************************************************
* Auxilliaries
*/
  #define ENABLE_BOOST_TIMER()  FTM1_C0SC |= FTM_CSC_CHIE
  #define DISABLE_BOOST_TIMER() FTM1_C0SC &= ~FTM_CSC_CHIE

  #define ENABLE_VVT_TIMER()    FTM1_C1SC |= FTM_CSC_CHIE
  #define DISABLE_VVT_TIMER()   FTM1_C1SC &= ~FTM_CSC_CHIE

  #define BOOST_TIMER_COMPARE   FTM1_C0V
  #define BOOST_TIMER_COUNTER   FTM1_CNT
  #define VVT_TIMER_COMPARE     FTM1_C1V
  #define VVT_TIMER_COUNTER     FTM1_CNT

  static inline void boostInterrupt();
  static inline void vvtInterrupt();

/*
***********************************************************************************************************
* Idle
*/
  #define IDLE_COUNTER FTM2_CNT
  #define IDLE_COMPARE FTM2_C0V

  #define IDLE_TIMER_ENABLE() FTM2_C0SC |= FTM_CSC_CHIE
  #define IDLE_TIMER_DISABLE() FTM2_C0SC &= ~FTM_CSC_CHIE

  static inline void idleInterrupt();

/*
***********************************************************************************************************
* CAN / Second serial
*/
  //Uart CANSerial (&sercom3, 0, 1, SERCOM_RX_PAD_1, UART_TX_PAD_0);

#endif //CORE_TEENSY
#endif //TEENSY32_H
