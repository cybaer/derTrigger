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
#include "avrlib/i2c/i2c.h"

using namespace avrlib;

typedef Gpio<PortD, 2> ClockIn;
typedef Gpio<PortD, 3> StartIn;
typedef Gpio<PortD, 4> ResetIn;

typedef Gpio<PortD, 5> Start_;
typedef Gpio<PortD, 6> Reset_;
typedef DebouncedSwitch< Start_> StartButton;
typedef DebouncedSwitch< Reset_> ResetButton;

typedef Gpio<PortD, 7> Output_A;
typedef Gpio<PortB, 0> Output_B;
typedef Gpio<PortB, 1> ClockOut;

static I2cMaster<8, 4, 400000> I2C_bus;

void initInputs(void)
{
  ClockIn::set_mode(DIGITAL_INPUT);
  ClockIn::High();
  StartIn::set_mode(DIGITAL_INPUT);
  StartIn::High();
  ResetIn::set_mode(DIGITAL_INPUT);
  ResetIn::High();
  StartButton::Init();
  ResetButton::Init();
}
void initOutputs(void)
{
  Output_A::set_mode(DIGITAL_OUTPUT);
  Output_A::set_value(true);
  Output_B::set_mode(DIGITAL_OUTPUT);
  Output_B::set_value(true);
  ClockOut::set_mode(DIGITAL_OUTPUT);
  ClockOut::set_value(true);
}

void initAnalogIn(void)
{
  ;
}

void initHW(void)
{
  initInputs();
  initOutputs();
  initAnalogIn();
}

#endif /* HARDWARECONFIG_H_ */
