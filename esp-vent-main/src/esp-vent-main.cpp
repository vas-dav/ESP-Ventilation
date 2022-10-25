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
#include <string.h>
#include "DigitalIoPin.h"
#include "LiquidCrystal.h"
#include "PressureWrapper.h"
#include "StateHandler/StateHandler.h"
#include "SwitchController.h"
#include "Timer.h"
#include "systick.h"
#include <MQTT/MQTTPacket.h>
#include "../inc/MQTT/esp8266_socket.h"

#include <cr_section_macros.h>

#define SSID        "SmartIotMQTT"
#define PASSWORD    "SmartIot"
#define BROKER_IP   "192.168.1.254"
#define BROKER_PORT  1883

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

//Status message example:
//{
//"nr": 96,
//"speed": 18,
//"setpoint": 18,
//"pressure": 5,
//"auto": false,
//"error": false,
//"co2": 300,
//"rh": 37,
//"temp": 20
//}

void socket_exchange(int pressure,int co2, int temperature, int speed, int setpoint, int rh, int count, bool mode, bool err)
{
	esp_socket(SSID, PASSWORD);
	std::string str_mode = mode? "true" : "false";
	std::string str_err = err? "true" : "false";

	const char *push_data = "{\"nr\":96,\"speed\":18,\"setpoint\":18,\"pressure\":5,\"auto\":false,\"error\":false,\"co2\": 300,\"rh\":37,\"temp\":20}";

	for(int i = 0; i < 2; ++i) {
		esp_connect(1,  "http://localhost:", 3000);
		//push data to server
		esp_write(1, push_data, strlen(push_data));
		uint32_t now = get_ticks();
		//response from server
		while(get_ticks() - now < 3000) {
			char buffer[64];
			memset(buffer, 0, 64);
			if(esp_read(1, buffer, 63) > 0) {
				//fputs(buffer,stdout); << data needs to be parsed still
			}
		}
		//socket closure
		esp_close(1);
	}

}


