/*
 * MCP23017.h
 *
 *  Created on: 20.02.2017
 *      Author: cybaer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MCP23017_H_
#define MCP23017_H_


#include "Wire.h"

#define MCP23017_ADDRESS 0x20

// registers
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14


#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255


class MCP23017 {
public:
  void init(uint8_t addr)
  {
   if (addr > 7) {
     addr = 7;
   }
   i2caddr = addr;

   Wire.begin();

   // set defaults!
   // all inputs on port A and B
   writeRegister(MCP23017_IODIRA,0);
   writeRegister(MCP23017_IODIRB,0);
 }


  void pinMode(uint8_t p, uint8_t d);
  void digitalWrite(uint8_t p, uint8_t d);
  void pullUp(uint8_t p, uint8_t d);
  uint8_t digitalRead(uint8_t p);

  void writeGPIOAB(uint16_t ba)
  {
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(MCP23017_GPIOA);
    Wire.write(ba & 0xFF);
    Wire.write(ba >> 8);
    Wire.endTransmission();
  }
  void writeGPIOA(uint8_t data)
  {
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(MCP23017_GPIOA);
    Wire.write(data);
    Wire.endTransmission();
  }
  void writeGPIOB(uint8_t data)
  {
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(MCP23017_GPIOB);
    Wire.write(data);
    Wire.endTransmission();
  }
  uint16_t readGPIOAB();
  uint8_t readGPIO(uint8_t b);

  void setupInterrupts(uint8_t mirroring, uint8_t open, uint8_t polarity);
  void setupInterruptPin(uint8_t p, uint8_t mode);
  uint8_t getLastInterruptPin();
  uint8_t getLastInterruptPinValue();

 private:
  uint8_t i2caddr;

  uint8_t bitForPin(uint8_t pin);
  uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);

  uint8_t readRegister(uint8_t addr);

  void writeRegister(uint8_t regAddr, uint8_t regValue)
  {
    // Write the register
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(regAddr);
    Wire.write(regValue);
    Wire.endTransmission();
  }

  /**
   * Utility private method to update a register associated with a pin (whether port A/B)
   * reads its value, updates the particular bit, and writes its value.
   */
  void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);

};




#endif /* MCP23017_H_ */
