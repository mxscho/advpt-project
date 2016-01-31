#include "simulation/forward_simulator.h"

ForwardSimulator::ForwardSimulator(const Game& game)
	: m_game(std::unique_ptr<Game>(game.clone())),
	m_successful(true),
	m_time(0),
	m_output_formatter(m_game->get_race()) {
}
ForwardSimulator::ForwardSimulator(const ForwardSimulator& forward_simulator)
	: m_game(std::unique_ptr<Game>(forward_simulator.get_game().clone())),
	m_successful(forward_simulator.m_successful),
	m_time(forward_simulator.m_time),
	m_output_formatter(forward_simulator.m_output_formatter) {
}
ForwardSimulator& ForwardSimulator::operator= (const ForwardSimulator& forward_simulator) {
	if (this == &forward_simulator) {
		return *this;
	}

	m_game = std::unique_ptr<Game>(forward_simulator.get_game().clone());
	m_successful = forward_simulator.m_successful;
	m_time = forward_simulator.m_time;
	m_output_formatter = forward_simulator.m_output_formatter;

	return *this;
}

const Game& ForwardSimulator::get_game() const {
	return *m_game;
}
bool ForwardSimulator::is_successful() const {
	return m_successful;
}
unsigned int ForwardSimulator::get_time() const {
	return m_time;
}
const OutputFormatter& ForwardSimulator::get_output_formatter() const {
	return m_output_formatter;
}

void ForwardSimulator::simulate(const std::list<std::string>& build_list, unsigned int max_time) {
	if (build_list.size() == 0) {
		return;
	}

	m_game->set_worker_unit_allocation_function([](const Game& game, const std::list<std::shared_ptr<Unit>>& worker_units) {
		std::list<std::shared_ptr<Unit>> vespene_gas_collecting_worker_units;
		std::copy_if(worker_units.begin(), worker_units.end(), std::back_inserter(vespene_gas_collecting_worker_units), [](const std::shared_ptr<Unit>& worker_unit) {
			return worker_unit->get_unit_blueprint().get_vespene_gas_collection_rate() != 0;
		});
		std::list<std::shared_ptr<Unit>> actual_vespene_gas_collecting_worker_units;
		for (auto i_building = game.get_buildings().begin(); i_building != game.get_buildings().end(); ++i_building) {
			if ((*i_building)->get_building_blueprint().get_name() == "refinery" ||
				(*i_building)->get_building_blueprint().get_name() == "assimilator" ||
				(*i_building)->get_building_blueprint().get_name() == "extractor") {
				unsigned int count = 0;
				while (vespene_gas_collecting_worker_units.size() > 1 && count < 3) {
					actual_vespene_gas_collecting_worker_units.push_back(vespene_gas_collecting_worker_units.front());
					vespene_gas_collecting_worker_units.pop_front();
					++count;
				}
			}
		}
		return WorkerUnitAllocation(vespene_gas_collecting_worker_units, actual_vespene_gas_collecting_worker_units);
	});
	std::list<std::string> local_build_list = build_list;
	m_game->update(0);
	while (m_time <= max_time) {
		std::list<std::unique_ptr<Event>> events = m_game->update(1);
		++m_time;
		if (local_build_list.size() > 0) {
			if (m_game->can_construct_buildings_by_names({ local_build_list.front() })) {
				m_game->construct_buildings_by_names({ local_build_list.front() });
				local_build_list.pop_front();
			} else if (m_game->can_produce_units_by_names({ local_build_list.front() })) {
				m_game->produce_units_by_names({ local_build_list.front() });
				local_build_list.pop_front();
			}
			events.splice(events.end(), m_game->update(0));
		}
		if (events.size() > 0) {
			m_output_formatter.add_event(m_time, *m_game, events);
		}
		if (local_build_list.size() == 0 && !m_game->is_busy()) {
			break;
		}
	}
	m_output_formatter.set_validity(m_time <= max_time);
}
