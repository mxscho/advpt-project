#pragma once

#include <list>
#include <memory>
#include <string>

class Game;
class ProtossGame;
class TerranGame;
class ZergGame;

std::unique_ptr<TerranGame> Terraninitialize(const std::string& race, char* configfilename, char* techtreefilename);
std::unique_ptr<ProtossGame> Protossinitialize(const std::string& race, char* configfilename, char* techtreefilename);
std::unique_ptr<ZergGame> Zerginitialize(const std::string& race, char* configfilename, char* techtreefilename);

class ConfigParser {
public:
	static std::unique_ptr<Game> create_game(int argc, char** argv);
	static std::list<std::string> create_build_list(int argc, char** argv);
};
