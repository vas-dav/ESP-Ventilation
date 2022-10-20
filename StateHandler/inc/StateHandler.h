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

#include "Counter.h"
#include "DigitalIoPin.h"
#include "Event.h"
#include "LiquidCrystal.h"
#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "GMP252.h"
#include "HMP60.h"

/** Buttons enumeration
 *
 * Current switch state is being passed
 * from main to StateHandler through
 * a keyEvent. Enumeration determines the state
 * of the particular button.
 * */

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
  AUTO
};

class StateHandler;
typedef void (StateHandler::*state_pointer) (const Event &);

class StateHandler
{
public:
  StateHandler (LiquidCrystal *lcd, ModbusRegister *A01);
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

  /** Display values on LCD depending on current mode
   *
   * MANUAL MODE: SPEED: XX% PRESSURE: XXPa
   *
   * AUTO MODE: P. SET: XXPa P. CURR: XXPa
   *
   * @param value1 value to be displayed on LCD line 0
   * @param value2 value to be displayed on LCD line 1
   */
  void displaySet (unsigned int value1, unsigned int value2);

  /** Handle the given event of the current state
   *
   * @param event event to be handled in the current state
   */
  void HandleState (const Event &event);

private:
  state_pointer current;
  /** Set a new curremt state
   * @param newstate new state to be set to current
   */
  void SetState (state_pointer newstate);
  bool current_mode;
  Counter value[2] = { { 0, 100 }, { 0, 120 } };
  /* motor of fan starts at value 90. probably because of some
   * weigh of fan, so voltage within range of 0-89 is not
   * sufficient to start motor.
   * TODO: Value 89 should be scaled to 0 at some point */
  Counter fan_speed = {20, 1000};
  /*integral controller for PID. should be global, since it
   * accumulates error signals encountered since startup*/
  int integral = 0;
  int saved_set_value[2] = { 0, 0 };
  int saved_curr_value[2] = { 0, 0 };
  LiquidCrystal *_lcd;
  ModbusRegister *A01;

  /* CO2 sensor object */
  GMP252 co2;

  /* Humidity and temperature sensor object */
  HMP60 humidity;

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

  /** Hnadle button presses
   *
   * @param button current button
   */
  void handleControlButtons (uint8_t button);

  /** Save values to class' varibales
   *
   * @param eventValue value coming from an event
   * @param counterValue value of the inner Counter
   * @param mode current mode
   */
  void save (int eventValue, int counterValue, size_t mode);

  /** Calculates pid for fan control value
   *
   */
  void pid ();
};

#endif /* STATE_HANDLER_H_ */
