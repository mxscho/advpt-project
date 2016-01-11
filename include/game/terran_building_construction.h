#pragma once 

#include "game/building_construction.h"
#include "game/updatable.h"

class BuildingBlueprint;
class Unit;

class TerranBuildingConstruction : public BuildingConstruction {
public:
	TerranBuildingConstruction(const BuildingBlueprint& building_blueprint, const Unit& worker_unit);

	const Unit& get_worker_unit() const;

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	const Unit& m_worker_unit;
};
