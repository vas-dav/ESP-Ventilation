/*
 * HMP60.h
 *
 *  Created on: 20 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#ifndef HMP60_H_
#define HMP60_H_

#include "ModbusMaster.h"
#include "ModbusRegister.h"

class HMP60
{
public:
  HMP60 ();
  int readRH ();
  int readT ();
  virtual ~HMP60 ();

private:
  ModbusMaster sens;
  ModbusRegister regRHint;
  ModbusRegister regRHfloat;
  ModbusRegister regTint;
};

#endif /* HMP60_H_ */
