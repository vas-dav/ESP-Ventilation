/*
 * HMP60.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#include <HMP60.h>

HMP60::HMP60() : sens{241}, regRHint{&sens, 0x0100},
				regTint{&sens, 0x0101}, regRHfloat{&sens, 0x0000}
{
	sens.begin(9600);
}

int HMP60::readRH() {
	int result = regRHint.read();
	return result/10;
}

int HMP60::readT() {
	int result = regTint.read();
	return result/10;
}

HMP60::~HMP60() {
	// TODO Auto-generated destructor stub
}

