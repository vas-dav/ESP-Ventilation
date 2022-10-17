/*
 * PressureWrapper.cpp
 *
 *  Created on: 5 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#include "PressureWrapper.h"

static uint8_t crc8(uint8_t *data, size_t size) {
	uint8_t crc = 0x00;
	uint8_t byteCtr;
	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < size; ++byteCtr) {
		crc ^= (data[byteCtr]);
		for (uint8_t bit = 8; bit > 0; --bit) {
			if (crc & 0x80) crc = (crc << 1) ^ 0x131; //P(x)=x^8+x^5+x^4+1 = 0001 0011 0001
			else crc = (crc << 1);
		}
	}
	return crc;
}

PressureWrapper::PressureWrapper ()
{
	NVIC_DisableIRQ(I2C0_IRQn);
	I2C_config config;
	I2C i2c_c(config);
	i2c = &i2c_c;
}

PressureWrapper::~PressureWrapper ()
{
  // TODO Auto-generated destructor stub
}

int PressureWrapper::getPressure() {

}

PRESSURE_DATA* PressureWrapper::getRawPressure () {
	uint8_t getMeasurementComm = 0xF1;
	i2c->transaction(0x40, &getMeasurementComm, 1, data.rBuffer, 3);
	return &data;
}


