/*
 * Counter.h
 *
 *  Created on: Sep 1, 2022
 *      Author: tylen
 */

#ifndef COUNTER_H_
#define COUNTER_H_

class Counter
{

public:
  Counter (unsigned int i, unsigned int up);
  void inc ();
  void dec ();
  unsigned int getCurrent ();
  void setInit (unsigned int i);
  ~Counter () = default;

private:
  unsigned int init;
  unsigned int up_lim;
  unsigned int down_lim;
};
#endif /* COUNTER_H_ */
