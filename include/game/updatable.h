#pragma once 

class Updatable {
public:
	virtual void update(unsigned int elapsed_time_seconds) = 0;
};
