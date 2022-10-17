/*
 * Event.h
 *
 *  Created on: Oct 5, 2022
 *      Author: tylen
 */

#ifndef EVENT_H_
#define EVENT_H_

class Event
{
public:
  virtual ~Event (){};

  enum eventType
  {
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
  Event (eventType e = eTick, int val = 0) : type (e), value (val){};
  eventType type;
  int value;
};

#endif /* EVENT_H_ */
