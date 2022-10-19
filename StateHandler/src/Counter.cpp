/*
 * Counter.cpp
 *
 *  Created on: Sep 1, 2022
 *      Author: tylen
 */

#include "Counter.h"

void
Counter::inc ()
{
  if (init < up_lim)
    {
      ++init;
    }
}

void
Counter::dec ()
{
  if (init > down_lim)
    {
      --init;
    }
}

unsigned int
Counter::getCurrent ()
{
  return this->init;
}

Counter::Counter (unsigned int down, unsigned int up)
{
  up_lim = up;
  down_lim = down;
  if (down > up)
    {
      init = up;
    }
  else if (down < 0)
    {
      init = 0;
    }
  else
    {
      init = down;
    }
}

void
Counter::setInit (unsigned int newInit)
{
  if(newInit > up_lim){
	  init = up_lim;
  }
  else if(newInit < down_lim){
	  init = down_lim;
  }
  else{
	  init = newInit;
  }
}
