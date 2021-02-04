#include "cancomms.h"

uint8_t currentsecondserialCommand;
uint8_t currentCanPage = 1;//Not the same as the speeduino config page numbers
uint8_t nCanretry = 0;      //no of retrys
uint8_t cancmdfail = 0;     //command fail yes/no
uint8_t canlisten = 0;
uint8_t Lbuffer[8];         //8 byte buffer to store incomng can data
uint8_t Gdata[9];
uint8_t Glow, Ghigh;
bool canCmdPending = false;

#if ( defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) )
  #define CANSerial_AVAILABLE
  HardwareSerial &CANSerial = Serial3;
#elif defined(CORE_STM32)
  #define CANSerial_AVAILABLE
  #ifndef Serial2
    #define Serial2 Serial1
  #endif
  #if defined(STM32GENERIC) // STM32GENERIC core
    SerialUART &CANSerial = Serial2;
  #else //libmaple core aka STM32DUINO
    HardwareSerial &CANSerial = Serial2;
  #endif
#elif defined(CORE_TEENSY)
  #define CANSerial_AVAILABLE
  HardwareSerial &CANSerial = Serial2;
#endif
