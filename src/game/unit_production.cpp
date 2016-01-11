#include "game/unit_production.h"

#include "game/unit_blueprint.h"

UnitProduction::UnitProduction(const UnitBlueprint& unit_blueprint)
	: Updatable(),
	m_unit_blueprint(unit_blueprint),
	m_remaining_creation_duration_milliseconds(m_unit_blueprint.get_creation_duration_seconds() * 1000),
	m_remaining_boost_duration_seconds(0),
	m_boost_factor() {
}

const UnitBlueprint& UnitProduction::get_unit_blueprint() const {
	return m_unit_blueprint;
}
unsigned int UnitProduction::get_remaining_creation_duration_milliseconds() const {
	return m_remaining_creation_duration_milliseconds;
}
unsigned int UnitProduction::get_remaining_boost_duration_seconds() const {
	return m_remaining_boost_duration_seconds;
}

void UnitProduction::boost_creation_duration(unsigned int boost_factor, unsigned int boost_duration_seconds) {
	m_boost_factor = boost_factor;
	m_remaining_boost_duration_seconds = boost_duration_seconds;
}
Unit UnitProduction::spawn_unit() const {
	return Unit(m_unit_blueprint);
}

void UnitProduction::update(unsigned int elapsed_time_seconds) {
	if (m_remaining_boost_duration_seconds > 0) {
		unsigned int boost_duration_seconds = elapsed_time_seconds <= m_remaining_boost_duration_seconds ? elapsed_time_seconds : m_remaining_boost_duration_seconds;
		m_remaining_boost_duration_seconds -= boost_duration_seconds;
		elapsed_time_seconds -= boost_duration_seconds;
		m_remaining_creation_duration_milliseconds -= boost_duration_seconds * m_boost_factor * 10;
	}
	if (elapsed_time_seconds > 0) {
		m_remaining_creation_duration_milliseconds -= elapsed_time_seconds * 1000;
	}
	if (m_remaining_creation_duration_milliseconds < 0) {
		m_remaining_creation_duration_milliseconds = 0;
	}
}
