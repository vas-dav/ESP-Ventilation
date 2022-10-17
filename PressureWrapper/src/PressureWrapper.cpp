/*
 * PressureWrapper.cpp
 *
 *  Created on: 5 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#include <PressureWrapper.h>

PressureWrapper::PressureWrapper (I2C *i2c) : i2c(i2c)
{

}

PressureWrapper::~PressureWrapper ()
{
  // TODO Auto-generated destructor stub
}

bool PressureWrapper::getStatus() {
	uint8_t control_register = 0x01;
	uint8_t status = 0;
	i2c->transaction(ADDRESS, &control_register, 1, &status, 1);
}

PRESSURE_DATA* PressureWrapper::getPressure () {
	uint8_t getMeasurementComm = 0xF1;
	i2c->transaction(ADDRESS, &getMeasurementComm, 1, data.rBuffer, 3);
	//i2c->transaction(ADDRESS, &getMeasurementComm, 1, data.crc, 1);
	return &data;
}
