/*
 * derTrigger.cpp
 *
 *  Created on: 09.02.2017
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

#include "derTrigger.h"
#include "HardwareConfig.h"

volatile bool poll = false;

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
  //ca 4kHz

  static int8_t subClock = 0;
  subClock = (subClock + 1) & 3;

  if (subClock == 0)
  { // 1kHz
    poll = true;
  }
}

void setup()
{
  initHW();

  pinMode(13, OUTPUT);


  //     16MHz / (8 * 510) = 3906,25 Hz
  // prescaler(2)_|
  Timer<2>::set_prescaler(2);//2
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();
}

void loop() {

  digitalWrite(13, true);
static uint8_t x=1;

        Extender.writeGPIOA(x);
        Extender.writeGPIOB(x);
        if(x == 0x80)
          x = 1;
        else
          x = x*2;
  delay(200);
  digitalWrite(13, false);
  delay(200);

  if(poll)
  {
    poll = false;


  }
}
