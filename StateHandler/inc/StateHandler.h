/*
 * StateHandler.h
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 *
 * Purpose of this class is to store and pass
 * the events of the current mode to further process.
 *
 * Current goal is to make it to operate on interrupts
 * caused by button presses.
 *
 */

#ifndef STATE_HANDLER_H_
#define STATE_HANDLER_H_

#include "DigitalIoPin.h"
#include "Counter.h"

/** A structure to hold button pointers
 *
 * Main four buttons that operate the
 * whole program. The structure should be
 * initialized in main with correct values and
 * passed to main EvenHandler object constructor.
 *
 * */
typedef struct _EVENT_HANDL{
	DigitalIoPin * _button_control_up;
	DigitalIoPin * _button_control_down;
	DigitalIoPin * _button_control_toggle_mode;
	DigitalIoPin * _button_control_toggle_active;
} EVENT_HANDL;

class StateHandler {
public:
	StateHandler(EVENT_HANDL btns);
	virtual ~StateHandler();
	int getSetPresuure(); // Get currently set pressure 0-100%
	int getSetSpeed(); //Get currently set FanSpeed 0-100%
private:
	EVENT_HANDL internal = {0,0,0,0};
	bool mode;
	Counter * bar_pressure;
	Counter * bar_speed;

};

#endif /* STATE_HANDLER_H_ */
