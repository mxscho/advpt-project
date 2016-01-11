#include "game/building_construction.h"

#include "game/building_blueprint.h"

BuildingConstruction::BuildingConstruction(const BuildingBlueprint& building_blueprint)
	: Updatable(),
	m_building_blueprint(building_blueprint),
	m_remaining_creation_duration_seconds(m_building_blueprint.get_creation_duration_seconds()) {
}

const BuildingBlueprint& BuildingConstruction::get_building_blueprint() const {
	return m_building_blueprint;
}
unsigned int BuildingConstruction::get_remaining_creation_duration_seconds() const {
	return m_remaining_creation_duration_seconds;
}

Building BuildingConstruction::spawn_building() const {
	return Building(m_building_blueprint);
}

void BuildingConstruction::update(unsigned int elapsed_time_seconds) {
	m_remaining_creation_duration_seconds -= elapsed_time_seconds;
	if (m_remaining_creation_duration_seconds < 0) {
		m_remaining_creation_duration_seconds = 0;
	}
}
