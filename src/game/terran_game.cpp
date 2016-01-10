#include "game/terran_game.h"

TerranGame::TerranGame(const std::list<BuildingBlueprint>& building_blueprints,
	const std::list<UnitBlueprint>& unit_blueprints,
	unsigned int mineral_count,
	unsigned int vespene_gas_count,
	const std::list<Building>& buildings,
	const std::list<Unit>& units)
	: Game(building_blueprints,
		unit_blueprints,
		mineral_count,
		vespene_gas_count,
		buildings,
		units),
	m_current_terran_building_constructions() {
}

const std::list<TerranBuildingConstruction>& TerranGame::get_current_terran_building_constructions() const {
	return m_current_terran_building_constructions;
}

const TerranBuildingConstruction& TerranGame::construct_building(const BuildingBlueprint& building_blueprint) {
	throw; // TODO
}
void TerranGame::call_mule() {
	throw; // TODO
}

void TerranGame::update(unsigned int elapsed_time_seconds) {
	throw; // TODO
}
