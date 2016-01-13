#include "game/zerg_game.h"

unsigned int ZergGame::c_larva_spawn_duration = 15;

ZergGame::ZergGame(unsigned int mineral_count, unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count),
	m_larva_spawn_remaining_duration(c_larva_spawn_duration) {
}

void ZergGame::inject_larva() {
	throw; // TODO
}

std::list<std::unique_ptr<Event>> ZergGame::update(unsigned int elapsed_time_seconds) {
	// TODO: This is totally wrong. Please make it better.
	unsigned int elapsed_time_seconds_copy = elapsed_time_seconds;
	while (elapsed_time_seconds_copy > 0) {
		--elapsed_time_seconds_copy;
		--m_larva_spawn_remaining_duration;
		if (m_larva_spawn_remaining_duration == 0) {
			m_units.push_back(std::shared_ptr<Unit>(new Unit(*find_unit_blueprint_by_name("larva"))));
			m_larva_spawn_remaining_duration = c_larva_spawn_duration;
		}
	}

	return Game::update(elapsed_time_seconds);
}
