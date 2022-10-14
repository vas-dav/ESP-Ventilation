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
  Counter (int i, int up);
  void inc ();
  void dec ();
  int getCurrent ();
  void setInit (int i);
  ~Counter () = default;

private:
  int init;
  int up_lim;
};
#endif /* COUNTER_H_ */
