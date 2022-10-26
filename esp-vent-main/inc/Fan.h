/*
 * Fan.h
 *
 *  Created on: Oct 26, 2022
 *      Author: tylen
 */

#include "Modbus/ModbusRegister.h"
#include <assert.h>

#ifndef FAN_H_
#define FAN_H_

class Fan
{
public:
  Fan (ModbusRegister *A01);
  virtual ~Fan ();
  void spin (int speed);

private:
  ModbusRegister *_A01;
};

#endif /* FAN_H_ */
