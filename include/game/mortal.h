#pragma once 

#include "game/updatable.h"

#include <list>
#include <memory>

class Blueprint;
class Event;

class Mortal: public Updatable {
public:
	explicit Mortal(const Blueprint& blueprint);

	bool is_immortal() const;
	unsigned int get_remaining_life_duration_seconds() const;

	bool is_dead() const;
	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	const Blueprint& m_blueprint;
	bool m_is_immortal;
	unsigned int m_remaining_life_duration_seconds;
};
