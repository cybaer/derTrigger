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
{}

void Sequencer::onStep(void)
{

}
void Sequencer::poll(void)
{
  StartButton::Read();
  ResetButton::Read();
  checkPulseOut();

  if(StartButton::raised()) onStartStop();
  if(ResetButton::raised()) onReset();
}

void Sequencer::onReset(void)
{

}
void Sequencer::checkPulseOut()
{

}

