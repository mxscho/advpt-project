#pragma once 

#include "game/building_construction.h"
#include "game/updatable.h"

class BuildingBlueprint;
class Unit;

class TerranBuildingConstruction : public BuildingConstruction {
public:
	TerranBuildingConstruction(const BuildingBlueprint& building_blueprint, Unit& builder_unit);

	const Unit& get_builder_unit() const;

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	Unit& m_builder_unit;
};
