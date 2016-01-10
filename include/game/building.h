#pragma once 

#include "game/unit_production.h"
#include "game/updatable.h"

#include <list>

class BuildingBlueprint;
class UnitBlueprint;

class Building : public Updatable {
public:
	explicit Building(const BuildingBlueprint& building_blueprint);

	const BuildingBlueprint& get_building_blueprint() const;
	const std::list<UnitProduction>& get_current_unit_productions() const;

	bool can_produce_unit(const UnitBlueprint& unit_blueprint) const;
	const UnitProduction& produce_unit(const UnitBlueprint& unit_blueprint);

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	const BuildingBlueprint& m_building_blueprint;
	std::list<UnitProduction> m_current_unit_productions;
};
