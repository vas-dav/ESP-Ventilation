/*
 * HMP60.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#include <HMP60.h>

HMP60::HMP60 ()
    : sens{ 241 }, regRHint{ &sens, 0x0100 }, regTint{ &sens, 0x0101 },
      regRHfloat{ &sens, 0x0000 }
{
  sens.begin (9600);
}

inline int
HMP60::readRH ()
{
  return (regRHint.read ()) / 10;
}

inline int
HMP60::readT ()
{
  return (regTint.read ()) / 10;
}

HMP60::~HMP60 ()
{
  // TODO Auto-generated destructor stub
}
