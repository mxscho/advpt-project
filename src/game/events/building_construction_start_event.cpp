#include "game/events/building_construction_start_event.h"

BuildingConstructionStartEvent::BuildingConstructionStartEvent(const std::shared_ptr<BuildingConstruction>& building_construction)
	: BuildingConstructionEvent(building_construction) {
}
