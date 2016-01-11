#include "game/zerg_game.h"

ZergGame::ZergGame(unsigned int mineral_count,
	unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count),
	m_current_building_constructions() {
}

const std::list<BuildingConstruction>& ZergGame::get_current_building_constructions() const {
	return m_current_building_constructions;
}

const BuildingConstruction& ZergGame::construct_building(const BuildingBlueprint& building_blueprint) {
	throw; // TODO
}
void ZergGame::inject_larva() {
	throw; // TODO
}

void ZergGame::update(unsigned int elapsed_time_seconds) {
	throw; // TODO
}
