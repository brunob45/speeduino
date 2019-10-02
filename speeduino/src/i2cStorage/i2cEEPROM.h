#ifndef __i2cEEPROM_H__
#define __i2cEEPROM_H__

#include <Arduino.h>
class i2cEEPROMClass {
  const int device_address = 0b1010000;
  const int device_size = 4096;
  const int page_size = 32;
  const int page_count = device_size/page_size;

  byte memory[4096];
  byte dirtyPages[128/sizeof(byte)];
public:
  i2cEEPROMClass(uint32_t busSpeed) ;
  byte read(int address);
  void write(int address, byte data);
  void update(int address, byte data);
  bool isBusy();
  bool writeAllPages();
  void readAllPages();
  bool isDirty();
private:
  void busy_wait();
};

extern i2cEEPROMClass EEPROM;

#endif // __i2cEEPROM_H__