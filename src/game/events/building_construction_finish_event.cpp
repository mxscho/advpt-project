#include "game/events/building_construction_finish_event.h"

BuildingConstructionFinishEvent::BuildingConstructionFinishEvent(const std::shared_ptr<BuildingConstruction>& building_construction)
	: BuildingConstructionEvent(building_construction) {
}
