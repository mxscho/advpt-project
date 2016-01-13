#include "debug.h"

#include "game/building_blueprint.h"
#include "game/events/unit_production_finish_event.h"
#include "game/events/unit_production_start_event.h"
#include "game/events/building_construction_finish_event.h"
#include "game/events/building_construction_start_event.h"
#include "game/terran_game.h"
#include "game/unit_blueprint.h"
#include "game/zerg_game.h"

#include <iostream>

void example() {
	LOG_INFO("Default log test.");
	LOG_DEBUG("Debug log test.");
	LOG_ERROR("Error log test.");
	
	TerranGame terran_game(50, 0);
	terran_game.add_unit_blueprint(UnitBlueprint(
		Race::Terran, // Race
		"SCV", // Name
		50, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		5, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{}, // Morphable blueprints
		1, // Supply costs,
		0, // Mineral collection rate
		0, // Vespine gas collection rate
		true)); // Is builder
	terran_game.add_building_blueprint(BuildingBlueprint(
		Race::Terran, // Race
		"Command Center", // Name
		400, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		60, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		50, // Start energy
		200, // Max energy
		9, // Supply provided
		{}, // Morphable blueprints
		{ terran_game.find_unit_blueprint_by_name("SCV") /* We add the SCV here already. */ }, // Producible unit blueprints
		1)); // Max concurrent unit production count

	terran_game.add_building_by_name("Command Center");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");

	// Can we produce a SCV?
	std::cout << "Expected: true, Is: " << (terran_game.can_produce_units_by_names({ "SCV" }) ? "true" : "false") << std::endl;
	terran_game.produce_units_by_names({ "SCV" });
	// Can we produce another one? (Obviously no because no minerals are left and all buildings are occupied.)
	std::cout << "Expected: false, Is: " << (terran_game.can_produce_units_by_names({ "SCV" }) ? "true" : "false") << std::endl;

	ZergGame zerg_game(50, 0);
	zerg_game.add_unit_blueprint(UnitBlueprint(
		Race::Zerg, // Race
		"Larva", // Name
		0, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		30, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{ /* Drone is not yet existing, therefore we have to add it later. */ }, // Morphable blueprints
		0, // Supply costs,
		0, // Mineral collection rate
		0, // Vespine gas collection rate
		false)); // Is builder
	zerg_game.add_unit_blueprint(UnitBlueprint(
		Race::Zerg, // Race
		"Drone", // Name
		50, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		30, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{}, // Morphable blueprints
		1, // Supply costs,
		700, // Mineral collection rate
		350, // Vespine gas collection rate
		false)); // Is builder
	// No we can add the drone to larva's morphables.
	zerg_game.find_unit_blueprint_by_name("Larva").add_morphable_blueprints({ zerg_game.find_unit_blueprint_by_name("Drone") });
	zerg_game.add_unit_blueprint(UnitBlueprint(
		Race::Zerg, // Race
		"Zergling", // Name
		25, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		40, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{}, // Morphable blueprints
		1, // Supply costs,
		0, // Mineral collection rate
		0, // Vespine gas collection rate
		false)); // Is builder
	// No we can add the drone to larva's morphables.
	zerg_game.find_unit_blueprint_by_name("Larva").add_morphable_blueprints({ 
		zerg_game.find_unit_blueprint_by_name("Zergling"),
		zerg_game.find_unit_blueprint_by_name("Zergling")
	});
	zerg_game.add_building_blueprint(BuildingBlueprint(
		Race::Zerg, // Race
		"Hatchery", // Name
		400, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		60, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		50, // Start energy
		200, // Max energy
		9, // Supply provided
		{}, // Morphable blueprints
		{}, // Producible unit blueprints
		1)); // Max concurrent unit production count

	zerg_game.add_building_by_name("Hatchery");
	zerg_game.add_unit_by_name("Larva");
	zerg_game.add_unit_by_name("Larva");
	zerg_game.add_unit_by_name("Larva");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");

	// Can we morph two zerglings?
	std::cout << "Expected: true, Is: " << (zerg_game.can_produce_units_by_names({ "Zergling", "Zergling" }) ? "true" : "false") << std::endl;
	zerg_game.produce_units_by_names({ "Zergling", "Zergling" });
	// Can we morph another drone? (Obviously no because no minerals are left.)
	std::cout << "Expected: false, Is: " << (zerg_game.can_produce_units_by_names({ "Drone" }) ? "true" : "false") << std::endl;

	// Example: How to simulate game and get events.
	for (unsigned int i = 0; i < 200; ++i) {
		std::cout << "> " << i << " sec until " << (i + 1) << " sec -" <<
			" Min: " << zerg_game.get_mineral_count() <<
			" Gas: " << zerg_game.get_vespene_gas_count() <<
			std::endl;
		auto events = zerg_game.Game::update(1);
		for (auto j = events.begin(); j != events.end(); ++j) {
			if (UnitProductionStartEvent* unit_production_start_event = dynamic_cast<UnitProductionStartEvent*>(j->get())) {
				std::shared_ptr<Unit> unit = unit_production_start_event->get_unit_production().get_unit();
				std::cout << "EVENT: UnitProductionStartEvent -" <<
					" ID: " << unit->get_id() <<
					" Name: " << unit->get_unit_blueprint().get_name() <<
					std::endl;
			}
			if (UnitProductionFinishEvent* unit_production_finish_event = dynamic_cast<UnitProductionFinishEvent*>(j->get())) {
				std::shared_ptr<Unit> unit = unit_production_finish_event->get_unit_production().get_unit();
				std::cout << "EVENT: UnitProductionFinishEvent -" <<
					" ID: " << unit->get_id() <<
					" Name: " << unit->get_unit_blueprint().get_name() <<
					std::endl;
			}
			if (BuildingConstructionStartEvent* building_construction_start_event = dynamic_cast<BuildingConstructionStartEvent*>(j->get())) {
				std::shared_ptr<Building> building = building_construction_start_event->get_building_construction().get_building();
				std::cout << "EVENT: BuildingConstructionStartEvent -" <<
					" ID: " << building->get_id() <<
					" Name: " << building->get_building_blueprint().get_name() <<
					std::endl;
			}
			if (BuildingConstructionFinishEvent* building_construction_finish_event = dynamic_cast<BuildingConstructionFinishEvent*>(j->get())) {
				std::shared_ptr<Building> building = building_construction_finish_event->get_building_construction().get_building();
				std::cout << "EVENT: BuildingConstructionFinishEvent -" <<
					" ID: " << building->get_id() <<
					" Name: " << building->get_building_blueprint().get_name() <<
					std::endl;
			}
		}
		// Let's build new hatcheries if we have 400 minerals.
		if (zerg_game.get_mineral_count() > 400) {
			zerg_game.construct_buildings_by_names({ "Hatchery" });
		}
	}
}

int main() {
	example();
}
