#include "i2cEEPROM.h"
#include <Wire.h>

i2cEEPROMClass EEPROM;

i2cEEPROMClass::i2cEEPROMClass(uint32_t busSpeed)
{
    Wire.begin();
    Wire.setClock(busSpeed);
}

byte i2cEEPROMClass::read(int address)
{
    if (nextAddress != address)
    {
        if (nextAddress == LAST_WAS_WIRTE)
            busy_wait();
        Wire.beginTransmission(device_address);
        Wire.write(address >> 8);
        Wire.write(address & 0xff);
        Wire.endTransmission(false);
    }

    Wire.requestFrom(device_address, 1);
    nextAddress = address + 1;

    while (!Wire.available())
        {}
    byte b = Wire.read();
    return b;
}

void i2cEEPROMClass::write(int address, byte data)
{
    if (nextAddress == LAST_WAS_WIRTE)
        busy_wait();
    Wire.beginTransmission(device_address);
    Wire.write(address >> 8);
    Wire.write(address & 0xff);
    Wire.write(data);
    Wire.endTransmission(true);
    nextAddress = LAST_WAS_WIRTE;
}

void i2cEEPROMClass::update(int address, byte data)
{
    if (read(address) != data)
        write(address, data);
}

void i2cEEPROMClass::busy_wait()
{
    do
    {
        // wait
    } while (is_busy());
}

bool i2cEEPROMClass::is_busy()
{
    Wire.beginTransmission(device_address);
    return Wire.endTransmission(true) != 0;
}
