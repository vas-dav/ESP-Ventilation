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
#include "PressureWrapper.h"
#include "StateHandler/StateHandler.h"
#include "SwitchController.h"
#include "Timer.h"
#include "MQTTPacket.h"
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
  Timer switch_time (false);
  DigitalIoPin rs (0, 29, false, true, false);
  DigitalIoPin en (0, 9, false, true, false);
  DigitalIoPin d4 (0, 10, false, true, false);
  DigitalIoPin d5 (0, 16, false, true, false);
  DigitalIoPin d6 (1, 3, false, true, false);
  DigitalIoPin d7 (0, 0, false, true, false);
  LiquidCrystal lcd (&rs, &en, &d4, &d5, &d6, &d7);
  //
  lcd.setCursor (0, 0);
  lcd.print ("Vent-Machine");

  /* FAN object */
  ModbusMaster fan (1);
  fan.begin (9600);
  ModbusRegister A01 (&fan, 0);
  //  ModbusRegister DI1(&fan, 4, false);

  PressureWrapper sens;
  glob_time.Sleep (1000);

  StateHandler ventMachine (&lcd, &A01, &sens, &glob_time);
  /** Common pins */
  DigitalIoPin b_up (0, 7, true, true, true); // A5
  SwitchController sw_up (&b_up, &ventMachine, BUTTON_CONTROL_UP);

  DigitalIoPin b_down (0, 6, true, true, true); // A4
  SwitchController sw_down (&b_down, &ventMachine, BUTTON_CONTROL_DOWN);

  DigitalIoPin b_toggle (0, 5, true, true, true); // A3
  SwitchController sw_toggle (&b_toggle, &ventMachine,
                              BUTTON_CONTROL_TOG_MODE);

  while (1)
    {

      sw_up.listen ();
      sw_down.listen ();
      sw_toggle.listen ();
      ventMachine.HandleState (Event (Event::eTick, glob_time.getCounter ()));
      glob_time.tickCounter (1);
    }

  return 0;
}

#if 0  // example of opening a plain socket
void socketTest()
{

	esp_socket(SSID, PASSWORD);

	const char *http_request = "GET / HTTP/1.0\r\n\r\n"; // HTTP requires cr-lf to end a line

	for(int i = 0; i < 2; ++i) {
		printf("\nopen socket\n");
		esp_connect(1,  "www.metropolia.fi", 80);
		printf("\nsend request\n");
		esp_write(1, http_request, strlen(http_request));

		uint32_t now = get_ticks();
		printf("\nreply:\n");

		while(get_ticks() - now < 3000) {
			char buffer[64];
			memset(buffer, 0, 64);
			if(esp_read(1, buffer, 63) > 0) {
				fputs(buffer,stdout);
			}
		}
		esp_close(1);

		printf("\nsocket closed\n");
	}

}
#endif


