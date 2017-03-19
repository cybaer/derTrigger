/*
 * stepper.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: cybaer
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
      break;
    }
    case Cxxx:
    {
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
