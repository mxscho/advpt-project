#include "game/building_blueprint.h"

BuildingBlueprint::BuildingBlueprint(const Race& race,
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
	const std::list<std::reference_wrapper<const UnitBlueprint>>& producible_unit_blueprints,
	unsigned int max_concurrent_unit_production_count)
	: Blueprint(race, name, mineral_costs, vespene_gas_costs, dependency_blueprints, creation_duration_seconds, life_duration_seconds, start_energy, max_energy, supply_provided, morphable_blueprints),
	m_producible_unit_blueprints(producible_unit_blueprints),
	m_max_concurrent_unit_production_count(max_concurrent_unit_production_count) {
}

const std::list<std::reference_wrapper<const UnitBlueprint>>& BuildingBlueprint::get_producible_unit_blueprints() const {
	return m_producible_unit_blueprints;
}
unsigned int BuildingBlueprint::get_max_concurrent_unit_production_count() const {
	return m_max_concurrent_unit_production_count;
}
