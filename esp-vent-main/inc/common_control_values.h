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

#define ONE_K_HZ 1000

enum _global_values
{
  LCD_SIZE = 16,
  TIMER_GLOBAL_TIMEOUT = 120000,
  TIMER_SENSORS_TIMEOUT = 500,
  TIMER_PRESSURE_TIMEOUT = 150,
};

#endif /* COMMON_CONTROL_VALUES_H_ */
