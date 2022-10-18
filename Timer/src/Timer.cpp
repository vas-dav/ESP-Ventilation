/*
 * Timer.cpp
 *
 *  Created on: Oct 14, 2022
 *      Author: tylen
 */

#include <Timer.h>

extern "C"
{
  void
  SysTick_Handler (void)
  {
	systicks++;
    if (timer > 0)
      timer--;
  }
}

Timer::Timer (uint32_t freq) : freq (freq)
{
  Chip_Clock_SetSysTickClockDiv (1);
  uint32_t sysTickRate = Chip_Clock_GetSysTickClockRate ();
  SysTick_Config (sysTickRate / freq);
  counter = 0;
  timer = 0;
}

Timer::~Timer ()
{
  // TODO Auto-generated destructor stub
}

void
Timer::tickCounter (int ms)
{
  if (counter >= INT_MAX)
    {
      counter = 0;
    }
  counter++;
  Sleep (ms);
}

void
Timer::Sleep (int ms)
{
  timer = ms;
  while (timer > 0)
    {
      __WFI ();
    }
}

int
Timer::getCounter ()
{
  return counter.load (std::memory_order_relaxed);
}

void
Timer::resetCounter ()
{
  counter.store (0, std::memory_order_relaxed);
}

uint32_t millis() {
	return systicks;
}
