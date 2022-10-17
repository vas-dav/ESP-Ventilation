/*
 * SwitchController.cpp
 *
 *  Created on: Oct 17, 2022
 *      Author: tylen
 */

#include <SwitchController.h>

SwitchController::SwitchController (DigitalIoPin *button, Timer *timer,
                                    StateHandler *handler, int button_mode)
{
  b = button;
  t = timer;
  h = handler;
  b_state = false;
  b_mode = button_mode;
  t->resetCounter ();
}

SwitchController::~SwitchController ()
{
  // TODO Auto-generated destructor stub
}

void
SwitchController::listen ()
{
  if (b->read ())
    {
      b_state = true;
    }
  if (!b->read () && b_state)
    {
      h->HandleState (Event (Event::eKey, b_mode));
      b_state = false;
    }
}