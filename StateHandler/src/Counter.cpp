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
  if (init - 1 > 0)
    {
      --init;
    }
}

int
Counter::getCurrent ()
{
  return this->init;
}

Counter::Counter (int i, int up)
{
  up_lim = up;
  if (i > up)
    {
      init = up;
    }
  else if (i < 0)
    {
      init = 0;
    }
  else
    {
      init = i;
    }
}

void
Counter::setInit (int i)
{
  init = i;
}
