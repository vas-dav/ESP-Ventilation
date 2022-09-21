/*
 * EventHandler.h
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "DigitalIoPin.h"
#include "Counter.h"

typedef struct _EVENT_HANDL{
	DigitalIoPin * _button_control_up;
	DigitalIoPin * _button_control_down;
	DigitalIoPin * _button_control_toggle_mode;
	DigitalIoPin * _button_control_toggle_active;
} EVENT_HANDL;

class EventHandler {
public:
	EventHandler(EVENT_HANDL btns);
	virtual ~EventHandler();
	int getSetPresuure();
	int getSetSpeed();
private:
	EVENT_HANDL internal = {0,0,0,0};
	bool mode;
	Counter * bar_pressure;
	Counter * bar_speed;

};

#endif /* EVENTHANDLER_H_ */
