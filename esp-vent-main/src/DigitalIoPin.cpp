/*
 * DigitalIoPin.cpp
 *
 *  Created on: Aug 29, 2022
 *      Author: Vasily Davydov
 */

#include "DigitalIoPin.h"

DigitalIoPin::DigitalIoPin (int port, int pin, bool input, bool pullup,
                            bool invert)
{
  assert ((port <= UINT8_MAX_VALUE) && (pin <= UINT8_MAX_VALUE));
  _io._port = (uint8_t)port;
  _io._pin = (uint8_t)pin;
  _io._input = input;
  _io._pullup = pullup;
  _io._invert = invert;
  _io.IOCON_mode = IOCON_MODE_INACT;
  _io.IOCON_inv = IOCON_FUNC0;
  setIoPin ();
}

DigitalIoPin::~DigitalIoPin ()
{
}

void
DigitalIoPin::setIoPin ()
{
  bool direction = true;
  if (_io._input)
    {
      direction = false;
      _io.IOCON_mode = IOCON_MODE_PULLUP;
      if (!_io._pullup)
        {
          _io.IOCON_mode = IOCON_MODE_PULLDOWN;
        }
      if (_io._invert)
        {
          _io.IOCON_inv = IOCON_INV_EN;
        }
    }
  Chip_IOCON_PinMuxSet (LPC_IOCON, _io._port, _io._pin,
                        (_io.IOCON_mode | _io.DigitalEn | _io.IOCON_inv));
  /**	False direction equals input */
  Chip_GPIO_SetPinDIR (LPC_GPIO, _io._port, _io._pin, direction);
}

bool
DigitalIoPin::read ()
{
	bool state = (Chip_GPIO_GetPinState (LPC_GPIO, _io._port, _io._pin));
	return (_io._invert && !_io._input) ? !state : state;
}

void
DigitalIoPin::write (bool value)
{
  assert (!(_io._input));
  Chip_GPIO_SetPinState (LPC_GPIO, _io._port, _io._pin, ((_io._invert) ? !value : value));
}

