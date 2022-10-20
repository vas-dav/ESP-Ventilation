/*
 * StateHandler.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#include <StateHandler.h>

StateHandler::StateHandler (LiquidCrystal *lcd, ModbusRegister *A01)
{
  this->_lcd = lcd;
  this->A01 = A01;
  current = &StateHandler::stateInit;
  (this->*current) (Event (Event::eEnter));
  current_mode = MANUAL;
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
      displaySet (saved_set_value[MANUAL], saved_curr_value[MANUAL]);
      this->A01->write(this->value[FAN_SPEED].getCurrent ());
      break;
    case Event::eExit:
      _lcd->clear ();
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      save (event.value, value[MANUAL].getCurrent (), MANUAL);
      break;
    }
}

void
StateHandler::stateAuto (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      displaySet (saved_set_value[AUTO], saved_curr_value[AUTO]);
      break;
    case Event::eExit:
      _lcd->clear ();
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      save (event.value, value[AUTO].getCurrent (), AUTO);
      int i = 0;
//      pid();
//      this->A01->write(fan_speed.getCurrent());
      if(saved_curr_value[AUTO] < saved_set_value[AUTO]) {
		  fan_speed.inc();
		  while(i<720) i++;
		  i = 0;
		  this->A01->write(fan_speed.getCurrent());
	  } else if(saved_curr_value[AUTO] > saved_set_value[AUTO]){
		  fan_speed.dec();
		  while(i<720) i++;
		  i = 0;
		  this->A01->write(fan_speed.getCurrent());
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
      if(current_mode == MANUAL)
    	  this->A01->write(value[(current_mode) ? AUTO : MANUAL].getCurrent() * 10);
      break;
    case BUTTON_CONTROL_UP:
      this->value[(current_mode) ? AUTO : MANUAL].inc ();
      if(current_mode == MANUAL)
    	  this->A01->write(value[(current_mode) ? AUTO : MANUAL].getCurrent() * 10);
      break;
    case BUTTON_CONTROL_TOG_MODE:
      current_mode = !current_mode;
      SetState (&StateHandler::stateInit);
      break;
    default:
      break;
    }
}

void
StateHandler::save (int eventValue, int counterValue, size_t mode)
{
  if (saved_curr_value[mode] != eventValue
      || saved_set_value[mode] != counterValue)
    {
      saved_curr_value[mode] = eventValue;
      saved_set_value[mode] = counterValue;
      displaySet (saved_set_value[mode], saved_curr_value[mode]);
    }
}

void StateHandler::pid () {
	float kP = 0.001, kI = 0.001, kD = 0.001;
	int error = 0, last_error = 0, derivative = 0;
	error = saved_set_value[AUTO] - saved_curr_value[AUTO];
	last_error = error;
	integral += error;
	derivative = error - last_error;
	fan_speed.setInit((kP*error) + (kI*integral) + (kD * derivative));
}

