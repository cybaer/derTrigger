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

#include "Arduino.h"
#include "avrlib/random.h"
#include "inttypes.h"

enum E_StepModes {Forward, Backward, ForBackward, PingPong, Crawl, Crab, Brownian, Random};
static const uint8_t MaxSteps = 16;

// poss.: template<uint8_t MaxSteps = 16>
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
    StepGenerator(void) : m_NextStep(0) {}
    virtual void reset(void) { m_NextStep = 0; }
    virtual uint8_t get(void) = 0;
  protected:
    void setNextStep(int8_t delta)
    {
      const int8_t testStep = m_NextStep + delta;
      // handle the wrap arround
      m_NextStep = testStep < 0         ? MaxSteps + testStep :
                   testStep >= MaxSteps ? testStep - MaxSteps :
                   testStep;
    }
    uint8_t m_NextStep;
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
      uint8_t actualStep = m_Backward ? MaxSteps - 1 - m_NextStep : m_NextStep;
      if(++m_NextStep == MaxSteps)
      {
        if(m_ToggleMode)
          m_Backward = !m_Backward;
        m_NextStep = m_StartStep;
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
      if(m_NextStep == 0)
        m_NextStep = MaxSteps-1;
      m_NextStep = MaxSteps - m_NextStep;
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
      const uint8_t actualStep = m_NextStep;
      if(++m_BitCounter > PatternLen) m_BitCounter = 0;
      const uint8_t bitMask = 0x01 << m_BitCounter;
      const bool bitActive = (Pattern & bitMask) != 0;
      const int8_t delta = bitActive ? value_1 : vatue_0;
      setNextStep(delta);
      return actualStep;
    }
  private:
    uint8_t m_BitCounter;
  };

  class BrownianGenerator : public StepGenerator
    {
    public:
      virtual uint8_t get(void)
      {
        uint8_t actualStep = m_NextStep;
        uint8_t rnd = avrlib::Random::GetByte();
        int8_t deltaStep = rnd > 127 ?  1 :
                           rnd <  64 ? -1 :
                           0;
        setNextStep(deltaStep);
        return actualStep;
      }
    };

  class RandomGenerator : public StepGenerator
  {
  public:
    virtual uint8_t get(void)
    {
      uint8_t actualStep = m_NextStep;
      m_NextStep = avrlib::Random::GetByte() & 0x0F;
      return actualStep;
    }
  };

  StepGenerator* m_StepGenerator;
  LinearGenerator m_LinearStepper;
  NonLinearGenerator<0x06, 2, -1, 1> m_CrawlStepper;
  NonLinearGenerator<0xAA, 7, -3, 4> m_CrabStepper;
  BrownianGenerator m_BrownianGenerator;
  RandomGenerator m_RandomGenerator;
};

#endif /* STEPPER_H_ */
