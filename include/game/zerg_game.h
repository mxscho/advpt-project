#pragma once 

#include "game/game.h"

class ZergGame : public Game {
public:
	ZergGame(unsigned int mineral_count, unsigned int vespene_gas_count);

	void inject_larva();
};
