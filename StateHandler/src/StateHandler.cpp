/*
 * StateHandler.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#include <StateHandler.h>
#define PID 0

StateHandler::StateHandler (LiquidCrystal *lcd, ModbusRegister *A01, PressureWrapper *pressure)
{
  this->_lcd = lcd;
  this->A01 = A01;
  this->pressure = pressure;
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
    	SetState(&StateHandler::stateSensors);
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
      save (event.value, MANUAL);
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
      save (event.value, AUTO);
#if PID
      pid();
      this->A01->write(fan_speed.getCurrent());
#endif
#if !PID
      if(saved_curr_value[AUTO] < saved_set_value[AUTO]) {
		  fan_speed.inc();
		  this->A01->write(fan_speed.getCurrent());
	  } else if(saved_curr_value[AUTO] > saved_set_value[AUTO]){
		  fan_speed.dec();
		  this->A01->write(fan_speed.getCurrent());
	  	 }
#endif
      break;
    }
}

void
StateHandler::stateSensors (const Event &event)
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
	  sensors_data[PRESSUREDAT] = pressure->getPressure();
	  sensors_data[TEMPERATURE] = humidity.readT();
	  sensors_data[HUMIDITY] = humidity.readRH();
	  sensors_data[CO2] = co2.read();
#if 1
	  char line_up[16] = { 0 };
	  char line_down[16] = { 0 };
	  snprintf (line_up, 16, "PRE:%02d  TEM:%02d", sensors_data[PRESSUREDAT],sensors_data[TEMPERATURE]);
	  snprintf (line_down, 16, "HUM:%02d  CO2:%02d", sensors_data[HUMIDITY], sensors_data[CO2]);

	  _lcd->clear ();
	  _lcd->setCursor (0, 0);
	  _lcd->print (line_up);
	  _lcd->setCursor (0, 1);
	  _lcd->print (line_down);
#endif
	  SetState (current_mode? &StateHandler::stateAuto : &StateHandler::stateManual);
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
StateHandler::save (int eventValue, size_t mode)
{
  /* if pressure is not provided from main it checks it in following if{}*/
  if(!eventValue) {
   	  /* Small delay for modbus communications with pressure sensor */
	  int i = 0;
	  while(i<720) i++;
	  i = 0;
	  eventValue = pressure->getPressure();
  }
  int counterValue = value[mode].getCurrent ();
  if (saved_curr_value[mode] != eventValue
      || saved_set_value[mode] != counterValue)
    {
      saved_curr_value[mode] = eventValue;
      saved_set_value[mode] = counterValue;
      displaySet (saved_set_value[mode], saved_curr_value[mode]);
    }
}

void StateHandler::pid () {
	float kP = 0.1, kI = 0.01, kD = 0.01;
	int error = 0, last_error = 0, derivative = 0;
	error = saved_set_value[AUTO] - saved_curr_value[AUTO];
	last_error = error;
	integral += error;
	derivative = error - last_error;
	fan_speed.setInit((kP*error) + (kI*integral) + (kD * derivative));
}

