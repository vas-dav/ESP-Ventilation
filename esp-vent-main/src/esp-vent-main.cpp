/*
===============================================================================
 Name        : main.c
 Author      : Vasily, Evgenii, Jaakko, Miisa
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined(__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "DigitalIoPin.h"
#include "LiquidCrystal.h"
#include "StateHandler.h"
#include "SwitchController.h"
#include "Timer.h"

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int
main (void)
{

#if defined(__USE_LPCOPEN)
  // Read clock settings and update SystemCoreClock variable
  SystemCoreClockUpdate ();
#if !defined(NO_BOARD_LIB)
  // Set b_up_state and initialize all required blocks and
  // functions related to the board hardware
  Board_Init ();
  // Set the LED to the state of "On"
  Board_LED_Set (0, true);
#endif
#endif
  /** Lcd & stateHandler */
  Chip_RIT_Init (LPC_RITIMER);
  Timer glob_time;
  DigitalIoPin rs (0, 29, false, true, false);
  DigitalIoPin en (0, 9, false, true, false);
  DigitalIoPin d4 (0, 10, false, true, false);
  DigitalIoPin d5 (0, 16, false, true, false);
  DigitalIoPin d6 (1, 3, false, true, false);
  DigitalIoPin d7 (0, 0, false, true, false);
  LiquidCrystal lcd (&rs, &en, &d4, &d5, &d6, &d7);
  StateHandler ventMachine (&lcd);
  //
  lcd.setCursor (0, 0);
  lcd.print ("Test");

  /** Common pins */
  DigitalIoPin b_up (0, 7, true, true, true); // A5
  SwitchController sw_up (&b_up, &glob_time, &ventMachine, BUTTON_CONTROL_UP);

  DigitalIoPin b_down (0, 6, true, true, true); // A4
  SwitchController sw_down (&b_down, &glob_time, &ventMachine,
                            BUTTON_CONTROL_DOWN);

  DigitalIoPin b_toggle (0, 5, true, true, true); // A3
  SwitchController sw_toggle (&b_toggle, &glob_time, &ventMachine,
                              BUTTON_CONTROL_TOG_MODE);

  int16_t pressure = 1;

  while (1)
    {

      sw_up.listen ();
      sw_down.listen ();
      sw_toggle.listen ();
      /**
       * TODO:
       * - Update current pressure to eTick
       */
      ventMachine.HandleState (Event (Event::eTick, pressure));
      glob_time.tickCounter (1);
    }

  return 0;
}
