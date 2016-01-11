#include "game/terran_game.h"

TerranGame::TerranGame(unsigned int mineral_count,
	unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count),
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
