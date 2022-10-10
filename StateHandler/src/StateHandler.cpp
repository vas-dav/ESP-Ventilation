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

void
StateHandler::displaySet (unsigned int value1, unsigned int value2)
{
  char line_up[16] = { 0 };
  char line_down[16] = { 0 };

  switch (current_mode)
    {
    /*
     * MANUAL MODE:
     * ----------------
     * SPEED: 20%
     * PRESSURE: XXPa
     * ----------------
     */
    case MANUAL:
      snprintf (line_up, 16, "SPEED: %02d%", value1);
      snprintf (line_down, 16, "PRESSURE: %02dPa", value2);
      break;
    /*
     * AUTO MODE:
     * ----------------
     * PRESSURE SET: 35Pa
     * PRESSURE CUR: XXPa
     * ----------------
     */
    case AUTO:
      snprintf (line_up, 16, "P. SET: %02dPa", value1);
      snprintf (line_down, 16, "P. CURR: %02dPa", value2);
      break;
    default:
      break;
    }
  _lcd->clear ();
  _lcd->setCursor (0, 0);
  _lcd->print (line_up);
  _lcd->setCursor (0, 1);
  _lcd->print (line_down);
}

unsigned int
StateHandler::getSetPressure ()
{
  return (unsigned int)this->value[PRESSURE].getCurrent ();
}

unsigned int
StateHandler::getSetSpeed ()
{
  return (unsigned int)this->value[FAN_SPEED].getCurrent ();
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

void
stateInit (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      break;
    case Event::eTick:
      break;
    }
}

void
stateManual (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      break;
    case Event::eTick:
      break;
    }
}

void
stateAuto (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      break;
    case Event::eTick:
      break;
    }
}