#pragma once 

#include "io/output_formatter.h"

#include <list>
#include <memory>
#include <string>

class Game;

class ForwardSimulator {
public:
	explicit ForwardSimulator(const Game& game);
	ForwardSimulator(const ForwardSimulator& forward_simulator);
	ForwardSimulator& operator=(const ForwardSimulator& forward_simulator);

	const Game& get_game() const;
	bool is_successful() const;
	unsigned int get_time() const;
	const OutputFormatter& get_output_formatter() const;

	void simulate(const std::list<std::string>& build_list, unsigned int max_time);
private:
	std::unique_ptr<Game> m_game;
	bool m_successful;
	unsigned int m_time;
	OutputFormatter m_output_formatter;
};
