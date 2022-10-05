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
#include "Event.h"

/** Buttons enumeration
 * 
 * Current switch state is being passed 
 * from main to StateHandler through 
 * a keyEvent. Enumeration determines the state
 * of the particular button. 
 * */

enum _buttons {
	/** Raises the bar up */
	BUTTON_CONTROL_UP,
	/** Raises the bar down */
	BUTTON_CONTROL_DOWN,
	/** Toggles the mode between auto and
	 *  manual, which changes the state */
	BUTTON_CONTROL_TOG_MODE,
	/** Optional button to toggle the 
	 * activation of the current setting.
	 * Not compulsory to be used. */
	BUTTON_CONTROL_TOG_ACTIVE
};

class StateHandler;
typedef void (StateHandler::*state_pointer)(const Event &);


class StateHandler {
public:
	StateHandler();
	virtual ~StateHandler();
	int getSetPresuure(); // Get currently set pressure 0-100%
	int getSetSpeed(); //Get currently set FanSpeed 0-100%

	/** Handle the given event of the current state
	* @param event event to be handled in the current state 
	*/
	void HandleState(const Event &event);
private:
	state_pointer current;
	/** Set a new curremt state
	* @param newstate new state to be set to current 
	*/
	void SetState(state_pointer newstate);
	bool mode;
	Counter * bar_pressure;
	Counter * bar_speed;

};

#endif /* STATE_HANDLER_H_ */
