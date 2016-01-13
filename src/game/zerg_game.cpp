#include "game/zerg_game.h"

ZergGame::ZergGame(unsigned int mineral_count, unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count) {
}

void ZergGame::inject_larva() {
	throw; // TODO
}

// TODO: Override update(...) and do automatic larva generation.
