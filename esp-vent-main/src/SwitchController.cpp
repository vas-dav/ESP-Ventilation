/*
 * SwitchController.cpp
 *
 *  Created on: Oct 17, 2022
 *      Author: tylen
 */

#include "SwitchController.h"

SwitchController::SwitchController (DigitalIoPin *button,
                                    StateHandler *handler, int button_mode)
{
  b = button;
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
  /** Button is pressed for the first time*/
  if (b->read () && !b_pressed)
    {
      b_pressed = true;
    }
  /** Button is released before 2 seconds*/
  if (!b->read () && b_pressed)
    {
      h->HandleState (Event (Event::eKey, b_mode));
      b_pressed = false;
    }

}

