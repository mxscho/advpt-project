#pragma once

#include <boost/property_tree/ptree.hpp>

#include "game/game.h"

using boost::property_tree::ptree;

class OutputFormatter {
public:
	OutputFormatter(std::string game);

	void add_event(unsigned int time, Game& game, const std::list<std::unique_ptr<Event>>& events);
	//void initialize();
	void print();
	void set_validity(bool valid);

private:
	ptree output;
	ptree messages;
};