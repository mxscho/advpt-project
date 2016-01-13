#pragma once 

#include "game/updatable.h"

#include <list>
#include <memory>

class Blueprint;
class Event;

class Energetic: public Updatable {
public:
	static const unsigned int c_energy_increase_rate;

	explicit Energetic(const Blueprint& blueprint);

	unsigned int get_energy();

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	const Blueprint& m_blueprint;
	unsigned int m_energy;
};
