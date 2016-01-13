#pragma once 

#include "game/events/building_construction_event.h"

#include <memory>

class BuildingConstruction;

class BuildingConstructionFinishEvent : public BuildingConstructionEvent {
public:
	explicit BuildingConstructionFinishEvent(const std::shared_ptr<BuildingConstruction>& building_construction);
};
