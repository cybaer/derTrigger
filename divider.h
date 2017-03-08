/*
 * divider.h
 *
 *  Created on: 04.03.2017
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

#ifndef DIVIDER_H_
#define DIVIDER_H_

class Divider
{
public:
  Divider(void)
  : m_Tick(0)
  , m_Divider(2)
  , m_Factor(1)
  {}
  bool operator()(uint8_t in)
  {

  }
  void onTick(void) { m_Tick++; }
  void setDivider(uint8_t divider) { m_Divider = divider; }
  void setFactor(uint8_t factor) { m_Factor = factor; }
private:
  uint8_t m_Tick;
  uint8_t m_Divider;
  uint8_t m_Factor;
};



#endif /* DIVIDER_H_ */
