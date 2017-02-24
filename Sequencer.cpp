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

#include "Sequencer.h"

Sequencer sequencer;

Sequencer::Sequencer(void)
: m_Tick(0)
, m_Run(false)
, m_ClockValue(true)
, m_Step(0)
{}

void Sequencer::onStep(void)
{
  nextStep();
  //calcOut_A();
  //calcOut_B();
}
void Sequencer::nextStep(void)
{
  static uint8_t a = 0;
  if(++m_Step == 8) m_Step = 0;
  uint8_t x = 0x1 << m_Step;
  //Extender.writeGPIOB(m_Step);
    Extender.writeGPIOA(x);
}



void Sequencer::poll(void)
{
  StartButton::Read();
  ResetButton::Read();
  checkPulseOut();

  if(StartButton::raised()) onStartStop();
  if(ResetButton::raised()) onReset();


  if(ClockIn::is_low() && m_ClockValue)
  {
    onStep();
    debug::Toggle();
  }
  m_ClockValue = ClockIn::value();



}

void Sequencer::onReset(void)
{

}
void Sequencer::checkPulseOut()
{

}

