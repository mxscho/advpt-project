#include "game/unit.h"

#include "game/unit_blueprint.h"

#include <cassert>

Unit::Unit(const UnitBlueprint& unit_blueprint)
	: Identifiable(),
	Energetic(unit_blueprint),
	Mortal(unit_blueprint),
	m_unit_blueprint(unit_blueprint),
	m_is_building(false) {
}

const UnitBlueprint& Unit::get_unit_blueprint() const {
	return m_unit_blueprint;
}

bool Unit::is_building() const {
	return m_is_building;
}
void Unit::toggle_building() {
	m_is_building = !m_is_building;
}
std::list<std::unique_ptr<Event>> Unit::update(unsigned int elapsed_time_seconds) {
	std::list<std::unique_ptr<Event>> events;

	events.splice(events.end(), Energetic::update(elapsed_time_seconds));
	events.splice(events.end(), Mortal::update(elapsed_time_seconds));

	return events;
}
