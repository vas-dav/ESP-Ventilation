/*
 * PressureWrapper.cpp
 *
 *  Created on: 5 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#include "PressureWrapper.h"
#include <cstdio>

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
	int16_t pressure = 0;
	if(!getRawPressure ()) {
		unsigned int i = 0;
		while(i<7200000) i++;
		getRawPressure ();
		i = 0;
	}
	if(crc8(data.rBuffer, 2) != data.crc){
	pressure = data.rBuffer[0];
	pressure = pressure << 8;
	pressure |= data.rBuffer[1];
	float result = (float) pressure * 0.95 / 240;
	return (int) result;
	}
	return -255;
}

bool PressureWrapper::getRawPressure () {
	uint8_t getMeasurementComm = 0xF1;
	return (i2c->transaction(ADDRESS, &getMeasurementComm, 1, data.rBuffer, 3));
}


