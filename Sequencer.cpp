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
: divider()
, clock()
, m_Tick(0)
, m_Run(false)
, m_ClockValue(true)
, m_Step(0)
, m_Pulse_A()
, m_Pulse_B()
{
  m_Pulse_A.setLength(8);
  m_Pulse_B.setLength(8);
}

void Sequencer::onStep(void)
{
  createNextStep();
  //delay(1);
  writeOutStep();
}
void Sequencer::createNextStep(void)
{
  static const uint8_t MaxSteps = 16;
  if(++m_Step == MaxSteps) m_Step = 0;

  uint16_t x = 0x1 << m_Step;
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
  if(in)
  {
    m_Pulse_A.set(m_Tick);
  }

}
void Sequencer::nextStep_B(uint8_t in)
{
  if(in)
  {
    m_Pulse_B.set(m_Tick);
  }
}

void Sequencer::poll(void)
{
  debug::High();

  StartButton::Read();
  ResetButton::Read();
  checkPulseOut();

  if(StartButton::raised()) onStartStop();
  if(ResetButton::raised()) onReset();

  m_ClockValue = ClockIn::value();

  if(clock(divider(m_ClockValue)))
  {
    onStep();
  }

  uint16_t val;
  val = S3::getValue();
  val = S4::getValue();
  val = S5::getValue();

  uint8_t divisor = S1::getValue() + 1;
  divider.setDivisor(divisor);

  uint8_t mode = S1::getValue();

  debug::Low();
}

void Sequencer::update()
{
  m_Pulse_A.checkReset(m_Tick);
  m_Pulse_B.checkReset(m_Tick);
}

void Sequencer::onReset(void)
{

}
void Sequencer::checkPulseOut()
{

}

