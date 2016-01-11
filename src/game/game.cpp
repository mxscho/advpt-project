#include "game/game.h"

#include <cassert>
#include <exception>

Game::Game(unsigned int mineral_count,
	unsigned int vespene_gas_count)
	: Updatable(),
	m_building_blueprints(),
	m_unit_blueprints(),
	m_mineral_count(mineral_count),
	m_vespene_gas_count(vespene_gas_count),
	m_buildings(),
	m_units(),
	m_morphing_unit_productions() {
}
Game::~Game() {
}

const std::list<BuildingBlueprint>& Game::get_building_blueprints() const {
	return m_building_blueprints;
}
const BuildingBlueprint& Game::find_building_blueprint_by_name(const std::string& name) const {
	for (auto i = m_building_blueprints.begin(); i != m_building_blueprints.end(); ++i) {
		if (i->get_name() == name) {
			return *i;
		}
	}
	throw std::out_of_range("Cannot find building blueprint with this name.");
}
void Game::add_building_blueprint(const BuildingBlueprint& building_blueprint) {
	m_building_blueprints.push_back(building_blueprint);
}
const std::list<UnitBlueprint>& Game::get_unit_blueprints() const {
	return m_unit_blueprints;
}
const UnitBlueprint& Game::find_unit_blueprint_by_name(const std::string& name) const {
	for (auto i = m_unit_blueprints.begin(); i != m_unit_blueprints.end(); ++i) {
		if (i->get_name() == name) {
			return *i;
		}
	}
	throw std::out_of_range("Cannot find unit blueprint with this name.");
}
void Game::add_unit_blueprint(const UnitBlueprint& unit_blueprint) {
	m_unit_blueprints.push_back(unit_blueprint);
}
unsigned int Game::get_mineral_count() const {
	return m_mineral_count;
}
unsigned int Game::get_vespene_gas_count() const {
	return m_vespene_gas_count;
}
const std::list<Building>& Game::get_buildings() const {
	return m_buildings;
}
void Game::add_building_by_name(const std::string& name) {
	m_buildings.push_back(Building(find_building_blueprint_by_name(name)));
}
const std::list<Unit>& Game::get_units() const {
	return m_units;
}
void Game::add_unit_by_name(const std::string& name) {
	m_units.push_back(Unit(find_unit_blueprint_by_name(name)));
}

unsigned int Game::get_supply_available() const {
	unsigned int supply_available = 0;
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		supply_available += i->get_building_blueprint().get_supply_provided();
	}
	return supply_available;
}
unsigned int Game::get_supply_used() const {
	unsigned int supply_used = 0;
	for (auto i = m_units.begin(); i != m_units.end(); ++i) {
		supply_used += i->get_unit_blueprint().get_supply_costs();
	}
	return supply_used;
}
unsigned int Game::get_remaining_supply() const {
	return get_supply_available() - get_supply_used();
}
void Game::set_worker_unit_allocation_function(const std::function<WorkerUnitAllocation(unsigned int)>& worker_unit_allocation_function) {
	m_worker_unit_allocation_function = worker_unit_allocation_function;
}
bool Game::can_produce_unit_by_name(const std::string& name) const {
	// TODO: Involve morphing & dependencies.

	const UnitBlueprint& unit_blueprint = find_unit_blueprint_by_name(name);
	if (unit_blueprint.get_mineral_costs() > m_mineral_count ||
		unit_blueprint.get_vespene_gas_costs() > m_vespene_gas_count) {
		return false;
	}

	if (get_remaining_supply() < unit_blueprint.get_supply_costs()) {
		return false;
	}

	return find_building_for_unit_production(unit_blueprint) != nullptr;
}
const UnitProduction& Game::produce_unit_by_name(const std::string& name) {
	assert(can_produce_unit_by_name(name));

	// TODO: Involve morphing & dependencies.

	const UnitBlueprint& unit_blueprint = find_unit_blueprint_by_name(name);
	m_mineral_count -= unit_blueprint.get_mineral_costs();
	m_vespene_gas_count -= unit_blueprint.get_vespene_gas_costs();
	return find_building_for_unit_production(unit_blueprint)->produce_unit(unit_blueprint);
}

void Game::update(unsigned int elapsed_time_seconds) {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		i->update(elapsed_time_seconds);
	}

	// TODO: Get finished buildings and units (from buildings and morphing)
	// TODO: Manage dependencies.
	// TODO: If zerg do automatic larva generation.
	// TODO: Update energy and resources.
	// TODO: Check life durations.
	// TODO: Allocate worker units.
}

const Building* Game::find_building_for_unit_production(const UnitBlueprint& unit_blueprint) const {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		if (i->can_produce_unit(unit_blueprint)) {
			return &(*i);
		}
	}
	return nullptr;
}
Building* Game::find_building_for_unit_production(const UnitBlueprint& unit_blueprint) {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		if (i->can_produce_unit(unit_blueprint)) {
			return &(*i);
		}
	}
	return nullptr;
}
