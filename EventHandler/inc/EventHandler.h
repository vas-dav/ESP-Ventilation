/*
 * EventHandler.h
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "DigitalIoPin.h"

typedef struct _EVENT_HANDL{
	DigitalIoPin * _button_control_up;
	DigitalIoPin * _button_control_down;
	DigitalIoPin * _button_control_toggle_mode;
	DigitalIoPin * _button_control_toggle_active;
} EVENT_HANDL;

class EventHandler {
public:
	EventHandler(EVENT_HANDL * btns);
	virtual ~EventHandler();
private:
	EVENT_HANDL = {0,0,0,0};

};

#endif /* EVENTHANDLER_H_ */
