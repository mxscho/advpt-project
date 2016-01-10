#pragma once

#include "game/building.h"
#include "game/building_construction.h"
#include "game/building_blueprint.h"
#include "game/unit.h"
#include "game/unit_blueprint.h"
#include "game/updatable.h"

#include <functional>
#include <list>

class WorkerUnitAllocation;

class Game : public Updatable {
public:
	Game(const std::list<BuildingBlueprint>& building_blueprints,
		const std::list<UnitBlueprint>& unit_blueprints,
		unsigned int mineral_count,
		unsigned int vespene_gas_count,
		const std::list<Building>& buildings,
		const std::list<Unit>& units);
	virtual ~Game() = 0;

	const std::list<BuildingBlueprint>& get_building_blueprints() const;
	const std::list<UnitBlueprint>& get_unit_blueprints() const;
	unsigned int get_mineral_count() const;
	unsigned int get_vespene_gas_count() const;
	const std::list<Building>& get_buildings() const;
	const std::list<Unit>& get_units() const;

	unsigned int get_supply_available() const;
	unsigned int get_supply_used() const;
	unsigned int get_remaining_supply() const;
	void set_worker_unit_allocation_function(const std::function<WorkerUnitAllocation(unsigned int)>& worker_unit_allocation_function);
	bool can_produce_unit(const UnitBlueprint& unit_blueprint) const;
	const UnitProduction& produce_unit(const UnitBlueprint& unit_blueprint);

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	std::list<BuildingBlueprint> m_building_blueprints;
	std::list<UnitBlueprint> m_unit_blueprints;
	unsigned int m_mineral_count;
	unsigned int m_vespene_gas_count;
	std::list<Building> m_buildings;
	std::list<Unit> m_units;

	std::list<UnitProduction> m_morphing_unit_productions;

	std::function<WorkerUnitAllocation(unsigned int)> m_worker_unit_allocation_function;

	const Building* find_building_for_unit_production(const UnitBlueprint& unit_blueprint) const;
	Building* find_building_for_unit_production(const UnitBlueprint& unit_blueprint);
};
