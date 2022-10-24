/*
 * SwitchController.h
 *
 *  Created on: Oct 17, 2022
 *      Author: tylen
 */

#ifndef SWITCHCONTROLLER_H_
#define SWITCHCONTROLLER_H_

#include "DigitalIoPin.h"
#include "StateHandler/StateHandler.h"
#include "Timer.h"

class SwitchController
{
public:
  SwitchController (DigitalIoPin *button, StateHandler *handler,
                    int button_mode);
  virtual ~SwitchController ();
  /** Listen to switch button
   */
  void listen ();

private:
  DigitalIoPin *b;
  StateHandler *h;
  bool b_pressed;
  int b_mode;
  void buttonOnHold ();
  void buttonInLoop ();
};

#endif /* SWITCHCONTROLLER_H_ */
