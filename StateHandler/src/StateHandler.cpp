/*
 * StateHandler.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#include <StateHandler.h>

StateHandler::StateHandler (LiquidCrystal *lcd)
{
  this->_lcd = lcd;
  // TODO
}

StateHandler::~StateHandler ()
{
  // TODO Auto-generated destructor stub
}

unsigned int
StateHandler::getSetPressure ()
{
  return (unsigned int)this->set[PRESSURE].getCurrent ();
}

unsigned int
StateHandler::getSetSpeed ()
{
  return (unsigned int)this->set[FAN_SPEED].getCurrent ();
}

void
StateHandler::displaySet (unsigned int value1, unsigned int value2)
{
  // TODO
  /**
   * MANUAL MODE:
   * ----------------
   * SPEED: 20%
   * PRESSURE: XXPa
   * ----------------
   * AUTO MODE:
   * ----------------
   * PRESSURE SET: 35Pa
   * PRESSURE CUR: XXPa
   * ----------------
   */
}

void
StateHandler::HandleState (const Event &event)
{
  (this->*current) (event);
}

void
StateHandler::SetState (state_pointer newstate)
{
  (this->*current) (Event (Event::eExit));
  current = newstate;
  (this->*current) (Event (Event::eEnter));
}

void StateHandler::HandleState(const Event &event){
	(this->*current)(event);
}

void StateHandler::SetState(state_pointer newstate){
	(this->*current)(Event(Event::eExit));
	current = newstate;
	(this->*current)(Event(Event::eEnter));

}
