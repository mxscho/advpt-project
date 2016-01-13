#include "game/building_construction.h"

#include "game/building_blueprint.h"
#include "game/events/building_construction_finish_event.h"

BuildingConstruction::BuildingConstruction(const BuildingBlueprint& building_blueprint)
	: std::enable_shared_from_this<BuildingConstruction>(),
	Updatable(),
	m_is_finished(false),
	m_building_blueprint(building_blueprint),
	m_remaining_creation_duration_seconds(m_building_blueprint.get_creation_duration_seconds()),
	m_building(new Building(m_building_blueprint)) {
}

const BuildingBlueprint& BuildingConstruction::get_building_blueprint() const {
	return m_building_blueprint;
}
unsigned int BuildingConstruction::get_remaining_creation_duration_seconds() const {
	return m_remaining_creation_duration_seconds;
}
bool BuildingConstruction::is_finished() const {
	return m_is_finished;
}
std::shared_ptr<Building> BuildingConstruction::get_building() const {
	return m_building;
}

std::list<std::unique_ptr<Event>> BuildingConstruction::update(unsigned int elapsed_time_seconds) {
	if (m_is_finished) {
		return {};
	}

	m_remaining_creation_duration_seconds -= elapsed_time_seconds < m_remaining_creation_duration_seconds ? elapsed_time_seconds : m_remaining_creation_duration_seconds;

	if (m_remaining_creation_duration_seconds == 0) {
		m_is_finished = true;

		std::list<std::unique_ptr<Event>> building_construction_finish_events;
		building_construction_finish_events.emplace_back(new BuildingConstructionFinishEvent(shared_from_this()));
		return building_construction_finish_events;
	}
	return {};
}
