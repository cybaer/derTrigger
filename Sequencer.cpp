/*
 * Sequencer.cpp
 *
 *  Created on: 22.02.2017
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

#include <stdlib.h> // prevent compiler error
#include "Arduino.h"
#include "Sequencer.h"
#include "analogSwitch.h"

Sequencer sequencer;
Adc adc;

Sequencer::Sequencer(void)
: multivider()
, clock()
, m_Tick(0)
, m_Run(false)
, m_ClockValue(true)
, m_Stepper()
, m_Action_A(*this, &Sequencer::setPulse_A)
, m_Action_B(*this, &Sequencer::setPulse_B)
{
  m_Pulse_A.setLength(DefaultPulseLength);
  m_Pulse_B.setLength(DefaultPulseLength);
}

void Sequencer::onStep(void)
{
  if(m_Run)
  {
    createNextStep();
    //delayMicroseconds(500);
    writeOutStep();
  }
}
void Sequencer::createNextStep(void)
{
  uint8_t step = m_Stepper.getNextStep();
  uint16_t x = 0x1 << step;
  Extender.writeGPIOB(x & 0xFF);
  Extender.writeGPIOA(x >> 8);
}

void Sequencer::writeOutStep(void)
{
  nextStep_A(Input_A::value());
  nextStep_B(Input_B::value());
}

void Sequencer::nextStep_A(uint8_t in)
{
  m_Action_A.action(in);
}

void Sequencer::nextStep_B(uint8_t in)
{
  m_Action_B.action(in);
}

void Sequencer::poll(void)
{
  StartButton::Read();
  ResetButton::Read();

  if(StartButton::lowered()) onStartStop();
  if(ResetButton::lowered()) onReset();

// check the switches for divider,mode, ...
  uint16_t val;
  val = SwitchLink::getValue();

  m_Action_A.setMode(SwitchAction_A::getValue());
  m_Action_B.setMode(SwitchAction_B::getValue());

  uint8_t multividerValue = SwitchMultivider::getValue();
  multivider.setDivisor(calcDivisor(multividerValue));
  multivider.setFactor(calcFactor(multividerValue));

  E_StepModes mode = static_cast<E_StepModes>(SwitchMode::getValue());
  m_Stepper.setStepMode(mode);
}

void Sequencer::update()
{
  if(StartIn::isTriggered()) onStartStop();
  if(ResetIn::isTriggered()) onReset();

  m_ClockValue = ClockIn::value();
  if(clock(multivider(m_ClockValue)))
  {
    onStep();
  }
  m_Pulse_A.checkReset(m_Tick);
  m_Pulse_B.checkReset(m_Tick);
}

void Sequencer::onReset(void)
{
  m_Stepper.reset();
  Extender.writeGPIOB(0x00);
  Extender.writeGPIOA(0);
}

uint8_t Sequencer::calcDivisor(uint8_t value)
{
  uint8_t divisor = 1;
  switch(value)
  {
  case 3: divisor = 2;
    break;
  case 2: divisor = 3;
    break;
  case 1: divisor = 4;
    break;
  case 0: divisor = 8;
    break;
  default: divisor = 1;
  }
  return divisor;
}
uint8_t Sequencer::calcFactor(uint8_t value)
{
  uint8_t factor = 1;
  switch(value)
  {
  case 5: factor = 2;
    break;
  case 6: factor = 3;
    break;
  case 7: factor = 4;
    break;
  default: factor = 1;
  }
  return factor;
}

