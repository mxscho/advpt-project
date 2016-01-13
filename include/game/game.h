#pragma once

#include "game/building.h"
#include "game/building_construction.h"
#include "game/building_blueprint.h"
#include "game/unit.h"
#include "game/unit_blueprint.h"
#include "game/updatable.h"
#include "game/worker_unit_allocation.h"

#include <functional>
#include <list>

class Game : public Updatable {
public:
	Game(unsigned int mineral_count,
		unsigned int vespene_gas_count);
	virtual ~Game() = 0;

	const std::list<BuildingBlueprint>& get_building_blueprints() const;
	const BuildingBlueprint& find_building_blueprint_by_name(const std::string& name) const;
	BuildingBlueprint& find_building_blueprint_by_name(const std::string& name);
	void add_building_blueprint(const BuildingBlueprint& building_blueprint);
	const std::list<UnitBlueprint>& get_unit_blueprints() const;
	const UnitBlueprint& find_unit_blueprint_by_name(const std::string& name) const;
	UnitBlueprint& find_unit_blueprint_by_name(const std::string& name);
	void add_unit_blueprint(const UnitBlueprint& unit_blueprint);
	unsigned int get_mineral_count() const;
	unsigned int get_vespene_gas_count() const;
	const std::list<Building>& get_buildings() const;
	void add_building_by_name(const std::string& name);
	const std::list<Unit>& get_units() const;
	void add_unit_by_name(const std::string& name);

	unsigned int get_supply_available() const;
	unsigned int get_supply_used() const;
	unsigned int get_remaining_supply() const;
	void set_worker_unit_allocation_function(const std::function<WorkerUnitAllocation(unsigned int)>& worker_unit_allocation_function);
	bool can_produce_units_by_names(const std::list<std::string>& names) const;
	std::list<std::reference_wrapper<const UnitProduction>> produce_units_by_names(const std::list<std::string>& names);

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	std::list<BuildingBlueprint> m_building_blueprints;
	std::list<UnitBlueprint> m_unit_blueprints;
	unsigned int m_mineral_count;
	unsigned int m_vespene_gas_count;
	std::list<Building> m_buildings;
	std::list<Unit> m_units;

	std::list<std::reference_wrapper<const Blueprint>> m_satisfied_dependencies;
	std::list<UnitProduction> m_morphing_unit_productions;
	std::function<WorkerUnitAllocation(unsigned int)> m_worker_unit_allocation_function;
	WorkerUnitAllocation m_current_worker_unit_allocation;

	const Building* find_building_for_unit_production(const UnitBlueprint& unit_blueprint) const;
	Building* find_building_for_unit_production(const UnitBlueprint& unit_blueprint);
	const Unit* find_unit_for_unit_productions(const std::list<std::reference_wrapper<const UnitBlueprint>>& unit_blueprints) const;
	void update_worker_unit_allocation();
};
