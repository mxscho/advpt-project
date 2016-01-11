#pragma once 

#include "game/race.h"

#include <functional>
#include <list>
#include <string>

class Blueprint {
public:
	Blueprint(const Race& race,
		const std::string& name,
		unsigned int mineral_costs,
		unsigned int vespene_gas_costs,
		const std::list<std::reference_wrapper<const Blueprint>>& dependency_blueprints,
		unsigned int creation_duration_seconds,
		unsigned int life_duration_seconds,
		unsigned int start_energy,
		unsigned int max_energy,
		unsigned int supply_provided,
		const std::list<std::list<std::reference_wrapper<const Blueprint>>>& morphable_blueprints);
	virtual ~Blueprint() = 0;

	const Race& get_race() const;
	const std::string& get_name() const;
	unsigned int get_mineral_costs() const;
	unsigned int get_vespene_gas_costs() const;
	const std::list<std::reference_wrapper<const Blueprint>>& get_dependency_blueprints() const;
	unsigned int get_creation_duration_seconds() const;
	unsigned int get_life_duration_seconds() const;
	unsigned int get_start_energy() const;
	unsigned int get_max_energy() const;
	unsigned int get_supply_provided() const;
	const std::list<std::list<std::reference_wrapper<const Blueprint>>>& get_morphable_blueprints() const;
private:
	Race m_race;
	std::string m_name;
	unsigned int m_mineral_costs;
	unsigned int m_vespene_gas_costs;
	std::list<std::reference_wrapper<const Blueprint>> m_dependency_blueprints;
	unsigned int m_creation_duration_seconds;
	unsigned int m_life_duration_seconds;
	unsigned int m_start_energy;
	unsigned int m_max_energy;
	unsigned int m_supply_provided;
	std::list<std::list<std::reference_wrapper<const Blueprint>>> m_morphable_blueprints;
};
