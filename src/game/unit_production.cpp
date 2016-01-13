#include "game/unit_production.h"

#include "game/events/unit_production_finish_event.h"
#include "game/unit_blueprint.h"

UnitProduction::UnitProduction(const UnitBlueprint& unit_blueprint)
	: std::enable_shared_from_this<UnitProduction>(),
	Updatable(),
	m_unit_blueprint(unit_blueprint),
	m_remaining_creation_duration_milliseconds(m_unit_blueprint.get_creation_duration_seconds() * 1000),
	m_remaining_boost_duration_seconds(0),
	m_is_finished(false),
	m_unit(new Unit(m_unit_blueprint)),
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
bool UnitProduction::is_finished() const {
	return m_is_finished;
}
std::shared_ptr<Unit> UnitProduction::get_unit() const {
	return m_unit;
}

void UnitProduction::boost_creation_duration(unsigned int boost_factor, unsigned int boost_duration_seconds) {
	m_boost_factor = boost_factor;
	m_remaining_boost_duration_seconds = boost_duration_seconds;
}

std::list<std::unique_ptr<Event>> UnitProduction::update(unsigned int elapsed_time_seconds) {
	if (m_is_finished) {
		return {};
	}

	if (m_remaining_boost_duration_seconds > 0) {
		unsigned int boost_duration_seconds = elapsed_time_seconds <= m_remaining_boost_duration_seconds ? elapsed_time_seconds : m_remaining_boost_duration_seconds;
		m_remaining_boost_duration_seconds -= boost_duration_seconds;
		elapsed_time_seconds -= boost_duration_seconds;
		unsigned int boosted_duration_milliseconds = boost_duration_seconds * m_boost_factor * 10;
		m_remaining_creation_duration_milliseconds -= boosted_duration_milliseconds < m_remaining_creation_duration_milliseconds ? boosted_duration_milliseconds : m_remaining_creation_duration_milliseconds;
	}
	if (elapsed_time_seconds > 0) {
		unsigned int elapsed_time_milliseconds = elapsed_time_seconds * 1000;
		m_remaining_creation_duration_milliseconds -= elapsed_time_milliseconds < m_remaining_creation_duration_milliseconds ? elapsed_time_milliseconds : m_remaining_creation_duration_milliseconds;
	}

	if (m_remaining_creation_duration_milliseconds == 0) {
		m_is_finished = true;

		std::list<std::unique_ptr<Event>> unit_production_finish_events;
		unit_production_finish_events.emplace_back(new UnitProductionFinishEvent(shared_from_this()));
		return unit_production_finish_events;
	}
	return {};
}
