#include "i2cEEPROM.h"
#include <Wire.h>
#include <assert.h>

static_assert(BUFFER_LENGTH >= 34, "Please change Wire.h buffer length to 34.");

i2cEEPROMClass EEPROM(1000000);

i2cEEPROMClass::i2cEEPROMClass(uint32_t busSpeed)
{
    Wire.begin();
    Wire.setClock(busSpeed);
}

byte i2cEEPROMClass::read(int address)
{
    byte result = 0;
    if(address >= 0 && address < device_size)
    {
        result = memory[address];
    }
    return result;
}

void i2cEEPROMClass::write(int address, byte data)
{
    update(address, data);
}

void i2cEEPROMClass::update(int address, byte data)
{
    if(address < 0 || address >= device_size)
        return;

    if (read(address) == data)
        return;

    memory[address] = data;

    int pageIndex = address / page_size;
    int dirtyCell = pageIndex / sizeof(byte);
    int dirtyBit = pageIndex & (sizeof(byte)-1);
    dirtyPages[dirtyCell] |= (1 << dirtyBit);
}

bool i2cEEPROMClass::writeAllPages()
{
    // find first dirty cell
    for(unsigned dirtyCell = 0; dirtyCell < page_count/sizeof(byte); dirtyCell++)
    {
        if(dirtyPages[dirtyCell] != 0)
        {
            // find first dirty bit in cell
            for(unsigned dirtyBit = 0; dirtyBit < sizeof(byte); dirtyBit++)
            {
                if(dirtyPages[dirtyCell] & (1 << dirtyBit))
                {
                    int pageIndex = dirtyCell * sizeof(byte) + dirtyBit;
                    int address = pageIndex * page_size;
                    Wire.beginTransmission(device_address);
                    Wire.write(address >> 8);
                    Wire.write(address & 0xff);

                    for(int i = 0; i < page_size; i++, address++)
                    {
                        Wire.write(memory[address]);
                    }

                    Wire.endTransmission(true);

                    dirtyPages[dirtyCell] &= ~(1 << dirtyBit);
                    return isDirty();
                }
            }
        }
    }
    return false;
}

void i2cEEPROMClass::readAllPages()
{
    Wire.beginTransmission(device_address);
    Wire.write(0);
    Wire.write(0);
    Wire.endTransmission(false);
    for(int i = 0; i < device_size; i++)
    {
        Wire.requestFrom(device_address, 1);
        while(!Wire.available()); // Wait
        memory[i] = Wire.read();
    }
}

bool i2cEEPROMClass::isDirty()
{
    bool lIsDirty = false;
    for(unsigned dirtyCell = 0; dirtyCell < page_count/sizeof(byte); dirtyCell++)
    {
        lIsDirty |= dirtyPages[dirtyCell];
    }
    return lIsDirty;
}

void i2cEEPROMClass::busy_wait()
{
    do
    {
        // wait
    } while (isBusy());
}

bool i2cEEPROMClass::isBusy()
{
    Wire.beginTransmission(device_address);
    return Wire.endTransmission(true) != 0;
}
