#pragma once 

#include "game/game.h"
#include "game/updatable.h"

class ProtossGame : public Game {
public:
	ProtossGame(const std::list<BuildingBlueprint>& building_blueprints,
		const std::list<UnitBlueprint>& unit_blueprints,
		unsigned int mineral_count,
		unsigned int vespene_gas_count,
		const std::list<Building>& buildings,
		const std::list<Unit>& units);

	const std::list<BuildingConstruction>& get_current_building_constructions() const;

	const BuildingConstruction& construct_building(const BuildingBlueprint& building_blueprint);
	void do_chronoboost(UnitProduction& unit_production);

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	std::list<BuildingConstruction> m_current_building_constructions;
};
