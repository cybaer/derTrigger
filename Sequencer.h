/*
 * Sequencer.h
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

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "HardwareConfig.h"
#include "Pulse.h"
#include "clock.h"
#include "divider.h"
#include "stepper.h"

using namespace avrlib;

class Sequencer
{
public:
  Sequencer(void);
  void onClock(void) { m_Tick++; divider.onTick(); } // called only in ISR context
  void onStep(void);
  void poll(void);
  void update(void);
  void onStartStop(void) { m_Run = !m_Run; };
  void onReset(void);
  void setPulse_A(void) { m_Pulse_A.set(m_Tick); };
  void setPulse_B(void) { m_Pulse_B.set(m_Tick); };

private:
  void createNextStep(void);
  void writeOutStep(void);
  void nextStep_A(uint8_t in);
  void nextStep_B(uint8_t in);

  Divider divider;
  Clock<ClockOut> clock;
  uint8_t m_Tick;
  bool m_Run;
  bool m_ClockValue;
  Stepper m_Stepper;
  Pulse<Output_A> m_Pulse_A;
  Pulse<Output_B> m_Pulse_B;

  typedef void (Sequencer::*P2MF)(void);

  class StepAction
  {
  public:
    StepAction(Sequencer& parent, P2MF function)
    : m_Parent(parent)
    , setPulse(function)
    , m_Mode(1)
    {}
    void setMode(uint8_t mode) { m_Mode = mode; }
    void action(uint8_t in)
    {
      if(in)
      {
        switch(m_Mode)
        {
        case 0:
          m_Parent.onReset();
          break;
        case 1:
          (m_Parent.*setPulse)(); // call of P2MF
          break;
        case 2:
          m_Parent.onStartStop();
          break;
        }
      }
    }
  private:
    Sequencer& m_Parent;
    P2MF setPulse;
    uint8_t m_Mode;
  };

  friend class StepAction;
  StepAction m_Action_A;
  StepAction m_Action_B;
};

extern Sequencer sequencer;

#endif /* SEQUENCER_H_ */
