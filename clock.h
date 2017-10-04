/*
 * clock.h
 *
 *  Created on: 03.03.2017
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

#ifndef CLOCK_H_
#define CLOCK_H_

template <typename ClockOut>
class Clock
{
public:
  Clock(void)
  : m_OldValue(0)
  {}
  // always wanted to use a functor :-)
  bool operator()(uint8_t in)
  {
    ClockOut::set_value(in);
    bool raised = (in != 0 && m_OldValue == 0);
    m_OldValue = in;
    return raised;
  }

private:
  uint8_t m_OldValue;
};

#endif /* CLOCK_H_ */
