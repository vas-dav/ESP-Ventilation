/*
 * common_control_values.h
 *
 *  Created on: Oct 26, 2022
 *      Author: tylen
 */

#ifndef COMMON_CONTROL_VALUES_H_
#define COMMON_CONTROL_VALUES_H_

#define SSID "SmartIotMQTT"
#define PASSWORD "SmartIot"
#define BROKER_IP "192.168.1.254"
#define BROKER_PORT 1883

enum _global_values
{
  LCD_SIZE = 16,
  TIMER_GLOBAL_TIMEOUT = 15000,
  TIMER_SENSORS_TIMEOUT = 5000,
  TIMER_PRESSURE_TIMEOUT = 500,
  TIMER_ERROR_VALUE = -255,
};

#endif /* COMMON_CONTROL_VALUES_H_ */
