#include "game/terran_building_construction.h"

TerranBuildingConstruction::TerranBuildingConstruction(const BuildingBlueprint& building_blueprint, const Unit& worker_unit)
	: BuildingConstruction(building_blueprint),
	m_worker_unit(worker_unit) {
}

const Unit& TerranBuildingConstruction::get_worker_unit() const {
	return m_worker_unit;
}

void TerranBuildingConstruction::update(unsigned int elapsed_time_seconds) {
	BuildingConstruction::update(elapsed_time_seconds);
}
