/*
 * GMP252.cpp
 *
 *  Created on: 20 Oct 2022
 *      Author: evgenymeshcheryakov
 */

#include "GMP252.h"

GMP252::GMP252 ()
    : sens{ 240 }, regInt{ &sens, 0x0100 }, regFloat{ &sens, 0x0000 }
{

  sens.begin (9600);
}

int
GMP252::read ()
{
  int result = regInt.read ();
  return result;
}

GMP252::~GMP252 ()
{
  // TODO Auto-generated destructor stub
}
