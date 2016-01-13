#pragma once 

#include "game/events/building_construction_event.h"

#include <memory>

class BuildingConstruction;

class BuildingConstructionStartEvent : public BuildingConstructionEvent {
public:
	explicit BuildingConstructionStartEvent(const std::shared_ptr<BuildingConstruction>& building_construction);
};
