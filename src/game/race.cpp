#include "game/race.h"

std::string get_race_string(const Race& race) {
	switch (race) {
		case Race::Protoss:
			return "sc2-hots-protoss";
		case Race::Terran:
			return "sc2-hots-terran";
		case Race::Zerg:
		default:
			return "sc2-hots-zerg";
	}
}
