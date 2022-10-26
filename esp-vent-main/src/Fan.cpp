/*
 * Fan.cpp
 *
 *  Created on: Oct 26, 2022
 *      Author: tylen
 */

#include "Fan.h"

Fan::Fan (ModbusRegister *A01) { this->_A01 = A01; }

Fan::~Fan ()
{
  // TODO Auto-generated destructor stub
}

void
Fan::spin (int speed)
{
  assert ((this->_A01));
  if (speed >= 0 && speed <= 1000)
    {
      this->_A01->write (speed);
    }
}