#pragma once

#include <boost/property_tree/ptree.hpp>

#include "game/game.h"

enum class Race;

using boost::property_tree::ptree;

class OutputFormatter {
public:
	OutputFormatter(const Race& race);

	void add_event(unsigned int time, Game& game, const std::list<std::unique_ptr<Event>>& events);
	//void initialize();
	void print() const;
	void set_validity(bool valid);

private:
	Race race;
	bool validity;
	ptree messages;
};