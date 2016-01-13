#include "game/protoss_game.h"

ProtossGame::ProtossGame(unsigned int mineral_count,
	unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count),
	m_current_building_constructions() {
}

const std::list<BuildingConstruction>& ProtossGame::get_current_building_constructions() const {
	return m_current_building_constructions;
}

const BuildingConstruction& ProtossGame::construct_building(const BuildingBlueprint& building_blueprint) {
	throw; // TODO
}
void ProtossGame::do_chronoboost(UnitProduction& unit_production) {
	throw; // TODO
}

void ProtossGame::update(unsigned int elapsed_time_seconds) {
	throw; // TODO
}
