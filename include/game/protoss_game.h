#pragma once 

#include "game/game.h"

class UnitProduction;

class ProtossGame : public Game {
public:
	ProtossGame(unsigned int mineral_count, unsigned int vespene_gas_count);

	void do_chronoboost(UnitProduction& unit_production);
};
