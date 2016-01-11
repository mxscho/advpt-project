#include "game/unit_blueprint.h"

UnitBlueprint::UnitBlueprint(const Race& race,
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
	bool is_builder)
	: Blueprint(race, name, mineral_costs, vespene_gas_costs, dependency_blueprints, creation_duration_seconds, life_duration_seconds, start_energy, max_energy, supply_provided, morphable_blueprints),
	m_supply_costs(supply_costs),
	m_mineral_collection_rate(mineral_collection_rate),
	m_vespene_gas_collection_rate(vespene_gas_collection_rate),
	m_is_builder(is_builder) {
}

unsigned int UnitBlueprint::get_supply_costs() const {
	return m_supply_costs;
}
unsigned int UnitBlueprint::get_mineral_collection_rate() const {
	return m_mineral_collection_rate;
}
unsigned int UnitBlueprint::get_vespene_gas_collection_rate() const {
	return m_vespene_gas_collection_rate;
}
bool UnitBlueprint::is_builder() const {
	return m_is_builder;
}
