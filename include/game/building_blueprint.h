#pragma once 

#include "game/blueprint.h"
#include "game/unit_blueprint.h"

#include <functional>
#include <list>
#include <string>

enum class Race;

class BuildingBlueprint : public Blueprint {
public:
	BuildingBlueprint(const Race& race,
		const std::string& name,
		unsigned int mineral_costs,
		unsigned int vespene_gas_costs,
		const std::list<std::reference_wrapper<const Blueprint>>& dependency_blueprints,
		unsigned int creation_duration_seconds,
		unsigned int life_duration_seconds,
		unsigned int start_energy,
		unsigned int max_energy,
		unsigned int supply_provided,
		const std::list<std::reference_wrapper<const BuildingBlueprint>>& morphable_building_blueprints,
		const std::list<std::reference_wrapper<const UnitBlueprint>>& producible_unit_blueprints,
		unsigned int max_concurrent_unit_production_count);

	const std::list<std::reference_wrapper<const BuildingBlueprint>>& get_morphable_building_blueprints() const;
	void add_morphable_building_blueprint(const BuildingBlueprint& building_blueprint);
	const std::list<std::reference_wrapper<const UnitBlueprint>>& get_producible_unit_blueprints() const;
	void add_producible_unit_blueprint(const UnitBlueprint& unit_blueprint);
	unsigned int get_max_concurrent_unit_production_count() const;
private:
	std::list<std::reference_wrapper<const BuildingBlueprint>> m_morphable_building_blueprints;
	std::list<std::reference_wrapper<const UnitBlueprint>> m_producible_unit_blueprints;
	unsigned int m_max_concurrent_unit_production_count;
};
