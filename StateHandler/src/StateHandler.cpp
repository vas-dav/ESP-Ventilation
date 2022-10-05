/*
 * StateHandler.cpp
 *
 *  Created on: Sep 21, 2022
 *      Author: tylen
 */

#include <StateHandler.h>

StateHandler::StateHandler() {
	// TODO Auto-generated constructor stub

}

StateHandler::~StateHandler() {
	// TODO Auto-generated destructor stub
}

int StateHandler::getSetPresuure(){
	//TODO
	return 0;
}

int StateHandler::getSetSpeed(){
	// TODO
	return 0;
}

void StateHandler::HandleState(const Event &event){
	(this->*current)(event);
}

void StateHandler::SetState(state_pointer newstate){
	(this->*current)(Event(Event::eExit));
	current = newstate;
	(this->*current)(Event(Event::eEnter));

}
