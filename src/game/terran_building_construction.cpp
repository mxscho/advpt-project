#include "game/terran_building_construction.h"

#include <cassert>

TerranBuildingConstruction::TerranBuildingConstruction(const BuildingBlueprint& building_blueprint, Unit& builder_unit)
	: BuildingConstruction(building_blueprint),
	m_builder_unit(builder_unit) {
	assert(!m_builder_unit.is_building());

	m_builder_unit.toggle_building();
}

const Unit& TerranBuildingConstruction::get_builder_unit() const {
	return m_builder_unit;
}

std::list<std::unique_ptr<Event>> TerranBuildingConstruction::update(unsigned int elapsed_time_seconds) {
	bool was_finished = m_is_finished;
	std::list<std::unique_ptr<Event>> events = BuildingConstruction::update(elapsed_time_seconds);
	if (!was_finished && m_is_finished) {
		m_builder_unit.toggle_building();
	}
	return events;
}
