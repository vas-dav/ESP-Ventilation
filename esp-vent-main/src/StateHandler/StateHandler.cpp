/*
 * StateHandler.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#include "StateHandler/StateHandler.h"
#define PID 1

StateHandler::StateHandler (LiquidCrystal *lcd, ModbusRegister *A01,
                            PressureWrapper *pressure, Timer *global)
{
  this->_lcd = lcd;
  this->A01 = A01;
  this->pressure = pressure;
  this->state_timer = global;
  current = &StateHandler::stateInit;
  (this->*current) (Event (Event::eEnter));
  current_mode = MANUAL;
}

StateHandler::~StateHandler ()
{
  // TODO Auto-generated destructor stub
}

void
StateHandler::displaySet (size_t mode)
{
  char line_up[16] = { 0 };
  char line_down[16] = { 0 };

  switch (mode)
    {
    case MANUAL:
      snprintf (line_up, 16, "SPEED: %02d%", saved_set_value[current_mode]);
      snprintf (line_down, 16, "PRESSURE: %02dPa",
                saved_curr_value[current_mode]);
      break;
    case AUTO:
      snprintf (line_up, 16, "P. SET: %02dPa", saved_set_value[current_mode]);
      snprintf (line_down, 16, "P. CURR: %02dPa",
                saved_curr_value[current_mode]);
      break;
    case SENSORS:
      snprintf (line_up, 16, "PRE:%02d  TEM:%02d", sensors_data[PRESSUREDAT],
                sensors_data[TEMPERATURE]);
      snprintf (line_down, 16, "HUM:%02d  CO2:%02d", sensors_data[HUMIDITY],
                sensors_data[CO2]);
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
      this->A01->write (fan_speed_normalized ());
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (event.value % 5000 == 0)
        {
          SetState (&StateHandler::stateSensors);
          displaySet (SENSORS);
        }
      if (event.value % 500 == 0)
        {
          SetState (&StateHandler::stateGetPressure);
          break;
        }
    }
}

void
StateHandler::stateAuto (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      this->A01->write (fan_speed.getCurrent ());
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (event.value % 2 == 0)
        {
          SetState (&StateHandler::stateGetPressure);
        }
      if (event.value % 150 == 0)
        {
          SetState (&StateHandler::stateSensors);
          //				displaySens ();
        }
      pid ();
      this->A01->write (fan_speed.getCurrent ());
      break;
    }
}

void
StateHandler::stateSensors (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      sensors_data[TEMPERATURE] = humidity.readT ();
      sensors_data[PRESSUREDAT] = pressure->getPressure ();
      sensors_data[CO2] = co2.read ();
      state_timer->Sleep(10);
      sensors_data[HUMIDITY] = humidity.readRH ();
      //      displaySens ();
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      //      save (pressure->getPressure (), ((current_mode) ? AUTO :
      //      MANUAL));
      SetState (current_mode ? &StateHandler::stateAuto
                             : &StateHandler::stateManual);
      break;
    }
}

void
StateHandler::stateGetPressure (const Event &event)
{
  switch (event.type)
    {
    case Event::eEnter:
      pressure_status = pressure->isAwake ();
      break;
    case Event::eExit:
      break;
    case Event::eKey:
      handleControlButtons (event.value);
      break;
    case Event::eTick:
      if (!pressure_status)
        {
          pressure->wakeUp ();
          break;
        }
      save (pressure->getPressure (), ((current_mode) ? AUTO : MANUAL));
      SetState (current_mode ? &StateHandler::stateAuto
                             : &StateHandler::stateManual);
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

void
StateHandler::save (int eventValue, size_t mode)
{
  int counterValue = value[mode].getCurrent ();
  if (saved_curr_value[mode] != eventValue
      || saved_set_value[mode] != counterValue)
    {
      saved_curr_value[mode] = eventValue;
      saved_set_value[mode] = counterValue;
      displaySet ((current_mode) ? AUTO : MANUAL);
    }
}

int
StateHandler::fan_speed_normalized ()
{
  int speed = value[MANUAL].getCurrent ();
  if (speed <= 92)
    speed += 8;
  return speed * 10;
}

void
StateHandler::pid ()
{
  float kP = 1.0, kI = 0.1, kD = 0.125;
  int error = 0, last_error = 0, derivative = 0;
  error = saved_set_value[AUTO] - saved_curr_value[AUTO];
  last_error = error;
  integral += error;
  derivative = error - last_error;
  fan_speed.setInit ((kP * error) + (kI * integral) + (kD * derivative));
}
