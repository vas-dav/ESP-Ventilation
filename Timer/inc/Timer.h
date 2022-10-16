/*
 * Timer.h
 *
 *  Created on: Oct 14, 2022
 *      Author: tylen
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "board.h"
#include <atomic>
#include <climits>

static volatile std::atomic_int timer;

extern "C"
{
  /**
   * @brief Handle interrupt from SysTick timer
   * @return Nothing
   */
  void SysTick_Handler (void);
}

class Timer
{
public:
  /**
   * @brief Initalize the systick configuration with your frequency
   *
   */
  Timer (uint32_t freq = 1000);
  virtual ~Timer ();

  /**
   * @brief Tick the counter.
   *
   * Counter is incremented by one every tick,
   * if it gets over the INT_MAX (see limits.h),
   * the counter rolls up back to zero and starts
   * over.
   *
   *
   * @param ms Counter ticking frequency is provided in milliseconds
   */
  void tickCounter (int ms);

  /**
   * @brief Get the current counter value
   *
   * @return int counter value
   */
  int getCounter ();

  /**
   * @brief Set counter to 0.
   *
   */
  void resetCounter ();

  /**
   * @brief Sleep for amount of time
   *
   * Time is either in ms or in sec, defined
   * by systickInit_xx()
   *
   * @param ms milliseconds
   */
  void Sleep (int ms);

private:
  volatile std::atomic_int counter;
  uint32_t freq;
};

#endif /* TIMER_H_ */
