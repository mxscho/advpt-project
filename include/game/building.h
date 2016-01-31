#pragma once 

#include "game/energetic.h"
#include "game/identifiable.h"
#include "game/mortal.h"
#include "game/unit_production.h"

#include <list>
#include <memory>

class Event;

class BuildingBlueprint;
class UnitBlueprint;

class Building : public Identifiable, public Energetic, public Mortal {
public:
	explicit Building(const BuildingBlueprint& building_blueprint);
	Building(const Building& building);

	const BuildingBlueprint& get_building_blueprint() const;
	const std::list<std::shared_ptr<UnitProduction>>& get_current_unit_productions() const;

	bool can_morph(const BuildingBlueprint& building_blueprint) const;
	bool can_produce_unit(const UnitBlueprint& unit_blueprint) const;
	std::shared_ptr<UnitProduction> produce_unit(const UnitBlueprint& unit_blueprint);

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	const BuildingBlueprint& m_building_blueprint;
	std::list<std::shared_ptr<UnitProduction>> m_current_unit_productions;

	bool is_working_to_capacity() const;
};
