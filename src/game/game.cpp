#include "game/game.h"

#include "game/building_construction.h"
#include "game/events/building_construction_start_event.h"
#include "game/events/building_construction_finish_event.h"
#include "game/events/unit_production_start_event.h"
#include "game/events/unit_production_finish_event.h"
#include "game/worker_unit_allocation.h"

#include <algorithm>
#include <cassert>
#include <exception>
#include <utility>
#include <iterator>

const std::function<WorkerUnitAllocation(const Game&, const std::list<std::shared_ptr<Unit>>&)> Game::c_default_worker_allocation_function = [](const Game& game, const std::list<std::shared_ptr<Unit>>& worker_units) {
	std::list<std::shared_ptr<Unit>> mineral_collecting_worker_units;
	std::copy_if(worker_units.begin(), worker_units.end(), std::back_inserter(mineral_collecting_worker_units), [](const std::shared_ptr<Unit>& worker_unit) {
		return worker_unit->get_unit_blueprint().get_mineral_collection_rate() != 0;
	});
	return WorkerUnitAllocation(mineral_collecting_worker_units, {});
};

Game::Game(unsigned int mineral_count, unsigned int vespene_gas_count)
	: Updatable(),
	m_buildings(),
	m_units(),
	m_raw_mineral_count(mineral_count * 1000),
	m_raw_vespene_gas_count(vespene_gas_count * 1000),
	m_current_building_constructions(),
	m_building_blueprints(),
	m_unit_blueprints(),
	m_worker_unit_allocation({}, {}),
	m_satisfied_dependencies(),
	m_morphing_unit_productions(),
	m_worker_unit_allocation_function(c_default_worker_allocation_function),
	m_collected_events() {
}
Game::~Game() {
}

const std::list<BuildingBlueprint>& Game::get_building_blueprints() const {
	return m_building_blueprints;
}
const BuildingBlueprint* Game::find_building_blueprint_by_name(const std::string& name) const {
	for (auto i = m_building_blueprints.begin(); i != m_building_blueprints.end(); ++i) {
		if (i->get_name() == name) {
			return &(*i);
		}
	}
	return nullptr;
}
BuildingBlueprint* Game::find_building_blueprint_by_name(const std::string& name) {
	for (auto i = m_building_blueprints.begin(); i != m_building_blueprints.end(); ++i) {
		if (i->get_name() == name) {
			return &(*i);
		}
	}
	return nullptr;
}
void Game::add_building_blueprint(const BuildingBlueprint& building_blueprint) {
	m_building_blueprints.push_back(building_blueprint);
}
const std::list<UnitBlueprint>& Game::get_unit_blueprints() const {
	return m_unit_blueprints;
}
const UnitBlueprint* Game::find_unit_blueprint_by_name(const std::string& name) const {
	for (auto i = m_unit_blueprints.begin(); i != m_unit_blueprints.end(); ++i) {
		if (i->get_name() == name) {
			return &(*i);
		}
	}
	return nullptr;
}
UnitBlueprint* Game::find_unit_blueprint_by_name(const std::string& name) {
	for (auto i = m_unit_blueprints.begin(); i != m_unit_blueprints.end(); ++i) {
		if (i->get_name() == name) {
			return &(*i);
		}
	}
	return nullptr;
}
void Game::add_unit_blueprint(const UnitBlueprint& unit_blueprint) {
	m_unit_blueprints.push_back(unit_blueprint);
}
unsigned int Game::get_mineral_count() const {
	return m_raw_mineral_count / 1000;
}
unsigned int Game::get_vespene_gas_count() const {
	return m_raw_vespene_gas_count / 1000;
}
const std::list<std::shared_ptr<BuildingConstruction>>& Game::get_current_building_constructions() const {
	return m_current_building_constructions;
}
const std::list<std::shared_ptr<Building>>& Game::get_buildings() const {
	return m_buildings;
}
void Game::add_building_by_name(const std::string& name) {
	const BuildingBlueprint* building_blueprint = find_building_blueprint_by_name(name);
	assert(building_blueprint);
	m_buildings.emplace_back(new Building(*building_blueprint));
	satisfy_dependency(*building_blueprint);
}
const std::list<std::shared_ptr<Unit>>& Game::get_units() const {
	return m_units;
}
void Game::add_unit_by_name(const std::string& name) {
	const UnitBlueprint* unit_blueprint = find_unit_blueprint_by_name(name);
	assert(unit_blueprint);
	m_units.emplace_back(new Unit(*unit_blueprint));
	satisfy_dependency(*unit_blueprint);
}
const WorkerUnitAllocation& Game::get_worker_unit_allocation() const {
	return m_worker_unit_allocation;
}

