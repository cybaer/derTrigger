/*
 * stepper.cpp
 *
 *  Created on: Mar 19, 2017
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

#include "stepper.h"

Stepper::Stepper(void)
: m_StepGenerator(&m_LinearStepper)
{}

uint8_t Stepper::getNextStep(void)
{
  return m_StepGenerator->get();
}

void Stepper::setStepMode(E_StepModes mode)
{
  switch(mode)
  {
    case Forward:
    {
      m_LinearStepper.setForward();
      m_StepGenerator = &m_LinearStepper;
      break;
    }
    case Backward:
    {
      m_LinearStepper.setBackward();
      m_StepGenerator = &m_LinearStepper;
      break;
    }
    case ForBackward:
    {
      m_LinearStepper.setForBackward();
      m_StepGenerator = &m_LinearStepper;
      break;
    }
    case PingPong:
    {
      m_LinearStepper.setPingPong();
      m_StepGenerator = &m_LinearStepper;
      break;
    }
    case Crawl:
    {
      m_StepGenerator = &m_CrawlStepper;
      break;
    }
    case Crab:
    {
      m_StepGenerator = &m_CrabStepper;
      break;
    }
    case Brownian:
    {
      break;
    }
    case Random:
    {
      break;
    }
    default:
    {
      m_LinearStepper.setForward();
      m_StepGenerator = &m_LinearStepper;
    }
  }
}
