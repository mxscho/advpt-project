#include "game/energetic.h"

#include "game/blueprint.h"

const unsigned int Energetic::c_energy_increase_rate = 562500;

Energetic::Energetic(const Blueprint& blueprint)
	: m_blueprint(blueprint),
	m_energy(m_blueprint.get_start_energy() * 1000000) {
}

unsigned int Energetic::get_energy() {
	return m_energy / 1000000;
}

std::list<std::unique_ptr<Event>> Energetic::update(unsigned int elapsed_time_seconds) {
	m_energy += elapsed_time_seconds * c_energy_increase_rate;
	unsigned int max_energy = m_blueprint.get_max_energy() * 1000000;
	if (m_energy > max_energy) {
		m_energy = max_energy;
	}
	return {};
}
