#pragma once 

#include "game/building.h"
#include "game/updatable.h"

#include <memory>

class Event;
class BuildingBlueprint;

class BuildingConstruction : public std::enable_shared_from_this<BuildingConstruction>, public Updatable {
public:
	explicit BuildingConstruction(const BuildingBlueprint& building_blueprint);

	const BuildingBlueprint& get_building_blueprint() const;
	unsigned int get_remaining_creation_duration_seconds() const;
	bool is_finished() const;
	std::shared_ptr<Building> get_building() const;

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
protected:
	bool m_is_finished;
private:
	const BuildingBlueprint& m_building_blueprint;
	unsigned int m_remaining_creation_duration_seconds;

	std::shared_ptr<Building> m_building;
};
