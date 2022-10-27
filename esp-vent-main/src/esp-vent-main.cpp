/*
===============================================================================
 Name        : main.c
 Author      : Vasily, Evgenii, Jaakko, Miisa
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "DigitalIoPin.h"
#include "Fan.h"
#include "LiquidCrystal.h"
#include "PressureWrapper.h"
#include "StateHandler/StateHandler.h"
#include "SwitchController.h"
#include "Timer.h"
#include "board.h"
#include "chip.h"
#include "common_control_values.h"
#include <cr_section_macros.h>

int
main (void)
{
  /* Board init */
  SystemCoreClockUpdate ();
  Board_Init ();

  /** LCD setup */
  Chip_RIT_Init (LPC_RITIMER);
  DigitalIoPin rs (0, 29, false, true, false);
  DigitalIoPin en (0, 9, false, true, false);
  DigitalIoPin d4 (0, 10, false, true, false);
  DigitalIoPin d5 (0, 16, false, true, false);
  DigitalIoPin d6 (1, 3, false, true, false);
  DigitalIoPin d7 (0, 0, false, true, false);
  LiquidCrystal lcd (&rs, &en, &d4, &d5, &d6, &d7);
  lcd.clear ();
  lcd.printOnLineOne (" ESP-VENT_MAIN ");

  /* Timers */
  Timer glob_time (ONE_K_HZ, true);
  /* Currently not used, but if we have time left,
   * quick button increment is needed
   * Timer switch_time (ONE_K_HZ, false);
   */

  /* Modbus Fan setup */
  ModbusMaster fan (1);
  fan.begin (9600);
  Fan propeller (new ModbusRegister (&fan, 0));

  /* Pressure sensor setup */
  PressureWrapper sens;
  glob_time.Sleep (1000);

  /* State Machine */
  StateHandler ventMachine (&lcd, &propeller, &sens, &glob_time);

  /** Control switches */
  DigitalIoPin b_up (0, 7, true, true, true); // A5
  SwitchController sw_up (&b_up, &ventMachine, BUTTON_CONTROL_UP);

  DigitalIoPin b_down (0, 6, true, true, true); // A4
  SwitchController sw_down (&b_down, &ventMachine, BUTTON_CONTROL_DOWN);

  DigitalIoPin b_toggle (0, 5, true, true, true); // A3
  SwitchController sw_toggle (&b_toggle, &ventMachine,
                              BUTTON_CONTROL_TOG_MODE);

  DigitalIoPin b_toggle_sens (1, 8, true, true, true); // A3
  SwitchController sw_toggle_sens (&b_toggle_sens, &ventMachine,
                              BUTTON_CONTROL_TOG_ACTIVE);
  /* Other declarations */
  int getcounterValue;
  while (1)
    {
      getcounterValue = glob_time.getCounter ();
      sw_up.listen ();
      sw_down.listen ();
      sw_toggle.listen ();
      sw_toggle_sens.listen();
      ventMachine.HandleState (Event (Event::eTick, getcounterValue));
      glob_time.tickCounter (1);
    }
  return 0;
}
