#pragma once 

#include "game/blueprint.h"

#include <list>
#include <string>

enum class Race;
class UnitBlueprint;

class BuildingBlueprint : public Blueprint {
public:
	BuildingBlueprint(const Race& race,
		const std::string& name,
		unsigned int mineral_costs,
		unsigned int vespene_gas_costs,
		const std::list<Blueprint>& dependency_blueprints,
		unsigned int creation_duration_seconds,
		unsigned int life_duration_seconds,
		unsigned int start_energy,
		unsigned int max_energy,
		unsigned int supply_provided,
		const std::list<std::list<Blueprint>>& morphable_blueprints,
		// The actual unit blueprints are stored in a Game instance. This is just a list of pointers to them.
		const std::list<UnitBlueprint*>& producible_unit_blueprints,
		unsigned int max_concurrent_unit_production_count);

	const std::list<UnitBlueprint*>& get_producible_unit_blueprints() const;
	unsigned int get_max_concurrent_unit_production_count() const;
private:
	std::list<UnitBlueprint*> m_producible_unit_blueprints;
	unsigned int m_max_concurrent_unit_production_count;
};
