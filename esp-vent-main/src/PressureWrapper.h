/*
 * PressureWrapper.h
 *
 *  Created on: 5 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#ifndef PRESSUREWRAPPER_H_
#define PRESSUREWRAPPER_H_

#include "I2C.h"
#include <cstdio>

#define ADDRESS 0x40


/**
 * @brief structure to hold a raw data from
 * the pressure sensor
 */

typedef struct _PRESSURE{
	uint8_t rBuffer[2];
	uint8_t crc;
}PRESSURE_DATA;

class PressureWrapper
{
public:
  PressureWrapper ();
  /*
   * @return pressure in Pascal
   */
  int getPressure ();

  virtual ~PressureWrapper ();

private:
  I2C *i2c;
  PRESSURE_DATA data = {{0, 0}, 0};
  /*
   * @return struct with pressure data in
   * rBuffer and CRC check in crc
   */
  bool getRawPressure ();
};

#endif /* PRESSUREWRAPPER_H_ */
