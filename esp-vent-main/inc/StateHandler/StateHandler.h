/*
 * StateHandler.h
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 *
 * Purpose of this class is to store and pass
 * the events of the current mode to further process.
 *
 *
 */

#ifndef STATE_HANDLER_H_
#define STATE_HANDLER_H_

#include "Counter.h"
#include "DigitalIoPin.h"
#include "Event.h"
#include "Fan.h"
#include "GMP252.h"
#include "HMP60.h"
#include "LiquidCrystal.h"
#include "PressureWrapper.h"
#include "Timer.h"
#include "common_control_values.h"
#include "nlohmann/json.hpp"

/** Buttons enumeration
 *
 * Current switch state is being passed
 * from main to StateHandler through
 * a keyEvent. Enumeration determines the state
 * of the particular button.
 */
enum _buttons
{
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

enum _bars
{
  /** 0-100 % */
  FAN_SPEED,
  /** 0-120 Pa */
  PRESSURE
};

enum _mode
{
  MANUAL,
  AUTO,
  SENSORS,
  ERROR_TIMEOUT
};

enum _sensors
{
  PRESSUREDAT,
  TEMPERATURE,
  HUMIDITY,
  CO2
};

class StateHandler;
typedef void (StateHandler::*state_pointer) (const Event &);

class StateHandler
{
public:
  StateHandler (LiquidCrystal *lcd, Fan *propeller, PressureWrapper *pressure,
                Timer *global);
  virtual ~StateHandler ();

  /** Get currently set pressure
   *
   * @return pressure in range of 0-120
   */
  unsigned int getSetPressure ();

  /** Get currently set FanSpeed
   *
   * @return speed in range of 0-100
   */
  unsigned int getSetSpeed ();

  /** Handle the given event of the current state
   *
   * @param event event to be handled in the current state
   */
  void HandleState (const Event &event);

private:
  state_pointer current;
  bool current_mode;
  Counter value[2] = { { 0, 100 }, { 0, 120 } };
  /* Motor of fan starts at value 90. probably because of some
   * weigh of fan, so voltage within range of 0-89 is not
   * sufficient to start motor.
   * TODO: Value 89 should be scaled to 0 at some point
   */
  Counter fan_speed = { 0, 1000 };
  /* Integral controller for PID. should be global, since it
   * accumulates error signals encountered since startup
   */
  int integral = 0;
  int saved_set_value[2] = { 0, 0 };
  int saved_curr_value[2] = { 0, 0 };
  int sensors_data[4] = { 0 };
  LiquidCrystal *_lcd;
  Fan *_propeller;
  PressureWrapper *_pressure;
  bool pressure_status;
  Timer *state_timer;
  /* CO2 sensor object */
  GMP252 co2;
  /* Humidity and temperature sensor object */
  HMP60 humidity;

  /** Display values on LCD depending on current mode
   *
   * MANUAL MODE: SPEED: XX% PRESSURE: XXPa
   *
   * AUTO MODE: P. SET: XXPa P. CURR: XXPa
   *
   * @param mode the current printing mode
   */
  void displaySet (size_t mode);

  /** Set a new curremt state
   * @param newstate new state to be set to current
   */
  void SetState (state_pointer newstate);

  /** Initialization state
   *
   * @param event event of the state
   */
  void stateInit (const Event &event);

  /** Manual state
   *
   * - set current speed
   * - print current pressure
   *
   * @param event event of the state
   */
  void stateManual (const Event &event);

  /** Automated state
   *
   * - print current pressure
   * - print set pressure
   * - inc/dec fan speed
   *
   * @param event
   */
  void stateAuto (const Event &event);

  /**
   * @brief Get pressure with the state
   *
   * @param event
   */
  void stateGetPressure (const Event &event);

  /** Handle button presses
   *
   * @param button current button
   */
  void handleControlButtons (uint8_t button);

  /** Handle the timer value of eTick event
   *
   * @param value current event value
   */
  void handleTickValue (int value);

  /** Save values to class' varibales
   *
   * @param eventValue value coming from an event
   * @param counterValue value of the inner Counter
   * @param mode current mode
   */
  void save (int eventValue, size_t mode);

  /** Calculates pid for fan control value
   *
   */
  void pid ();

  int fan_speed_normalized ();

  void updateSensorValues ();
};

#endif /* STATE_HANDLER_H_ */
