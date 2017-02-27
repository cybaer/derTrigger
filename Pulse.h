/*
 * Pulse.h
 *
 *  Created on: 26.02.2017
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

#ifndef PULSE_H_
#define PULSE_H_

#include "HardwareConfig.h"

template<typename Output>
class Pulse
{
public:


  void setLength(uint8_t len) { m_Length = len; }
  void set(uint8_t tick)
  {
    m_EndTick = tick + m_Length;
    Output::High();
  }
  void checkReset(uint8_t tick)
  {
    if(m_EndTick-tick <= 0)
    {
      Output::Low();
    }
  }
  bool isActive(void) const { return Output::is_high(); }
private:
  static uint8_t m_Length;
  static uint8_t m_EndTick;
};

template<typename Output>
uint8_t Pulse<Output>::m_Length;
template<typename Output>
uint8_t Pulse<Output>::m_EndTick;


#endif /* PULSE_H_ */
