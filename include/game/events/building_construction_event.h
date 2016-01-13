#pragma once 

#include "game/events/event.h"

#include <memory>

class BuildingConstruction;

class BuildingConstructionEvent : public Event {
public:
	explicit BuildingConstructionEvent(const std::shared_ptr<BuildingConstruction>& building_construction);

	const BuildingConstruction& get_building_construction() const;
private:
	std::shared_ptr<BuildingConstruction> m_building_construction;
};
