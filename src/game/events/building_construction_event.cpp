#include "game/events/building_construction_event.h"

BuildingConstructionEvent::BuildingConstructionEvent(const std::shared_ptr<BuildingConstruction>& building_construction)
	: Event(),
	m_building_construction(building_construction) {
}

const BuildingConstruction& BuildingConstructionEvent::get_building_construction() const {
	return *m_building_construction;
}
