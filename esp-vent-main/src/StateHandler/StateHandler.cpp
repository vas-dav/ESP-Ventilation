/*
 * StateHandler.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#include "StateHandler/StateHandler.h"

StateHandler::StateHandler (LiquidCrystal *lcd, Fan *propeller,
                            PressureWrapper *pressure, Timer *global)
{
  this->_lcd = lcd;
  this->_propeller = propeller;
  this->_pressure = pressure;
  this->state_timer = global;
  current = &StateHandler::stateInit;
  (this->*current) (Event (Event::eEnter));
  current_mode = MANUAL;
  pressure_achieved = 0;
}

StateHandler::~StateHandler ()
{
  // TODO Auto-generated destructor stub
}

void
StateHandler::displaySet (size_t mode)
{
  char line_up[LCD_SIZE] = { 0 };
  char line_down[LCD_SIZE] = { 0 };

  switch (mode)
    {
    case MANUAL:
      snprintf (line_up, LCD_SIZE, "SPEED: %02d   (M)",
                saved_set_value[current_mode]);
      snprintf (line_down, LCD_SIZE, "PRESSURE: %02dPa",
                saved_curr_value[current_mode]);
      break;
    case AUTO:
      snprintf (line_up, LCD_SIZE, "P.SET: %02dPa (A)",
                saved_set_value[current_mode]);
      snprintf (line_down, LCD_SIZE, "P.CURR: %02dPa",
                saved_curr_value[current_mode]);
      break;
    case SENSORS:
      snprintf (line_up, LCD_SIZE, "PRE:%02d  TEM:%02d",
                sensors_data[PRESSUREDAT], sensors_data[TEMPERATURE]);
      snprintf (line_down, LCD_SIZE, "HUM:%02d  CO2:%02d",
                sensors_data[HUMIDITY], sensors_data[CO2]);
      break;
    case ERROR_TIMEOUT:
      snprintf (line_up, LCD_SIZE, "  FORCE STOP  ");
      snprintf (line_down, LCD_SIZE, "REASON: TIMEOUT");
      break;
    default:
      break;
    }

  _lcd->clear ();
  _lcd->printOnLineOne (line_up);
  _lcd->printOnLineTwo (line_down);
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
      this->_propeller->spin (fan_speed_normalized ());
      break;
    case Event::eExit:
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
      displaySet (MANUAL);
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      handleTickValue (event.value);
      break;
    }
}

void
StateHandler::stateAuto (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      displaySet (AUTO);
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      handleTickValue (event.value);
      if(saved_set_value[AUTO]){
		  pid ();
		  this->_propeller->spin (fan_speed.getCurrent ());
      }
      if (saved_curr_value[AUTO] == saved_set_value[AUTO])
        {
          ++pressure_achieved;
        }
      if (pressure_achieved > 5)
        {
          pressure_achieved = 0;
          task_is_pending = false;
        }
      break;
    }
}

void
StateHandler::stateGetPressure (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      pressure_status = _pressure->isAwake ();
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (!pressure_status)
        {
          _pressure->wakeUp ();
          break;
        }
      savePressureAndDisplay (_pressure->getPressure (),
                              ((current_mode) ? AUTO : MANUAL));
      SetState (current_mode ? &StateHandler::stateAuto
                             : &StateHandler::stateManual);
      break;
    }
}

void
StateHandler::handleControlButtons (uint8_t button)
{
  task_is_pending = true;
  error_timer = 0;
  switch (button)
    {
    case BUTTON_CONTROL_DOWN:
      this->value[(current_mode)].dec ();
      state_timer->resetCounter ();
      break;
    case BUTTON_CONTROL_UP:
      this->value[(current_mode)].inc ();
      state_timer->resetCounter ();
      break;
    case BUTTON_CONTROL_TOG_MODE:
      current_mode = !current_mode;
      SetState (&StateHandler::stateInit);
      state_timer->resetCounter ();
      return;
      break;
    default:
      break;
    }
  if (current_mode == MANUAL && saveSetAndDisplay (MANUAL))
    {
      this->_propeller->spin (fan_speed_normalized ());
    }
  else
    {
      saveSetAndDisplay (AUTO);
    }
}

void
StateHandler::handleTickValue (int value)
{
  if (sensor_timer > TIMER_SENSORS_TIMEOUT)
    {
      updateSensorValues ();
      //displaySet (SENSORS);
      sensor_timer = 0;
    }
  if (value > TIMER_PRESSURE_TIMEOUT)
    {
      SetState (&StateHandler::stateGetPressure);
      sensor_timer += value;
      error_timer += value;
      state_timer->resetCounter ();
    }
  if (error_timer > TIMER_GLOBAL_TIMEOUT && task_is_pending)
    {
      this->fan_speed.setInit (0);
      this->_propeller->spin (0);
      this->value[(current_mode)].setInit (0);
      saveSetAndDisplay (AUTO);
      displaySet (ERROR_TIMEOUT);
      state_timer->Sleep (2000);
      state_timer->resetCounter ();
      error_timer = 0;
      SetState (&StateHandler::stateInit);
    }
}

void
StateHandler::savePressureAndDisplay (int pressure, size_t mode)
{
  if (saved_curr_value[mode] != pressure)
    {
      saved_curr_value[mode] = pressure;
      displaySet (mode);
    }
}

bool
StateHandler::saveSetAndDisplay (size_t mode)
{
  int counterValue = value[mode].getCurrent ();
  if (saved_set_value[mode] != counterValue)
    {
      saved_set_value[mode] = counterValue;
      displaySet (mode);
      return true;
    }
  return false;
}

int
StateHandler::fan_speed_normalized ()
{
  int speed = value[MANUAL].getCurrent ();
  if (speed <= 95)
    speed += 5;
  return speed * 10;
}

void
StateHandler::pid ()
{
  float kP = 0.6, kI = 0.05, kD = 0.125;
  int error = 0, last_error = 0, derivative = 0;
  error = saved_set_value[AUTO] - saved_curr_value[AUTO];
  last_error = error;
  integral += error;
  derivative = error - last_error;
  fan_speed.setInit ((kP * error) + (kI * integral) + (kD * derivative));
}

void
StateHandler::updateSensorValues ()
{

  sensors_data[TEMPERATURE] = humidity.readT ();
  sensors_data[PRESSUREDAT] = _pressure->getPressure ();
  sensors_data[CO2] = co2.read ();
  state_timer->tickCounter (5);
  sensors_data[HUMIDITY] = humidity.readRH ();
}
