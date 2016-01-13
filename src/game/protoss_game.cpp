#include "game/protoss_game.h"

ProtossGame::ProtossGame(unsigned int mineral_count, unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count) {
}

void ProtossGame::do_chronoboost(UnitProduction& unit_production) {
	throw; // TODO
}
