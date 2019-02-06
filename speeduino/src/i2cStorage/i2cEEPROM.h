#ifndef __i2cEEPROM_H__
#define __i2cEEPROM_H__

#include <Arduino.h>
class i2cEEPROMClass {
  const int LAST_WAS_WIRTE = -1;
  const int device_address = 0b1010000;
  int nextAddress = LAST_WAS_WIRTE;
public:
  i2cEEPROMClass(uint32_t busSpeed) ;
  byte read(int address);
  void write(int address, byte data);
  void update(int address, byte data);
  bool is_busy();
private:
  void busy_wait();
};

extern i2cEEPROMClass EEPROM;

#endif // __i2cEEPROM_H__