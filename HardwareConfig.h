/*
 * HardwareConfig.h
 *
 *  Created on: 09.02.2017
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

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

//#define ATMEGA328P
#undef HIGH
#undef LOW
#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "avrlib/devices/switch.h"
#include "avrlib/adc.h"
//#include "avrlib/i2c/i2c.h"
#include "MCP23017.h"
#include "analogSwitch.h"

using namespace avrlib;


template<typename Input, uint8_t ActiveLevel=0>
class EdgeTrigger
{
public:
  static inline void init(void)
  {
    Input::set_mode(DIGITAL_INPUT);
    if(ActiveLevel == 0) Input::High();
  }
  static inline bool isTriggered(void)
  {
    uint8_t in = Input::value();
    bool ret = in != OldValue && in == ActiveLevel;
    OldValue = in;
    return ret;
  }
private:
  static uint8_t OldValue;
};

template<typename Input, uint8_t ActiveLevel>
uint8_t EdgeTrigger<Input, ActiveLevel>::OldValue;

typedef Gpio<PortD, 2> ClockIn;  // 2
typedef EdgeTrigger<Gpio<PortD, 3>, 0> StartIn;  // 3
typedef EdgeTrigger<Gpio<PortD, 4>, 0> ResetIn;  // 4
typedef Gpio<PortB, 3> Input_A;  // 11
typedef Gpio<PortB, 2> Input_B;  // 10

typedef Gpio<PortB, 5> debug;  // 13


typedef Gpio<PortB, 0> Start_;  // 8
typedef Gpio<PortB, 1> Reset_;  // 9
typedef DebouncedSwitch< Start_> StartButton;
typedef DebouncedSwitch< Reset_> ResetButton;

typedef Inverter<Gpio<PortD, 6> > Output_A;  // 6
typedef Inverter<Gpio<PortD, 7> > Output_B;  // 7
typedef Inverter<Gpio<PortD, 5> > ClockOut;  // 5

//typedef I2cMaster<8, 4, 100000> I2C_bus;

typedef AnalogSwitch<Adc, 2, 8> SwitchDivider;
typedef AnalogSwitch<Adc, 3, 8> SwitchMode;
typedef AnalogSwitch<Adc, 1, 3> SwitchLink;     // 1x16; 2x8; ...
typedef AnalogSwitch<Adc, 7, 3> SwitchAction_A;
typedef AnalogSwitch<Adc, 0, 3> SwitchAction_B;

 extern MCP23017 Extender;
 extern Adc adc;

inline void initInputs(void)
{
  ClockIn::set_mode(DIGITAL_INPUT);
  ClockIn::High();
  StartIn::init();
  ResetIn::init();
  Input_A::set_mode(DIGITAL_INPUT);
  Input_A::Low();
  Input_B::set_mode(DIGITAL_INPUT);
  Input_B::Low();

  StartButton::Init();
  ResetButton::Init();
}
inline void initOutputs(void)
{
  Output_A::set_mode(DIGITAL_OUTPUT);
  Output_A::set_value(true);
  Output_B::set_mode(DIGITAL_OUTPUT);
  Output_B::set_value(true);
  ClockOut::set_mode(DIGITAL_OUTPUT);
  ClockOut::set_value(true);

  debug::set_mode(DIGITAL_OUTPUT);
  debug::Low();
}

inline void initAnalogIn(void)
{
  adc.Init();
}

inline void initHW(void)
{
  initInputs();
  initOutputs();
  initAnalogIn();
  Extender.init(0);
  Extender.writeGPIOA(0);
  Extender.writeGPIOB(0);
}




#endif /* HARDWARECONFIG_H_ */
