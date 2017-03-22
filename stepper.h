/*
 * stepper.h
 *
 *  Created on: Mar 19, 2017
 *      Author: cybaer
 */

#ifndef STEPPER_H_
#define STEPPER_H_

#include "inttypes.h"

enum E_StepModes {Forward, Backward, ForBackward, PingPong, Crawl, Cxxx, Brownian, Random};
static const uint8_t MaxSteps = 16;

//template<uint8_t MaxSteps = 16>
class Stepper
{
public:
  Stepper(void);
  uint8_t getNextStep(void);
  void setStepMode(E_StepModes mode);
  void reset(void) { m_StepGenerator->reset(); }
private:
  class StepGenerator
  {
  public:
    StepGenerator(void) : m_Step(0) {}
    virtual void reset(void) { m_Step = 0; }
    virtual uint8_t get(void) = 0;
  protected:
    uint8_t m_Step;
  };
  class LinearGenerator : public StepGenerator
  {
  public:
    LinearGenerator(void) : m_Backward(false), m_ToggleMode(false), m_StartStep(0) {}
    virtual void reset(void)
    {
      if(m_ToggleMode) m_Backward = false;
      StepGenerator::reset();
    }
    virtual uint8_t get(void)
    {
      uint8_t actualStep = m_Backward ? MaxSteps - 1 - m_Step : m_Step;
      if(++m_Step == MaxSteps)
      {
        if(m_ToggleMode)
          m_Backward = !m_Backward;
        m_Step = m_StartStep;
      }
      return actualStep;
    }
    void setForward(void)
    {
      if(m_Backward)
        setReverseStep();
      m_Backward = false;
      m_ToggleMode = false;
      m_StartStep = 0;
    }
    void setBackward(void)
    {
      if(!m_Backward)
        setReverseStep();
      m_Backward = true;
      m_ToggleMode = false;
      m_StartStep = 0;
    }
    void setForBackward(void) { m_ToggleMode = true; m_StartStep = 1; }
    void setPingPong(void) { m_ToggleMode = true; m_StartStep = 0; }
  private:
    void setReverseStep(void)
    {
      if(m_Step == 0)
        m_Step = MaxSteps-1;
      m_Step = MaxSteps - m_Step;
    }
    bool m_Backward;
    bool m_ToggleMode;
    uint8_t m_StartStep;
  };
  StepGenerator* m_StepGenerator;
  LinearGenerator m_LinearStepper;
};



#endif /* STEPPER_H_ */
