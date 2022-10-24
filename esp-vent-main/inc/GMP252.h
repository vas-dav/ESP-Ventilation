/*
 * GMP252.h
 *
 *  Created on: 20 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#ifndef GMP252_H_
#define GMP252_H_

#include "Modbus/ModbusMaster.h"
#include "Modbus/ModbusRegister.h"

class GMP252
{
public:
  GMP252 ();
  int read ();
  virtual ~GMP252 ();

private:
  ModbusMaster sens;
  ModbusRegister regInt;
  ModbusRegister regFloat;
};

#endif /* GMP252_H_ */
