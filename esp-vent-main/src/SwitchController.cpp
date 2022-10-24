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
  b_pressed = false;
  b_mode = button_mode;
}

SwitchController::~SwitchController ()
{
  // TODO Auto-generated destructor stub
}

void
SwitchController::listen ()
{
  int timer = t->getCounter ();
  /** Button is pressed for the first time*/
  if (b->read () && !b_pressed)
    {
      t->resetCounter ();
      b_pressed = true;
    }
  /** Button is released before 2 seconds*/
  if (!b->read () && b_pressed && timer < 2000)
    {
      h->HandleState (Event (Event::eKey, b_mode));
      b_pressed = false;
      t->resetCounter ();
    }
  /** Button is pressed after 2 seconds*/
  if (b->read () && b_pressed && timer >= 2000)
    {
      buttonOnHold ();
    }
}

void
SwitchController::buttonOnHold ()
{
  t->resetCounter ();
  while (b->read ())
    {
      buttonInLoop ();
    }
  if (b_mode == BUTTON_CONTROL_TOG_MODE)
    {
      h->HandleState (Event (Event::eKey, b_mode));
    }
  b_pressed = false;
  t->resetCounter ();
}

void
SwitchController::buttonInLoop ()
{
  if (t->getCounter () > 50 && b_mode != BUTTON_CONTROL_TOG_MODE)
    {
      h->HandleState (Event (Event::eKey, b_mode));
      h->HandleState (Event (Event::eTick));
      t->resetCounter ();
    }
  t->tickCounter (2);
}
