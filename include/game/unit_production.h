#pragma once 

#include "game/unit.h"
#include "game/updatable.h"

#include <memory>

class Building;
class Event;
class UnitBlueprint;

class UnitProduction : public std::enable_shared_from_this<UnitProduction>, public Updatable {
public:
	explicit UnitProduction(const UnitBlueprint& unit_blueprint);
	// Use this constructor to hand over a building which is responsible for the unit production so this is not a morphing unit production.
	UnitProduction(const UnitBlueprint& unit_blueprint, const Building& building);

	const UnitBlueprint& get_unit_blueprint() const;
	// If this returns a nullptr, the unit production is a morphing unit production and no building is responsible for the production.
	const Building* get_building() const;
	unsigned int get_remaining_creation_duration_milliseconds() const;
	unsigned int get_remaining_boost_duration_seconds() const;
	bool is_finished() const;
	std::shared_ptr<Unit> get_unit() const;

	// boost_factor describes the boosted unit production rate/speed in percentage points.
	// If the building is e.g. working 50% faster, boost_factor would be 150.
	void boost_creation_duration(unsigned int boost_factor, unsigned int boost_duration_seconds);

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	const UnitBlueprint& m_unit_blueprint;
	const Building* m_building;
	unsigned int m_remaining_creation_duration_milliseconds;
	unsigned int m_remaining_boost_duration_seconds;
	bool m_is_finished;

	std::shared_ptr<Unit> m_unit;
	unsigned int m_boost_factor;
};
