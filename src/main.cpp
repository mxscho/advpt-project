#include "debug.h"

#include "game/building_blueprint.h"
#include "game/terran_game.h"
#include "game/unit_blueprint.h"

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
		{ terran_game.find_unit_blueprint_by_name("SCV") }, // Producible unit blueprints
		1)); // Max concurrent unit production count

	terran_game.add_building_by_name("Command Center");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");

	// Can we produce a SCV?
	std::cout << "Expected: true, Is: " << (terran_game.can_produce_unit_by_name("SCV") ? "true" : "false") << std::endl;
	terran_game.produce_unit_by_name("SCV");
	// Can we produce another one? (Obviously no because no minerals are left and all buildings are occupied.)
	std::cout << "Expected: false, Is: " << (terran_game.can_produce_unit_by_name("SCV") ? "true" : "false") << std::endl;
}

int main() {
	example();
}
