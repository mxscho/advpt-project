#pragma once 

#include "game/blueprint.h"

#include <list>
#include <string>

enum class Race;

class UnitBlueprint : public Blueprint {
public:
	UnitBlueprint(const Race& race,
		const std::string& name,
		unsigned int mineral_costs,
		unsigned int vespene_gas_costs,
		const std::list<std::reference_wrapper<const Blueprint>>& dependency_blueprints,
		unsigned int creation_duration_seconds,
		unsigned int life_duration_seconds,
		unsigned int start_energy,
		unsigned int max_energy,
		unsigned int supply_provided,
		const std::list<std::list<std::reference_wrapper<const Blueprint>>>& morphable_blueprints,
		unsigned int supply_costs,
		unsigned int mineral_collection_rate,
		unsigned int vespene_gas_collection_rate,
		bool is_builder);
	
	unsigned int get_supply_costs() const;
	unsigned int get_mineral_collection_rate() const;
	unsigned int get_vespene_gas_collection_rate() const;
	bool is_builder() const;
private:
	unsigned int m_supply_costs;
	unsigned int m_mineral_collection_rate;
	unsigned int m_vespene_gas_collection_rate;
	bool m_is_builder;
};
