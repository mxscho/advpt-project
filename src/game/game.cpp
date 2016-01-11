#include "game/game.h"

#include "game/worker_unit_allocation.h"

#include <algorithm>
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
	m_satisfied_dependencies(),
	m_morphing_unit_productions(),
	m_worker_unit_allocation_function([](unsigned int worker_count) { return WorkerUnitAllocation(worker_count, 0); }),
	m_current_worker_unit_allocation(0, 0) {
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
BuildingBlueprint& Game::find_building_blueprint_by_name(const std::string& name) {
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
UnitBlueprint& Game::find_unit_blueprint_by_name(const std::string& name) {
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
bool Game::can_produce_units_by_names(const std::list<std::string>& names) const {
	assert(names.size() > 0);

	std::list<std::reference_wrapper<const UnitBlueprint>> unit_blueprints;
	for (auto i = names.begin(); i != names.end(); ++i) {
		unit_blueprints.push_back(find_unit_blueprint_by_name(*i));
	}

	const UnitBlueprint& unit_blueprint = unit_blueprints.front();
	if (unit_blueprints.size() == 1 && find_building_for_unit_production(unit_blueprint)) {
		// Produce unit in building.

		// Check resources.
		if (unit_blueprint.get_mineral_costs() > m_mineral_count ||
			unit_blueprint.get_vespene_gas_costs() > m_vespene_gas_count) {
			return false;
		}

		// Check supply.
		if (get_remaining_supply() < unit_blueprint.get_supply_costs()) {
			return false;
		}

		// Check dependencies.
		auto& dependency_blueprints = unit_blueprint.get_dependency_blueprints();
		for (auto i_dependency_blueprint = dependency_blueprints.begin(); i_dependency_blueprint != dependency_blueprints.end(); ++i_dependency_blueprint) {
			bool is_dependency_satisfied = false;
			for (auto i_satisfied_dependency = m_satisfied_dependencies.begin(); i_satisfied_dependency != m_satisfied_dependencies.end(); ++i_satisfied_dependency) {
				if (&i_dependency_blueprint->get() == &i_satisfied_dependency->get()) {
					is_dependency_satisfied = true;
				}
			}
			if (!is_dependency_satisfied) {
				return false;
			}
		}
		return true;
	}

	const Unit* unit = find_unit_for_unit_productions(unit_blueprints);
	if (unit) {
		// Morph units from unit.

		unsigned int mineral_costs = 0;
		unsigned int vespene_gas_costs = 0;
		int supply_costs = 0;
		for (auto i_unit_blueprint = unit_blueprints.begin(); i_unit_blueprint != unit_blueprints.end(); ++i_unit_blueprint) {
			mineral_costs += i_unit_blueprint->get().get_mineral_costs();
			vespene_gas_costs += i_unit_blueprint->get().get_vespene_gas_costs();
			supply_costs += i_unit_blueprint->get().get_supply_costs();

			// Check dependencies.
			auto& dependency_blueprints = i_unit_blueprint->get().get_dependency_blueprints();
			for (auto i_dependency_blueprint = dependency_blueprints.begin(); i_dependency_blueprint != dependency_blueprints.end(); ++i_dependency_blueprint) {
				bool is_dependency_satisfied = false;
				for (auto i_satisfied_dependency = m_satisfied_dependencies.begin(); i_satisfied_dependency != m_satisfied_dependencies.end(); ++i_satisfied_dependency) {
					if (&i_dependency_blueprint->get() == &i_satisfied_dependency->get()) {
						is_dependency_satisfied = true;
					}
				}
				if (!is_dependency_satisfied) {
					return false;
				}
			}
		}
		supply_costs -= unit->get_unit_blueprint().get_supply_costs();
		if (supply_costs < 0) {
			supply_costs = 0;
		}

		// Check resources.
		if (mineral_costs > m_mineral_count ||
			vespene_gas_costs > m_vespene_gas_count) {
			return false;
		}

		// Check supply.
		if (get_remaining_supply() < static_cast<unsigned int>(supply_costs)) {
			return false;
		}
		return true;
	}
	return false;
}
std::list<std::reference_wrapper<const UnitProduction>> Game::produce_units_by_names(const std::list<std::string>& names) {
	assert(can_produce_units_by_names(names));

	// TODO: Involve morphing.

	std::list<std::reference_wrapper<const UnitBlueprint>> unit_blueprints;
	for (auto i = names.begin(); i != names.end(); ++i) {
		unit_blueprints.push_back(find_unit_blueprint_by_name(*i));
	}
	if (unit_blueprints.size() == 1) {
		const UnitBlueprint& unit_blueprint = unit_blueprints.front();
		Building* building = find_building_for_unit_production(unit_blueprint);
		if (building) {
			// Produce unit in building.
			m_mineral_count -= unit_blueprint.get_mineral_costs();
			m_vespene_gas_count -= unit_blueprint.get_vespene_gas_costs();
			return { building->produce_unit(unit_blueprint) };
		}
	}

	const Unit* unit = find_unit_for_unit_productions(unit_blueprints);
	if (unit) {
		// Morph units from unit.
		m_units.remove_if([&unit](const Unit& unit_b) { return &unit_b == unit; });
		std::list<std::reference_wrapper<const UnitProduction>> unit_productions;
		for (auto i_unit_blueprint = unit_blueprints.begin(); i_unit_blueprint != unit_blueprints.end(); ++i_unit_blueprint) {
			m_mineral_count -= i_unit_blueprint->get().get_mineral_costs();
			m_vespene_gas_count -= i_unit_blueprint->get().get_vespene_gas_costs();
			m_morphing_unit_productions.push_back(UnitProduction(i_unit_blueprint->get()));
			unit_productions.push_back(m_morphing_unit_productions.front());
		}
		return unit_productions;
	}

	// This should not occur due to the assertion at the beginning.
	throw std::exception();
}

void Game::update(unsigned int elapsed_time_seconds) {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		i->update(elapsed_time_seconds);
	}

	// TODO: Get finished buildings and units (from buildings and morphing) and add them to satisfied dependencies.
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
const Unit* Game::find_unit_for_unit_productions(const std::list<std::reference_wrapper<const UnitBlueprint>>& unit_blueprints) const {
	for (auto i = m_units.begin(); i != m_units.end(); ++i) {
		if (i->can_morph<UnitBlueprint>(unit_blueprints)) {
			return &(*i);
		}
	}
	return nullptr;
}
void Game::update_worker_unit_allocation() {
	unsigned int worker_count = std::count_if(m_units.begin(), m_units.end(), [](const Unit& unit) {
		const UnitBlueprint& unit_blueprint = unit.get_unit_blueprint();
		return unit_blueprint.get_mineral_collection_rate() != 0 &&
			unit_blueprint.get_vespene_gas_collection_rate() != 0;
	});
	m_current_worker_unit_allocation = m_worker_unit_allocation_function(worker_count);
}
