/*
 * SwitchController.h
 *
 *  Created on: Oct 17, 2022
 *      Author: tylen
 */

#ifndef SWITCHCONTROLLER_H_
#define SWITCHCONTROLLER_H_

#include "DigitalIoPin.h"
#include "StateHandler.h"
#include "Timer.h"

class SwitchController
{
public:
  SwitchController (DigitalIoPin *button, Timer *timer, StateHandler *handler,
                    int button_mode);
  virtual ~SwitchController ();
  /** Listen to switch button
   */
  void listen ();

private:
  DigitalIoPin *b;
  Timer *t;
  StateHandler *h;
  bool b_state;
  int b_mode;
};

#endif /* SWITCHCONTROLLER_H_ */
