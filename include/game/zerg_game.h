#pragma once 

#include "game/game.h"
#include "game/updatable.h"

class ZergGame : public Game {
public:
	ZergGame(const std::list<BuildingBlueprint>& building_blueprints,
		const std::list<UnitBlueprint>& unit_blueprints,
		unsigned int mineral_count,
		unsigned int vespene_gas_count,
		const std::list<Building>& buildings,
		const std::list<Unit>& units);

	const std::list<BuildingConstruction>& get_current_building_constructions() const;

	const BuildingConstruction& construct_building(const BuildingBlueprint& building_blueprint);
	void inject_larva();

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	std::list<BuildingConstruction> m_current_building_constructions;
};
