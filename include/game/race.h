#pragma once 

#include <string>

enum class Race {
	Protoss,
	Terran,
	Zerg
};

std::string get_race_string(const Race& race);
