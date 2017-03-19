/*
 * analogSwitch.h
 *
 *  Created on: 18.03.2017
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

#ifndef ANALOGSWITCH_H_
#define ANALOGSWITCH_H_

#include "avrlib/adc.h"
#include <stdint.h>

template<uint8_t Count>
class Mapper
{
public:

  static uint8_t get(uint8_t in)
  {
    return (in & 0xFE) / Section;
  }
  static const uint8_t Section = (0xFF / Count) +1;
};

template <class Adc, uint8_t Port, uint8_t Count>
class AnalogSwitch
{
public:
  typedef Mapper<Count> Map;
  static uint8_t getValue(void)
  {
    return Map::get(Adc::Read(Port) >> 2);
  }
};



#endif /* ANALOGSWITCH_H_ */
