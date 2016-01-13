#include "game/mortal.h"

#include "game/blueprint.h"

#include <cassert>

Mortal::Mortal(const Blueprint& blueprint)
	: m_blueprint(blueprint),
	m_is_immortal(m_blueprint.get_life_duration_seconds() == 0),
	m_remaining_life_duration_seconds(m_blueprint.get_life_duration_seconds()) {
}

bool Mortal::is_immortal() const {
	return m_is_immortal;
}
unsigned int Mortal::get_remaining_life_duration_seconds() const {
	assert(!m_is_immortal);

	return m_remaining_life_duration_seconds;
}

bool Mortal::is_dead() const {
	return !m_is_immortal && m_remaining_life_duration_seconds == 0;
}

std::list<std::unique_ptr<Event>> Mortal::update(unsigned int elapsed_time_seconds) {
	m_remaining_life_duration_seconds -= elapsed_time_seconds < m_remaining_life_duration_seconds ? elapsed_time_seconds : m_remaining_life_duration_seconds;
	return {};
}
