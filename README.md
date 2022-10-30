# ESP-Ventilation
A school project to control a simulated ventilation system based on measured CO2 levels <br>
**[Google drive folder](https://docs.google.com/document/d/12K4G12bVbDqZUrHPMkFk3nm1rfgFxZ1O2iIwMq2Nnus/edit?usp=sharing)**

## Embedded Systems Programming Ventilation Project

The objective of this project was to create a smart ventilation system that has the capability to display sensor data and let users set the ventilation fan speed manually, or let the system determine the fan speed in automatic operation by setting the target output air pressure to achieve. This documentation examines the overall system architecture in hardware and software, operation of the system, the development process and goes in detail of the technical implementation. As hardware implementation of this ventilation system is pre-determined, particular attention in this documentation is paid to the embedded software and communications implementation.

## Download latest release 
### [esp-vent-main v1.0.0](https://github.com/vas-dav/ESP-Ventilation/releases) (2022-10-31)
> Source code is safely building on **MCUXpresso IDE v11.5.0 [Build 7232] [2022-01-11]**
### Current functionality:
### Modes:
##### Manual:
```
- Buttons to inc/dec fan speed(0-10V) (show in percents 10% == 1V)
- Display fan speed + pressure.
- Fan updates at the same time as UI
- BTN1 inc voltage,  BTN2 dec voltage
  ```
##### Automatic:
```
- Buttons to inc/dec fan pressure level (0-120 Pa) (show in percents 10% == 1V)
- Pressure to inc/dec fan speed
- BTN1 inc pressure,  BTN2 dec pressure
```
##### Additional: 
```
- BTN3 switch between modes
- BTN4 show sensor values
  ```

## Latest Changes
* [Closed Pull-requests](https://github.com/vas-dav/ESP-Ventilation/pulls?q=is%3Apr+is%3Aclosed)

#### [Open issues](https://github.com/vas-dav/ESP-Ventilation/issues)

