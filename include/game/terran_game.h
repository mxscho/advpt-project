#pragma once 

#include "game/game.h"
#include "game/terran_building_construction.h"
#include "game/updatable.h"

class TerranGame : public Game {
public:
	TerranGame(const std::list<BuildingBlueprint>& building_blueprints,
		const std::list<UnitBlueprint>& unit_blueprints,
		unsigned int mineral_count,
		unsigned int vespene_gas_count,
		const std::list<Building>& buildings,
		const std::list<Unit>& units);

	const std::list<TerranBuildingConstruction>& get_current_terran_building_constructions() const;

	const TerranBuildingConstruction& construct_building(const BuildingBlueprint& building_blueprint);
	void call_mule();

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	std::list<TerranBuildingConstruction> m_current_terran_building_constructions;
};
