#include "game/blueprint.h"

Blueprint::Blueprint(const Race& race,
	const std::string& name,
	unsigned int mineral_costs,
	unsigned int vespene_gas_costs,
	const std::list<Blueprint>& dependency_blueprints,
	unsigned int creation_duration_seconds,
	unsigned int life_duration_seconds,
	unsigned int start_energy,
	unsigned int max_energy,
	unsigned int supply_provided,
	const std::list<std::list<Blueprint>>& morphable_blueprints)
	: m_race(race),
	m_name(name),
	m_mineral_costs(mineral_costs),
	m_vespene_gas_costs(vespene_gas_costs),
	m_dependency_blueprints(dependency_blueprints),
	m_creation_duration_seconds(creation_duration_seconds),
	m_life_duration_seconds(life_duration_seconds),
	m_start_energy(start_energy),
	m_max_energy(max_energy),
	m_supply_provided(supply_provided),
	m_morphable_blueprints(morphable_blueprints) {
}

const Race& Blueprint::get_race() const {
	return m_race;
}
const std::string& Blueprint::get_name() const {
	return m_name;
}
unsigned int Blueprint::get_mineral_costs() const {
	return m_mineral_costs;
}
unsigned int Blueprint::get_vespene_gas_costs() const {
	return m_vespene_gas_costs;
}
const std::list<Blueprint>& Blueprint::get_dependency_blueprints() const {
	return m_dependency_blueprints;
}
unsigned int Blueprint::get_creation_duration_seconds() const {
	return m_creation_duration_seconds;
}
unsigned int Blueprint::get_life_duration_seconds() const {
	return m_life_duration_seconds;
}
unsigned int Blueprint::get_start_energy() const {
	return m_start_energy;
}
unsigned int Blueprint::get_max_energy() const {
	return m_max_energy;
}
unsigned int Blueprint::get_supply_provided() const {
	return m_supply_provided;
}
const std::list<std::list<Blueprint>>& Blueprint::get_morphable_blueprints() const {
	return m_morphable_blueprints;
}
