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
	const std::list<std::reference_wrapper<const BuildingBlueprint>>& morphable_building_blueprints,
	const std::list<std::reference_wrapper<const UnitBlueprint>>& producible_unit_blueprints,
	unsigned int max_concurrent_unit_production_count)
	: Blueprint(race, name, mineral_costs, vespene_gas_costs, dependency_blueprints, creation_duration_seconds, life_duration_seconds, start_energy, max_energy, supply_provided),
	m_morphable_building_blueprints(morphable_building_blueprints),
	m_producible_unit_blueprints(producible_unit_blueprints),
	m_max_concurrent_unit_production_count(max_concurrent_unit_production_count) {
}

const std::list<std::reference_wrapper<const BuildingBlueprint>>& BuildingBlueprint::get_morphable_building_blueprints() const {
	return m_morphable_building_blueprints;
}
void BuildingBlueprint::add_morphable_building_blueprint(const BuildingBlueprint& building_blueprint) {
	m_morphable_building_blueprints.push_back(building_blueprint);
}
const std::list<std::reference_wrapper<const UnitBlueprint>>& BuildingBlueprint::get_producible_unit_blueprints() const {
	return m_producible_unit_blueprints;
}
void BuildingBlueprint::add_producible_unit_blueprint(const UnitBlueprint& unit_blueprint) {
	m_producible_unit_blueprints.push_back(unit_blueprint);
}
unsigned int BuildingBlueprint::get_max_concurrent_unit_production_count() const {
	return m_max_concurrent_unit_production_count;
}
