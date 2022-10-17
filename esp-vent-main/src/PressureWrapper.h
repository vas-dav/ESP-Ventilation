/*
 * PressureWrapper.h
 *
 *  Created on: 5 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#ifndef PRESSUREWRAPPER_H_
#define PRESSUREWRAPPER_H_

#include "I2C.h"

#define ADDRESS 0x40
#define READADD 0x81
#define WRITEADD 0x80

typedef struct _PRESSURE{
	uint8_t rBuffer[2];
	uint8_t crc;
}PRESSURE_DATA;

class PressureWrapper
{
public:
  PressureWrapper ();
  /**
   * @brief Get the Status object
   *
   * @return true
   * @return false
   */
  int getPressure ();

  virtual ~PressureWrapper ();

private:
  I2C *i2c;
  PRESSURE_DATA data = {{0, 0}, 0};
  PRESSURE_DATA* getRawPressure ();
};

#endif /* PRESSUREWRAPPER_H_ */
