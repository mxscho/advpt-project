#include "game/building.h"

#include "game/building_blueprint.h"

#include <cassert>

Building::Building(const BuildingBlueprint& building_blueprint)
	: Updatable(),
	m_building_blueprint(building_blueprint),
	m_current_unit_productions() {
}

const BuildingBlueprint& Building::get_building_blueprint() const {
	return m_building_blueprint;
}
const std::list<UnitProduction>& Building::get_current_unit_productions() const {
	return m_current_unit_productions;
}

bool Building::can_produce_unit(const UnitBlueprint& unit_blueprint) const {
	auto& producible_unit_blueprints = m_building_blueprint.get_producible_unit_blueprints();
	for (auto i = producible_unit_blueprints.begin(); i != producible_unit_blueprints.end(); ++i) {
		if (&i->get() == &unit_blueprint) {
			return m_current_unit_productions.size() < m_building_blueprint.get_max_concurrent_unit_production_count();
		}
	}
	return false;
}
const UnitProduction& Building::produce_unit(const UnitBlueprint& unit_blueprint) {
	assert(can_produce_unit(unit_blueprint));

	m_current_unit_productions.push_back(UnitProduction(unit_blueprint));
	return m_current_unit_productions.back();
}

void Building::update(unsigned int elapsed_time_seconds) {
	for (auto i = m_current_unit_productions.begin(); i != m_current_unit_productions.end(); ++i) {
		i->update(elapsed_time_seconds);
	}
}
