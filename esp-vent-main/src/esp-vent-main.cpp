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
#include "StateHandler.h"

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

  DigitalIoPin b_up ();
  DigitalIoPin b_down ();
  DigitalIoPin b_toggle ();
  bool b_up_state = false, b_down_state = false, b_toggle_state = false;

  StateHandler ventMachine;

  while (1)
    {
      if (b_up.read ())
        b_up_state = true;
      if (!b_up.read () && b_up_state)
        {
          ventMachine.HandleState (Event (Event::eKey, BUTTON_CONTROL_UP));
          b_up_state = false;
        }
      if (b_down.read ())
        b_down_state = true;
      if (!b_down.read () && b_down_state)
        {
          ventMachine.HandleState (Event (Event::eKey, BUTTON_CONTROL_DOWN));
          b_down_state = false;
        }
      if (b_toggle.read ())
        b_toggle_state = true;
      if (!b_toggle.read () && b_toggle_state)
        {
          ventMachine.HandleState (
              Event (Event::eKey, BUTTON_CONTROL_TOG_MODE));
          b_toggle_state = false;
        }

      /**
       * TODO:
       * - Update current pressure to eTick
       */
      ventMachine.HandleState (Event (Event::eTick));
    }

  return 0;
}
