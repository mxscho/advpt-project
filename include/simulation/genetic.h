#pragma once


#include "game/building.h"
#include "game/building_blueprint.h"
#include "game/unit.h"
#include "game/unit_blueprint.h"
#include "game/updatable.h"
#include "game/worker_unit_allocation.h"
#include "game/protoss_game.h"
#include "game/terran_game.h"
#include "game/unit_blueprint.h"
#include "game/zerg_game.h"
#include "game/events/unit_production_finish_event.h"
#include "game/events/unit_production_start_event.h"
#include "game/events/building_construction_finish_event.h"
#include "game/events/building_construction_start_event.h"
#include "io/output_formatter.h"


#include <functional>
#include <memory>
#include <list>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>



using namespace std;

class Genetic{
public:
	
	Genetic(const string& unit, Game& game,int mode, const string& race);
	
protected:
	
private:
	int forwardSimulator(const string& race, Game& game, std::list<std::string> buildlist);
	int fitness(std::list<std::string> buildlist);
	void selection();
	void reproduction(Game& game);
	void mutation();
	int m_mode;
	int m_bestfitnes;
	int m_fitnesspushmax;
	string m_unit;
	string m_race;
	//Game& m_game;
	std::list<std::pair<int, std::list<std::string>>> population;
	std::list<std::pair<int, std::list<std::string>>> newpopulation;
};
