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
  current = &StateHandler::stateInit;
  (this->*current) (Event (Event::eEnter));
  current_mode = MANUAL;
  saved_set_value = 0;
  saved_curr_value = 0;
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

  if (current_mode == MANUAL)
    {
      snprintf (line_up, 16, "SPEED: %02d%", value1);
      snprintf (line_down, 16, "PRESSURE: %02dPa", value2);
    }
  else
    {
      snprintf (line_up, 16, "P. SET: %02dPa", value1);
      snprintf (line_down, 16, "P. CURR: %02dPa", value2);
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
StateHandler::stateInit (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      break;
    case Event::eExit:
      _lcd->clear ();
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (current_mode == MANUAL)
        {
          SetState (&StateHandler::stateManual);
        }
      else
        {
          SetState (&StateHandler::stateAuto);
        }
      break;
    }
}

void
StateHandler::stateManual (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      break;
    case Event::eExit:
      _lcd->clear ();
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (saved_curr_value != event.value
          || saved_set_value != value[MANUAL].getCurrent ())
        {
          saved_curr_value = event.value;
          saved_set_value = value[MANUAL].getCurrent ();
          displaySet (value[MANUAL].getCurrent (), event.value);
        }
      break;
    }
}

void
StateHandler::stateAuto (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      break;
    case Event::eExit:
      _lcd->clear ();
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (saved_curr_value != event.value
          || saved_set_value != value[AUTO].getCurrent ())
        {
          saved_curr_value = event.value;
          saved_set_value = value[AUTO].getCurrent ();
          displaySet (value[AUTO].getCurrent (), event.value);
        }
      break;
    }
}

void
StateHandler::handleControlButtons (uint8_t button)
{
  switch (button)
    {
    case BUTTON_CONTROL_DOWN:
      this->value[(current_mode) ? AUTO : MANUAL].dec ();
      break;
    case BUTTON_CONTROL_UP:
      this->value[(current_mode) ? AUTO : MANUAL].inc ();
      break;
    case BUTTON_CONTROL_TOG_MODE:
      current_mode = !current_mode;
      SetState (&StateHandler::stateInit);
      break;
    default:
      break;
    }
}
