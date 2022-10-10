/*
 * PressureWrapper.h
 *
 *  Created on: 5 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#ifndef PRESSUREWRAPPER_H_
#define PRESSUREWRAPPER_H_

#include "DigitalIoPin.h"
#include "I2C.h"

#define ADDRESS 0x40
#define READADD 0x81
#define WRITEADD 0x80

class PressureWrapper
{
public:
	PressureWrapper();
	bool getStatus();
	int16_t getPressure();

	virtual ~PressureWrapper();

private:
	I2C *i2c;
};

#endif /* PRESSUREWRAPPER_H_ */
