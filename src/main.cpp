#include "debug.h"

#include "game/building_blueprint.h"
#include "game/terran_game.h"
#include "game/unit_blueprint.h"

#include <iostream>

void example() {
	LOG_INFO("Default log test.");
	LOG_DEBUG("Debug log test.");
	LOG_ERROR("Error log test.");

	UnitBlueprint terran_scv_unit_blueprint(
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
		true // Is builder
	);
	BuildingBlueprint terran_command_center_blueprint(
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
		{ &terran_scv_unit_blueprint }, // Producible unit blueprints
		1 // Max concurrent unit production count
		);

	std::list<BuildingBlueprint> building_blueprints = {
		terran_command_center_blueprint
	};
	std::list<UnitBlueprint> unit_blueprints = {
		terran_scv_unit_blueprint
	};

	std::list<Building> buildings = {
		Building(terran_command_center_blueprint)
	};

	TerranGame terran_game(building_blueprints, unit_blueprints, 50, 0, buildings, {});
	// Can we produce a SCV?
	std::cout << "Expected: true, Is: " << (terran_game.can_produce_unit(terran_scv_unit_blueprint) ? "true" : "false") << std::endl;
	terran_game.produce_unit(terran_scv_unit_blueprint);
	// Can we produce another one? (Obviously no because no minerals are left and all buildings are occupied.)
	std::cout << "Expected: false, Is: " << (terran_game.can_produce_unit(terran_scv_unit_blueprint) ? "true" : "false") << std::endl;
}

int main() {
	example();
}
