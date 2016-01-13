#pragma once 

#include "game/unit.h"
#include "game/updatable.h"

class UnitBlueprint;

class UnitProduction : public Updatable {
public:
	explicit UnitProduction(const UnitBlueprint& unit_blueprint);

	const UnitBlueprint& get_unit_blueprint() const;
	unsigned int get_remaining_creation_duration_milliseconds() const;
	unsigned int get_remaining_boost_duration_seconds() const;

	// boost_factor describes the boosted unit production rate/speed in percentage points.
	// If the building is e.g. working 50% faster, boost_factor would be 150.
	void boost_creation_duration(unsigned int boost_factor, unsigned int boost_duration_seconds);
	Unit spawn_unit() const;

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	const UnitBlueprint& m_unit_blueprint;
	unsigned int m_remaining_creation_duration_milliseconds;
	unsigned int m_remaining_boost_duration_seconds;

	unsigned int m_boost_factor;
};
