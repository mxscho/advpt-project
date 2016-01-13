#pragma once 

#include "game/building.h"
#include "game/updatable.h"

class BuildingBlueprint;

class BuildingConstruction : public Updatable {
public:
	explicit BuildingConstruction(const BuildingBlueprint& building_blueprint);

	const BuildingBlueprint& get_building_blueprint() const;
	unsigned int get_remaining_creation_duration_seconds() const;

	Building spawn_building() const;

	virtual void update(unsigned int elapsed_time_seconds) override;
private:
	const BuildingBlueprint& m_building_blueprint;
	unsigned int m_remaining_creation_duration_seconds;
};
