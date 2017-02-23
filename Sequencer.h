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
using namespace avrlib;

class Sequencer
{
public:
  Sequencer(void);
  void onClock(void) { m_Tick++; } // called only in ISR context
  void onStep(void);
  void poll(void);

  void onStartStop(void) { m_Run = !m_Run; };
  void onReset(void);

private:
  void checkPulseOut(void);

  uint8_t m_Tick;
  bool m_Run;
};

extern Sequencer sequencer;

#endif /* SEQUENCER_H_ */
