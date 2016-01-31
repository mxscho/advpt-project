#pragma once 

#include "game/game.h"

enum class Race;
class UnitProduction;

class ProtossGame : public Game {
public:
	ProtossGame(unsigned int mineral_count, unsigned int vespene_gas_count);

	virtual Race get_race() const override;
	virtual ProtossGame* clone() const override;

	void do_chronoboost(UnitProduction& unit_production);
};
