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
#include "LiquidCrystal.h"
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

enum _bars {
	/** 0-100 % */
	FAN_SPEED,
	/** 0-120 Pa */
	PRESSURE
};

enum _mode {
	MANUAL,
	AUTO
};

class StateHandler;
typedef void (StateHandler::*state_pointer)(const Event &);


class StateHandler {
public:
	StateHandler(LiquidCrystal * lcd);
	virtual ~StateHandler();

	/** Get currently set pressure
	 * 
	 * @return pressure in range of 0-120
	 */
	unsigned int getSetPressure();

	/** Get currently set FanSpeed
	*
	* @return speed in range of 0-100
	*/
	unsigned int getSetSpeed();

	/** Display values on LCD depending on current mode
	 * 
	 * @param value1 value to be displayed on LCD line 0
	 * @param value2 value to be displayed on LCD line 1
	 */
	void displaySet(unsigned int value1, unsigned int value2);

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
	bool current_mode;
	Counter set [2] = {{0, 100}, {0, 120}};
	LiquidCrystal * _lcd;

};

#endif /* STATE_HANDLER_H_ */
