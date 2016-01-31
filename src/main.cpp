#include "io/config_parser.h"
#include "simulation/forward_simulator.h"
#include "simulation/genetic.h"

#include <string>

void do_forward_simulation(Game& game, std::list<std::string> build_list) {
	ForwardSimulator forward_simulator(game);
	forward_simulator.simulate(build_list, 1000);
	forward_simulator.get_output_formatter().print();
}
void do_optimization_rush(const string& unit, Game& game, const string& race) {
	// 6 minutes time limit to gain as many units as possible.
	// unsigned int Time = 0; // in seconds
	// unsigned int targetTime = 6 * 60; // in seconds
	Genetic pushgen(unit, game, 0, race);
}
void do_optimization_push(const string& unit, Game& game, const string& race) {
	// Fastest way to achive a unit.
	Genetic pushgen(unit, game, 1, race);

}
void do_optimization(Game& game, const string& task) {
	if (task.compare("terran_rush_A") == 0) {
		do_optimization_rush("marine", game, "terran");
	} else if (task.compare("terran_rush_B") == 0) {
		do_optimization_rush("marauders", game, "terran");
	} else if (task.compare("terran_push_A") == 0) {
		do_optimization_push("battlecruiser", game, "terran");
	} else if (task.compare("terran_push_B") == 0) {
		do_optimization_push("tank", game, "terran");
	} else if (task.compare("protoss_rush_A") == 0) {
		do_optimization_rush("zealots", game, "protoss");
	} else if (task.compare("protoss_rush_B") == 0) {
		do_optimization_rush("stalker", game, "protoss");
	} else if (task.compare("protoss_push_A") == 0) {
		do_optimization_push("void_ray", game, "protoss");
	} else if (task.compare("protoss_push_B") == 0) {
		do_optimization_push("colossus", game, "protoss");
	} else if (task.compare("zerg_rush_A") == 0) {
		do_optimization_rush("zerglings", game, "zerg");
	} else if (task.compare("zerg_rush_B") == 0) {
		do_optimization_rush("roaches", game, "zerg");
	} else if (task.compare("zerg_push_A") == 0) {
		do_optimization_push("brood_lord", game, "zerg");
	} else if (task.compare("zerg_push_B") == 0) {
		do_optimization_push("ultralisk", game, "zerg");
	}
}

int main(int argc, char** argv) {
	std::unique_ptr<Game> game = ConfigParser::create_game(argc, argv);
	if (argc == 5) {
		// Example Parameter: ../config.csv ../techtrees.csv sc2-hots-terran ../terran1.txt
		std::list<std::string> build_list = ConfigParser::create_build_list(argc, argv);
		if (build_list.size() > 0) {
			do_forward_simulation(*game, build_list);
		}
	} else if (argc == 4) {
		// Example Parameter: ../config.csv ../techtrees.csv terran_rush_A
		do_optimization(*game, argv[3]);
	}

#ifdef _WIN32
	system("PAUSE");
#endif
}
