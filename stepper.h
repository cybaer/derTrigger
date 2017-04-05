/*
 * stepper.h
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

#ifndef STEPPER_H_
#define STEPPER_H_

#include "inttypes.h"

enum E_StepModes {Forward, Backward, ForBackward, PingPong, Crawl, Crab, Brownian, Random};
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



  template<uint8_t Pattern, uint8_t PatternLen, int8_t vatue_0, int8_t value_1>
  class NonLinearGenerator : public StepGenerator
  {
  public:
    NonLinearGenerator(void) : m_BitCounter(0) {};
    virtual void reset(void)
    {
      m_BitCounter = 0;
      StepGenerator::reset();
    }
    virtual uint8_t get(void)
    {
      const uint8_t actualStep = m_Step;
      if(++m_BitCounter > PatternLen) m_BitCounter = 0;
      const uint8_t bitMask = 0x01 << m_BitCounter;
      const bool bitActive = (Pattern & bitMask) != 0;
      const int8_t delta = bitActive ? value_1 : vatue_0;
      const int8_t testStep = m_Step + delta;
      m_Step = testStep < 0         ? MaxSteps - testStep :
               testStep >= MaxSteps ? testStep - MaxSteps :
                                      testStep;
      return actualStep;
    }
  private:
    uint8_t m_BitCounter;
  };
  StepGenerator* m_StepGenerator;
  LinearGenerator m_LinearStepper;
  NonLinearGenerator<0x03, 2, -1, 1> m_CrawlStepper;
};



#endif /* STEPPER_H_ */
