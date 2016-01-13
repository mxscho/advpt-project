#pragma once 

#include "game/events/event.h"

#include <memory>
#include <list>

class Updatable {
public:
	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) = 0;
};
