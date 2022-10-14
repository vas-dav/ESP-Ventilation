/*
 * Event.h
 *
 *  Created on: Oct 5, 2022
 *      Author: tylen
 */

#ifndef EVENT_H_
#define EVENT_H_

class Event {
 public:
  virtual ~Event(){};

  enum eventType {
    /** Start of the event */
    eEnter,
    /** End of the event*/
    eExit,
    /** Button toggle event type (has values:
     * temperature or button) */
    eKey,
    /** Time event */
    eTick
  };
  Event(eventType e = eTick, uint8_t b = 0, int t = 0)
      : type(e), button(b), temp(t){};
  eventType type;
  uint8_t button;
  int temp;
};

#endif /* EVENT_H_ */
