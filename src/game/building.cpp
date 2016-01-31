#include "game/building.h"

#include "game/building_blueprint.h"

#include <algorithm>
#include <cassert>
#include <iterator>

Building::Building(const BuildingBlueprint& building_blueprint)
	: Identifiable(),
	Energetic(building_blueprint),
	Mortal(building_blueprint),
	m_building_blueprint(building_blueprint),
	m_current_unit_productions() {
}
Building::Building(const Building& building)
	: Identifiable(),
	Energetic(building.m_building_blueprint),
	Mortal(building.m_building_blueprint),
	m_building_blueprint(building.m_building_blueprint),
	m_current_unit_productions() {
}

const BuildingBlueprint& Building::get_building_blueprint() const {
	return m_building_blueprint;
}
const std::list<std::shared_ptr<UnitProduction>>& Building::get_current_unit_productions() const {
	return m_current_unit_productions;
}

bool Building::can_morph(const BuildingBlueprint& building_blueprint) const {
	if (m_current_unit_productions.size() > 0) {
		return false;
	}

	auto& morphable_building_blueprints = m_building_blueprint.get_morphable_building_blueprints();
	for (auto i = morphable_building_blueprints.begin(); i != morphable_building_blueprints.end(); ++i) {
		if (&i->get() == &building_blueprint) {
			return true;
		}
	}
	return false;
}
bool Building::can_produce_unit(const UnitBlueprint& unit_blueprint) const {
	auto& producible_unit_blueprints = m_building_blueprint.get_producible_unit_blueprints();
	for (auto i = producible_unit_blueprints.begin(); i != producible_unit_blueprints.end(); ++i) {
		if (&i->get() == &unit_blueprint) {
			return !is_working_to_capacity();
		}
	}
	return false;
}
std::shared_ptr<UnitProduction> Building::produce_unit(const UnitBlueprint& unit_blueprint) {
	assert(can_produce_unit(unit_blueprint));

	m_current_unit_productions.emplace_back(new UnitProduction(unit_blueprint, *this));
	return m_current_unit_productions.back();
}

std::list<std::unique_ptr<Event>> Building::update(unsigned int elapsed_time_seconds) {
	std::list<std::unique_ptr<Event>> events;

	events.splice(events.end(), Energetic::update(elapsed_time_seconds));
	events.splice(events.end(), Mortal::update(elapsed_time_seconds));
	auto i = m_current_unit_productions.begin();
	while (i != m_current_unit_productions.end()) {
		events.splice(events.end(), (*i)->update(elapsed_time_seconds));
		if ((*i)->is_finished()) {
			i = m_current_unit_productions.erase(i);
		} else {
			++i;
		}
	}

	return events;
}

bool Building::is_working_to_capacity() const {
	std::list<std::shared_ptr<const UnitProduction>> current_unit_productions_copy;
	std::copy_if(m_current_unit_productions.begin(), m_current_unit_productions.end(), std::back_inserter(current_unit_productions_copy), [](const std::shared_ptr<const UnitProduction>& unit_production) {
		return !unit_production->is_finished();
	});
	return current_unit_productions_copy.size() >= m_building_blueprint.get_max_concurrent_unit_production_count();
}