unsigned int Game::get_supply_available() const {
	unsigned int supply_available = 0;
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		supply_available += (*i)->get_building_blueprint().get_supply_provided();
	}
	return supply_available;
}
unsigned int Game::get_supply_used() const {
	unsigned int supply_used = 0;
	for (auto i = m_units.begin(); i != m_units.end(); ++i) {
		supply_used += (*i)->get_unit_blueprint().get_supply_costs();
	}
	return supply_used;
}
unsigned int Game::get_remaining_supply() const {
	return get_supply_available() - get_supply_used();
}
void Game::set_worker_unit_allocation_function(const std::function<WorkerUnitAllocation(const Game&, const std::list<std::shared_ptr<Unit>>&)>& worker_unit_allocation_function) {
	m_worker_unit_allocation_function = worker_unit_allocation_function;
}
bool Game::can_construct_buildings_by_names(const std::list<std::string>& names) const {
	std::list<std::reference_wrapper<const BuildingBlueprint>> building_blueprints;
	for (auto i = names.begin(); i != names.end(); ++i) {
		const BuildingBlueprint* building_blueprint = find_building_blueprint_by_name(*i);
		if (!building_blueprint) {
			return false;
		}
		building_blueprints.push_back(*building_blueprint);

		// Check resources.
		if (building_blueprint->get_mineral_costs() * 1000 > m_raw_mineral_count ||
			building_blueprint->get_vespene_gas_costs() * 1000 > m_raw_vespene_gas_count) {
			return false;
		}

		// Check dependencies.
		auto& dependency_blueprints = building_blueprint->get_dependency_blueprints();
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

	if (do_building_constructions_require_morphing(building_blueprints)) {
		// Morph building from building or unit.
		if (building_blueprints.size() == 1) {
			const Building* building = find_building_for_building_construction(building_blueprints.front().get());
			if (building) {
				return true;
			}
		}
		const Unit* unit = find_unit_for_building_constructions(building_blueprints);
		return unit != nullptr;
	}
	// Construct building.
	return true;
}
std::list<std::shared_ptr<const BuildingConstruction>> Game::construct_buildings_by_names(const std::list<std::string>& names) {
	assert(can_construct_buildings_by_names(names));

	std::list<std::reference_wrapper<const BuildingBlueprint>> building_blueprints;
	for (auto i = names.begin(); i != names.end(); ++i) {
		const BuildingBlueprint* building_blueprint = find_building_blueprint_by_name(*i);
		building_blueprints.push_back(*building_blueprint);
	}

	if (do_building_constructions_require_morphing(building_blueprints)) {
		// Morph building from building or unit.
		bool already_removed = false;
		if (building_blueprints.size() == 1) {
			const Building* building = find_building_for_building_construction(building_blueprints.front().get());
			if (building) {
				m_buildings.remove_if([&building](const std::shared_ptr<Building>& building_b) { return building_b.get() == building; });
				already_removed = true;
			}
		}
		if (!already_removed) {
			const Unit* unit = find_unit_for_building_constructions(building_blueprints);
			if (unit) {
				m_units.remove_if([&unit](const std::shared_ptr<Unit>& unit_b) { return unit_b.get() == unit; });
			}
		}
	}
	// Construct building(s).
	std::list<std::shared_ptr<const BuildingConstruction>> building_constructions;
	for (auto i_building_blueprint = building_blueprints.begin(); i_building_blueprint != building_blueprints.end(); ++i_building_blueprint) {
		m_raw_mineral_count -= i_building_blueprint->get().get_mineral_costs() * 1000;
		m_raw_vespene_gas_count -= i_building_blueprint->get().get_vespene_gas_costs() * 1000;
		m_current_building_constructions.emplace_back(create_building_construction(i_building_blueprint->get()).release());
		building_constructions.push_back(m_current_building_constructions.back());
		m_collected_events.emplace_back(new BuildingConstructionStartEvent(m_current_building_constructions.back()));
	}
	return building_constructions;
}
bool Game::can_produce_units_by_names(const std::list<std::string>& names) const {
	assert(names.size() > 0);

	std::list<std::reference_wrapper<const UnitBlueprint>> unit_blueprints;
	for (auto i = names.begin(); i != names.end(); ++i) {
		const UnitBlueprint* unit_blueprint = find_unit_blueprint_by_name(*i);
		if (!unit_blueprint) {
			return false;
		}
		unit_blueprints.push_back(*unit_blueprint);
	}

	const UnitBlueprint& unit_blueprint = unit_blueprints.front().get();
	if (unit_blueprints.size() == 1 && find_building_for_unit_production(unit_blueprint)) {
		// Produce unit in building.

		// Check resources.
		if (unit_blueprint.get_mineral_costs() * 1000 > m_raw_mineral_count ||
			unit_blueprint.get_vespene_gas_costs() * 1000 > m_raw_vespene_gas_count) {
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
		if (mineral_costs * 1000 > m_raw_mineral_count ||
			vespene_gas_costs * 1000 > m_raw_vespene_gas_count) {
			return false;
		}

		// Check supply.
		return get_remaining_supply() >= static_cast<unsigned int>(supply_costs);
	}
	return false;
}
std::list<std::shared_ptr<const UnitProduction>> Game::produce_units_by_names(const std::list<std::string>& names) {
	assert(can_produce_units_by_names(names));

	std::list<std::reference_wrapper<const UnitBlueprint>> unit_blueprints;
	for (auto i = names.begin(); i != names.end(); ++i) {
		unit_blueprints.push_back(*find_unit_blueprint_by_name(*i));
	}
	if (unit_blueprints.size() == 1) {
		const UnitBlueprint& unit_blueprint = unit_blueprints.front().get();
		Building* building = find_building_for_unit_production(unit_blueprint);
		if (building) {
			// Produce unit in building.
			m_raw_mineral_count -= unit_blueprint.get_mineral_costs() * 1000;
			m_raw_vespene_gas_count -= unit_blueprint.get_vespene_gas_costs() * 1000;
			std::shared_ptr<UnitProduction> unit_production = building->produce_unit(unit_blueprint);
			m_collected_events.emplace_back(new UnitProductionStartEvent(unit_production));
			return { unit_production };
		}
	}

	const Unit* unit = find_unit_for_unit_productions(unit_blueprints);
	if (unit) {
		// Morph units from unit.
		m_units.remove_if([&unit](const std::shared_ptr<Unit>& unit_b) { return unit_b.get() == unit; });
		std::list<std::shared_ptr<const UnitProduction>> unit_productions;
		for (auto i_unit_blueprint = unit_blueprints.begin(); i_unit_blueprint != unit_blueprints.end(); ++i_unit_blueprint) {
			m_raw_mineral_count -= i_unit_blueprint->get().get_mineral_costs() * 1000;
			m_raw_vespene_gas_count -= i_unit_blueprint->get().get_vespene_gas_costs() * 1000;
			m_morphing_unit_productions.emplace_back(new UnitProduction(i_unit_blueprint->get()));
			unit_productions.push_back(m_morphing_unit_productions.back());
			m_collected_events.emplace_back(new UnitProductionStartEvent(m_morphing_unit_productions.back()));
		}
		return unit_productions;
	}

	// This should never be reached due to the assertion at the beginning.
	throw std::exception();
}

std::list<std::unique_ptr<Event>> Game::update(unsigned int elapsed_time_seconds) {
	std::list<std::unique_ptr<Event>> events = std::move(m_collected_events);
	m_collected_events = std::list<std::unique_ptr<Event>>();

	// Update resources (minerals, vespene gas and energy) and life durations.
	auto worker_units = find_worker_units();
	m_worker_unit_allocation = m_worker_unit_allocation_function(*this, worker_units);
	auto& minearal_collecting_worker_units = m_worker_unit_allocation.get_mineral_collecting_worker_units();
	for (auto i_minearal_collecting_worker_unit = minearal_collecting_worker_units.begin(); i_minearal_collecting_worker_unit != minearal_collecting_worker_units.end(); ++i_minearal_collecting_worker_unit) {
		unsigned int collecting_duration_seconds = (*i_minearal_collecting_worker_unit)->is_immortal() || elapsed_time_seconds < (*i_minearal_collecting_worker_unit)->get_remaining_life_duration_seconds() ? elapsed_time_seconds : (*i_minearal_collecting_worker_unit)->get_remaining_life_duration_seconds();
		m_raw_mineral_count += collecting_duration_seconds * (*i_minearal_collecting_worker_unit)->get_unit_blueprint().get_mineral_collection_rate();
	}
	auto& vespene_gas_collecting_worker_units = m_worker_unit_allocation.get_vespene_gas_collecting_worker_units();
	for (auto i_vespene_gas_collecting_worker_unit = vespene_gas_collecting_worker_units.begin(); i_vespene_gas_collecting_worker_unit != vespene_gas_collecting_worker_units.end(); ++i_vespene_gas_collecting_worker_unit) {
		unsigned int collecting_duration_seconds = (*i_vespene_gas_collecting_worker_unit)->is_immortal() || elapsed_time_seconds < (*i_vespene_gas_collecting_worker_unit)->get_remaining_life_duration_seconds() ? elapsed_time_seconds : (*i_vespene_gas_collecting_worker_unit)->get_remaining_life_duration_seconds();
		m_raw_vespene_gas_count += collecting_duration_seconds * (*i_vespene_gas_collecting_worker_unit)->get_unit_blueprint().get_vespene_gas_collection_rate();
	}
	auto i_unit = m_units.begin();
	while (i_unit != m_units.end()) {
		events.splice(events.end(), (*i_unit)->update(elapsed_time_seconds));
		if ((*i_unit)->is_dead()) {
			i_unit = m_units.erase(i_unit);
		} else {
			++i_unit;
		}
	}

	// Update building constructions.
	auto i_current_building_construction = m_current_building_constructions.begin();
	while (i_current_building_construction != m_current_building_constructions.end()) {
		auto building_construction_finished_events = (*i_current_building_construction)->update(elapsed_time_seconds);
		for (auto i_building_construction_finished_event = building_construction_finished_events.begin(); i_building_construction_finished_event != building_construction_finished_events.end(); ++i_building_construction_finished_event) {
			if (BuildingConstructionFinishEvent* building_construction_finished_event = dynamic_cast<BuildingConstructionFinishEvent*>(i_building_construction_finished_event->get())) {
				const std::shared_ptr<Building>& building = building_construction_finished_event->get_building_construction().get_building();
				m_buildings.push_back(building);
				satisfy_dependency(building->get_building_blueprint());
			}
		}
		events.splice(events.end(), building_construction_finished_events);

		if ((*i_current_building_construction)->is_finished()) {
			i_current_building_construction = m_current_building_constructions.erase(i_current_building_construction);
		} else {
			++i_current_building_construction;
		}
	}

	// Update unit productions.
	auto i_building = m_buildings.begin();
	while (i_building != m_buildings.end()) {
		auto building_events = (*i_building)->update(elapsed_time_seconds);
		for (auto i_building_event = building_events.begin(); i_building_event != building_events.end(); ++i_building_event) {
			if (UnitProductionFinishEvent* unit_production_finish_event = dynamic_cast<UnitProductionFinishEvent*>(i_building_event->get())) {
				const std::shared_ptr<Unit>& unit = unit_production_finish_event->get_unit_production().get_unit();
				m_units.push_back(unit);
				satisfy_dependency(unit->get_unit_blueprint());
			}
		}
		events.splice(events.end(), building_events);

		if ((*i_building)->is_dead()) {
			i_building = m_buildings.erase(i_building);
		} else {
			++i_building;
		}
	}
	auto i_morphing_unit_production = m_morphing_unit_productions.begin();
	while (i_morphing_unit_production != m_morphing_unit_productions.end()) {
		auto morphing_events = (*i_morphing_unit_production)->update(elapsed_time_seconds);
		for (auto i_morphing_event = morphing_events.begin(); i_morphing_event != morphing_events.end(); ++i_morphing_event) {
			if (UnitProductionFinishEvent* unit_production_finish_event = dynamic_cast<UnitProductionFinishEvent*>(i_morphing_event->get())) {
				const std::shared_ptr<Unit>& unit = unit_production_finish_event->get_unit_production().get_unit();
				m_units.push_back(unit);
				satisfy_dependency(unit->get_unit_blueprint());
			}
		}
		events.splice(events.end(), morphing_events);

		if ((*i_morphing_unit_production)->is_finished()) {
			i_morphing_unit_production = m_morphing_unit_productions.erase(i_morphing_unit_production);
		} else {
			++i_morphing_unit_production;
		}
	}

	return events;
}

bool Game::do_building_constructions_require_morphing(const std::list<std::reference_wrapper<const BuildingBlueprint>>& building_blueprints) const {
	assert(building_blueprints.size() > 0);

	if (building_blueprints.size() > 1) {
		return true;
	}
	const BuildingBlueprint& building_blueprint = building_blueprints.front().get();
	for (auto i_building_blueprint = m_building_blueprints.begin(); i_building_blueprint != m_building_blueprints.end(); ++i_building_blueprint) {
		auto& morphable_building_blueprints = i_building_blueprint->get_morphable_building_blueprints();
		for (auto i_morphable_building_blueprint = morphable_building_blueprints.begin(); i_morphable_building_blueprint != morphable_building_blueprints.end(); ++i_morphable_building_blueprint) {
			if (&i_morphable_building_blueprint->get() == &building_blueprint) {
				return true;
			}
		}
	}
	for (auto i_unit_blueprint = m_unit_blueprints.begin(); i_unit_blueprint != m_unit_blueprints.end(); ++i_unit_blueprint) {
		auto& morphable_blueprints = i_unit_blueprint->get_morphable_blueprints();
		for (auto i_morphable_blueprints = morphable_blueprints.begin(); i_morphable_blueprints != morphable_blueprints.end(); ++i_morphable_blueprints) {
			if (i_morphable_blueprints->size() == 1 && &i_morphable_blueprints->front().get() == &building_blueprint) {
				return true;
			}
		}
	}
	return false;
}
const Building* Game::find_building_for_building_construction(const BuildingBlueprint& building_blueprint) const {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		if ((*i)->can_morph(building_blueprint)) {
			return i->get();
		}
	}
	return nullptr;
}
const Unit* Game::find_unit_for_building_constructions(const std::list<std::reference_wrapper<const BuildingBlueprint>>& building_blueprints) const {
	for (auto i = m_units.begin(); i != m_units.end(); ++i) {
		if ((*i)->can_morph<BuildingBlueprint>(building_blueprints)) {
			return i->get();
		}
	}
	return nullptr;
}
std::list<std::shared_ptr<Unit>> Game::find_worker_units() const {
	std::list<std::shared_ptr<Unit>> worker_units;
	std::copy_if(m_units.begin(), m_units.end(), std::back_inserter(worker_units), [](const std::shared_ptr<Unit>& unit) {
		const UnitBlueprint& unit_blueprint = unit->get_unit_blueprint();
		return unit_blueprint.get_mineral_collection_rate() != 0 || unit_blueprint.get_vespene_gas_collection_rate() != 0;
	});
	return worker_units;
}
std::unique_ptr<BuildingConstruction> Game::create_building_construction(const BuildingBlueprint& building_blueprint) {
	return std::unique_ptr<BuildingConstruction>(new BuildingConstruction(building_blueprint));
}

const Building* Game::find_building_for_unit_production(const UnitBlueprint& unit_blueprint) const {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		if ((*i)->can_produce_unit(unit_blueprint)) {
			return i->get();
		}
	}
	return nullptr;
}
Building* Game::find_building_for_unit_production(const UnitBlueprint& unit_blueprint) {
	for (auto i = m_buildings.begin(); i != m_buildings.end(); ++i) {
		if ((*i)->can_produce_unit(unit_blueprint)) {
			return i->get();
		}
	}
	return nullptr;
}
const Unit* Game::find_unit_for_unit_productions(const std::list<std::reference_wrapper<const UnitBlueprint>>& unit_blueprints) const {
	for (auto i = m_units.begin(); i != m_units.end(); ++i) {
		if ((*i)->can_morph<UnitBlueprint>(unit_blueprints)) {
			return i->get();
		}
	}
	return nullptr;
}
void Game::satisfy_dependency(const Blueprint& blueprint) {
	for (auto i = m_satisfied_dependencies.begin(); i != m_satisfied_dependencies.end(); ++i) {
		if (&i->get() == &blueprint) {
			return;
		}
	}
	m_satisfied_dependencies.push_back(blueprint);
}
