#pragma once

#include "game/building.h"
#include "game/building_blueprint.h"
#include "game/unit.h"
#include "game/unit_blueprint.h"
#include "game/updatable.h"
#include "game/worker_unit_allocation.h"

#include <functional>
#include <memory>
#include <list>

class BuildingConstruction;
class Event;

class Game : public Updatable {
public:
	static const std::function<WorkerUnitAllocation(const Game&, const std::list<std::shared_ptr<Unit>>&)> c_default_worker_allocation_function;

	Game(unsigned int mineral_count, unsigned int vespene_gas_count);
	Game(const Game& game);
	Game& operator=(const Game& game);
	virtual ~Game() = 0;

	virtual Race get_race() const = 0;
	virtual Game* clone() const = 0;

	const std::list<std::shared_ptr<BuildingBlueprint>>& get_building_blueprints() const;
	const BuildingBlueprint* find_building_blueprint_by_name(const std::string& name) const;
	BuildingBlueprint* find_building_blueprint_by_name(const std::string& name);
	void add_building_blueprint(const std::shared_ptr<BuildingBlueprint>& building_blueprint);
	const std::list<std::shared_ptr<UnitBlueprint>>& get_unit_blueprints() const;
	const UnitBlueprint* find_unit_blueprint_by_name(const std::string& name) const;
	UnitBlueprint* find_unit_blueprint_by_name(const std::string& name);
	void add_unit_blueprint(const std::shared_ptr<UnitBlueprint>& unit_blueprint);
	unsigned int get_mineral_count() const;
	unsigned int get_vespene_gas_count() const;
	const std::list<std::shared_ptr<BuildingConstruction>>& get_current_building_constructions() const;
	const std::list<std::shared_ptr<Building>>& get_buildings() const;
	void add_building_by_name(const std::string& name);
	const std::list<std::shared_ptr<Unit>>& get_units() const;
	void add_unit_by_name(const std::string& name);
	const WorkerUnitAllocation& get_worker_unit_allocation() const;
	bool is_busy() const;

	unsigned int get_supply_available() const;
	unsigned int get_supply_used() const;
	unsigned int get_remaining_supply() const;
	void set_worker_unit_allocation_function(const std::function<WorkerUnitAllocation(const Game&, const std::list<std::shared_ptr<Unit>>&)>& worker_unit_allocation_function);
	virtual bool can_construct_buildings_by_names(const std::list<std::string>& names) const;
	std::list<std::shared_ptr<const BuildingConstruction>> construct_buildings_by_names(const std::list<std::string>& names);
	bool can_produce_units_by_names(const std::list<std::string>& names) const;
	std::list<std::shared_ptr<const UnitProduction>> produce_units_by_names(const std::list<std::string>& names);

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
protected:
	std::list<std::shared_ptr<Building>> m_buildings;
	std::list<std::shared_ptr<Unit>> m_units;
	unsigned int m_raw_mineral_count;
	unsigned int m_raw_vespene_gas_count;
	std::list<std::shared_ptr<BuildingConstruction>> m_current_building_constructions;

	bool do_building_constructions_require_morphing(const std::list<std::reference_wrapper<const BuildingBlueprint>>& building_blueprints) const;
	const Building* find_building_for_building_construction(const BuildingBlueprint& building_blueprint) const;
	const Unit* find_unit_for_building_constructions(const std::list<std::reference_wrapper<const BuildingBlueprint>>& building_blueprints) const;
	virtual std::list<std::shared_ptr<Unit>> find_worker_units() const;
	virtual std::unique_ptr<BuildingConstruction> create_building_construction(const BuildingBlueprint& building_blueprint);
private:
	std::list<std::shared_ptr<BuildingBlueprint>> m_building_blueprints;
	std::list<std::shared_ptr<UnitBlueprint>> m_unit_blueprints;
	WorkerUnitAllocation m_worker_unit_allocation;

	std::list<std::reference_wrapper<const Blueprint>> m_satisfied_dependencies;
	std::list<std::shared_ptr<UnitProduction>> m_morphing_unit_productions;
	std::function<WorkerUnitAllocation(const Game&, const std::list<std::shared_ptr<Unit>>&)> m_worker_unit_allocation_function;
	std::list<std::unique_ptr<Event>> m_collected_events;

	const Building* find_building_for_unit_production(const UnitBlueprint& unit_blueprint) const;
	Building* find_building_for_unit_production(const UnitBlueprint& unit_blueprint);
	const Unit* find_unit_for_unit_productions(const std::list<std::reference_wrapper<const UnitBlueprint>>& unit_blueprints) const;
	void satisfy_dependency(const Blueprint& blueprint);
};
