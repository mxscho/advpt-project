#include "io/config_parser.h"

#include "debug.h"
#include "game/protoss_game.h"
#include "game/terran_game.h"
#include "game/zerg_game.h"

#include <fstream>

using namespace std;

std::unique_ptr<TerranGame> Terraninitialize(const std::string& race, char* configfilename, char* techtreefilename) {
	ifstream file;
	char    line[1024];

	//Allgmeine Config Informationen
	unsigned int mineral_count = 0;
	unsigned int vespene_gas_count = 0;
	unsigned int worker = 0;
	unsigned int basebuilding = 0;
	//Parser Config File 


	// Config Datei öffnen	
	file.open(configfilename, ios::in);
	if (file.good()) {
		while (!file.eof()) {
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			if (temp.length() < 2) {
				continue;
			}
			if (temp.compare(2, 7, "General") == 0) {
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // Mineralien Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				mineral_count = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); // Vespene Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				vespene_gas_count = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); //Worker Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				worker = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); //BaseBuilding Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				basebuilding = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();

			} else if (temp.compare(2, 4, "Terran") == 0 && race.compare("sc2-hots-terran") == 0) {
				// Hier kommt alles rein was in der Config die Terraner betrifft, bisher nichts
			}
		}
		file.close();
	} else {
		LOG_ERROR("Config-File not found");

	}
	//Terran game erstellen
	std::unique_ptr<TerranGame> game_setup(new TerranGame(mineral_count, vespene_gas_count));
	//Einlesen der Techtrees
	if (race.compare("sc2-hots-terran") == 0) {

		for (unsigned int runs = 0; runs < 2; runs++) {
			file.open(techtreefilename, ios::in);
			if (file.good()) {
				while (!file.eof()) {
					file.getline(line, 1024);
					std::string temp = line;
					if (temp.length() < 1) {
						continue;
					}
					if (temp.compare(1, 6, "Terran") == 0) {
						file.getline(line, 1024); //### zeile überspringen
						file.getline(line, 1024); //  erste Unit einlesen
						temp = line;
						while (temp.compare(1, 9, "buildings") != 0) {
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0) {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supcosttemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int productmaxtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int minharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vespaharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								bool isbuildertemp = false;
								unsigned int lifedurtemp = 0;
								if (minharvtemp != 0 && vespaharvtemp != 0) {
									isbuildertemp = true;
								}



								game_setup->add_unit_blueprint(std::shared_ptr<UnitBlueprint>(new UnitBlueprint(
									Race::Terran, // Race
									nametemp, // Name
									mineraltemp, // Mineral costs
									vesptemp, // Vespene gas costs
									{}, // Dependency blueprints
									buildtimetemp, // Creation duration in seconds
									lifedurtemp, // Life duration in seconds (0 = infinite)
									startentemp, // Start energy
									maxenergtemp, // Max energy
									supprovtemp, // Supply provided
									{}, // Morphable blueprints
									supcosttemp, // Supply costs,
									minharvtemp, // Mineral collection rate in ms
									vespaharvtemp, // Vespine gas collection rate in ms
									isbuildertemp))); // Is builder
							} else {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));



								//dependency

								std::string  dependtemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), dependtemp.length() + 1);

								//Produced by
								std::string  prodbytemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), prodbytemp.length() + 1);

								//morph

								dummytemp = temp.substr(temp.find_last_of(",") + 1);
								unsigned int morphtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));

								postemp = 0;
								while (postemp < dependtemp.length()) {
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_unit_blueprint_by_name(nametemp)->add_dependency_blueprint(*game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0) {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										game_setup->find_building_blueprint_by_name(dumtemp)->add_producible_unit_blueprint(*game_setup->find_unit_blueprint_by_name(nametemp));
									}
								} else {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										if (UnitBlueprint* unit_blueprint = game_setup->find_unit_blueprint_by_name(dumtemp)) {
											unit_blueprint->add_morphable_blueprints({ *game_setup->find_unit_blueprint_by_name(nametemp) });
										} else {
											game_setup->find_building_blueprint_by_name(dumtemp)->add_morphable_building_blueprint({ *game_setup->find_building_blueprint_by_name(nametemp) });
										}

									}
								}
							}

							temp.clear();
							file.getline(line, 1024); //  Units einlesen
							temp = line;
						}

						if (runs == 0) {
							/////////////////////////////Special Terran MULE Beginn
							game_setup->add_unit_blueprint(std::shared_ptr<UnitBlueprint>(new UnitBlueprint(
								Race::Terran, // Race
								"Mule", // Name
								0, // Mineral costs
								0, // Vespene gas costs
								{}, // Dependency blueprints
								0, // Creation duration in seconds
								90, // Life duration in seconds (0 = infinite)
								0, // Start energy
								0, // Max energy
								0, // Supply provided
								{}, // Morphable blueprints
								0, // Supply costs,
								4 * 700, // Mineral collection rate
								4 * 350, // Vespine gas collection rate
								false))); // Is builder
							/////////////////////////////Special Terran MULE End
						}
						file.getline(line, 1024); // erstes Gebäude einlesen
						temp = line;
						//Ab hier beginnt das einlesen der Gebäude
						while (temp.compare(0, 1, "#") != 0 && temp.size() > 0) {
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0) {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int supcosttemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int productmaxtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int minharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int vespaharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								unsigned int lifedurtemp = 0;

								game_setup->add_building_blueprint(std::shared_ptr<BuildingBlueprint>(new BuildingBlueprint(
									Race::Terran, // Race
									nametemp, // Name
									mineraltemp, // Mineral costs
									vesptemp, // Vespene gas costs
									{}, // Dependency blueprints
									buildtimetemp, // Creation duration in seconds
									lifedurtemp, // Life duration in seconds (0 = infinite)
									startentemp, // Start energy
									maxenergtemp, // Max energy
									supprovtemp, // Supply provided
									{}, // Morphable blueprints
									{}, // Producible unit blueprints
									productmaxtemp))); // Max concurrent unit production count
							} else {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));



								//dependency

								std::string  dependtemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), dependtemp.length() + 1);

								//Produced by
								std::string  prodbytemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), prodbytemp.length() + 1);

								//morph
								dummytemp = temp.substr(temp.find_last_of(",") + 1);
								unsigned int morphtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));

								postemp = 0;
								while (postemp < dependtemp.length()) {
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_building_blueprint_by_name(nametemp)->add_dependency_blueprint(*game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0) {
									//while (postemp<prodbytemp.length())
									//	{
									//std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
									//	postemp += dumtemp.length() + 1;
									//game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									//}
								} else {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;

										if (BuildingBlueprint* building_blueprint = game_setup->find_building_blueprint_by_name(dumtemp)) {
											building_blueprint->add_morphable_building_blueprint({ *game_setup->find_building_blueprint_by_name(nametemp) });
										} else {
											game_setup->find_unit_blueprint_by_name(dumtemp)->add_morphable_blueprints({ *game_setup->find_building_blueprint_by_name(nametemp) });
										}
									}
								}

							}
							temp.clear();
							file.getline(line, 1024); //  Buildings einlesen
							temp = line;

						}
					}
				}
			}
			file.close();

		}
		for (unsigned int i = 0; i < worker; i++) {
			game_setup->add_unit_by_name("scv");
		}
		for (unsigned int a = 0; a < basebuilding; a++) {
			game_setup->add_building_by_name("command_center");
		}

	}

	else {
		LOG_ERROR("No such Race");

	}
	return game_setup;


}

std::unique_ptr<ProtossGame> Protossinitialize(const std::string& race, char* configfilename, char* techtreefilename) {
	ifstream file;
	char    line[1024];

	//Allgmeine Config Informationen
	unsigned int mineral_count = 0;
	unsigned int vespene_gas_count = 0;
	unsigned int worker = 0;
	unsigned int basebuilding = 0;
	//Parser Config File 

	// Config Datei öffnen	
	file.open(configfilename, ios::in);
	if (file.good()) {
		while (!file.eof()) {
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			if (temp.length() < 2) {
				continue;
			}
			if (temp.compare(2, 7, "General") == 0) {
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // Mineralien Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				mineral_count = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); // Vespene Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				vespene_gas_count = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); //Worker Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				worker = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); //BaseBuilding Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				basebuilding = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();



			} else if (temp.compare(2, 4, "Protoss") == 0 && race.compare("sc2-hots-protoss") == 0) {
				// Hier kommt alles rein was in der Config die Protoss betrifft, bisher nichts
			}
		}
		file.close();
	} else {
		LOG_ERROR("Config-File not found");

	}
	std::unique_ptr<ProtossGame> game_setup(new ProtossGame(mineral_count, vespene_gas_count)); //Protoss game erstellen
	//Einlesen der Techtrees

	if (race.compare("sc2-hots-protoss") == 0) {

		for (unsigned int runs = 0; runs < 2; runs++) {
			file.open(techtreefilename, ios::in);
			if (file.good()) {
				while (!file.eof()) {
					file.getline(line, 1024);
					std::string temp = line;
					if (temp.length() < 1) {
						continue;
					}
					if (temp.compare(1, 7, "Protoss") == 0) {
						file.getline(line, 1024); //### zeile überspringen
						file.getline(line, 1024); //  erste Unit einlesen
						temp = line;
						while (temp.compare(1, 9, "buildings") != 0) {
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0) {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supcosttemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int productmaxtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int minharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vespaharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								bool isbuildertemp = false;
								unsigned int lifedurtemp = 0;
								if (minharvtemp != 0 && vespaharvtemp != 0) {
									isbuildertemp = true;
								}



								game_setup->add_unit_blueprint(std::shared_ptr<UnitBlueprint>(new UnitBlueprint(
									Race::Protoss, // Race
									nametemp, // Name
									mineraltemp, // Mineral costs
									vesptemp, // Vespene gas costs
									{}, // Dependency blueprints
									buildtimetemp, // Creation duration in seconds
									lifedurtemp, // Life duration in seconds (0 = infinite)
									startentemp, // Start energy
									maxenergtemp, // Max energy
									supprovtemp, // Supply provided
									{}, // Morphable blueprints
									supcosttemp, // Supply costs,
									minharvtemp, // Mineral collection rate in ms
									vespaharvtemp, // Vespine gas collection rate in ms
									isbuildertemp))); // Is builder
							} else {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));



								//dependency

								std::string  dependtemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), dependtemp.length() + 1);

								//Produced by
								std::string  prodbytemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), prodbytemp.length() + 1);

								//morph

								dummytemp = temp.substr(temp.find_last_of(",") + 1);
								unsigned int morphtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));

								postemp = 0;
								while (postemp < dependtemp.length()) {
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_unit_blueprint_by_name(nametemp)->add_dependency_blueprint(*game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0) {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										game_setup->find_building_blueprint_by_name(dumtemp)->add_producible_unit_blueprint(*game_setup->find_unit_blueprint_by_name(nametemp));
									}
								} else {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										if (UnitBlueprint* unit_blueprint = game_setup->find_unit_blueprint_by_name(dumtemp)) {
											unit_blueprint->add_morphable_blueprints({ *game_setup->find_unit_blueprint_by_name(nametemp) });
										} else {
											game_setup->find_building_blueprint_by_name(dumtemp)->add_morphable_building_blueprint({ *game_setup->find_building_blueprint_by_name(nametemp) });
										}
									}
								}
							}

							temp.clear();
							file.getline(line, 1024); //  Units einlesen
							temp = line;
						}
						file.getline(line, 1024); // erstes Gebäude einlesen
						temp = line;
						//Ab hier beginnt das einlesen der Gebäude
						while (temp.compare(0, 1, "#") != 0 && temp.size() > 0) {
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0) {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int supcosttemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int productmaxtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int minharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int vespaharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								unsigned int lifedurtemp = 0;

								game_setup->add_building_blueprint(std::shared_ptr<BuildingBlueprint>(new BuildingBlueprint(
									Race::Protoss, // Race
									nametemp, // Name
									mineraltemp, // Mineral costs
									vesptemp, // Vespene gas costs
									{}, // Dependency blueprints
									buildtimetemp, // Creation duration in seconds
									lifedurtemp, // Life duration in seconds (0 = infinite)
									startentemp, // Start energy
									maxenergtemp, // Max energy
									supprovtemp, // Supply provided
									{}, // Morphable blueprints
									{}, // Producible unit blueprints
									productmaxtemp))); // Max concurrent unit production count
							} else {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));



								//dependency

								std::string  dependtemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), dependtemp.length() + 1);

								//Produced by
								std::string  prodbytemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), prodbytemp.length() + 1);

								//morph
								dummytemp = temp.substr(temp.find_last_of(",") + 1);
								unsigned int morphtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));

								postemp = 0;
								while (postemp < dependtemp.length()) {
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_building_blueprint_by_name(nametemp)->add_dependency_blueprint(*game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0) {
									//while (postemp<prodbytemp.length())
									//	{
									//std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
									//	postemp += dumtemp.length() + 1;
									//game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									//}
								} else {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										if (UnitBlueprint* unit_blueprint = game_setup->find_unit_blueprint_by_name(dumtemp)) {
											unit_blueprint->add_morphable_blueprints({ *game_setup->find_building_blueprint_by_name(nametemp) });
										} else {
											game_setup->find_building_blueprint_by_name(dumtemp)->add_morphable_building_blueprint({ *game_setup->find_building_blueprint_by_name(nametemp) });
										}
									}
								}

							}
							temp.clear();
							file.getline(line, 1024); //  Buildings einlesen
							temp = line;

						}
					}
				}
			}
			file.close();

		}
		for (unsigned int i = 0; i < worker; i++) {
			game_setup->add_unit_by_name("probe");
		}
		for (unsigned int a = 0; a < basebuilding; a++) {
			game_setup->add_building_by_name("nexus");
		}

	} else {
		LOG_ERROR("No such Race");

	}
	return game_setup;
}

std::unique_ptr<ZergGame> Zerginitialize(const std::string& race, char* configfilename, char* techtreefilename) {
	ifstream file;
	char    line[1024];

	//Allgmeine Config Informationen
	unsigned int mineral_count = 0;
	unsigned int vespene_gas_count = 0;
	unsigned int worker = 0;
	unsigned int basebuilding = 0;
	//Zerg Specifische Config informationen
	unsigned int overlord = 0;
	unsigned int larvae = 0;

	//Parser Config File 


	// Config Datei öffnen	
	file.open(configfilename, ios::in);
	if (file.good()) {
		while (!file.eof()) {
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			if (temp.length() < 2) {
				continue;
			}
			if (temp.compare(2, 7, "General") == 0) {
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // Mineralien Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				mineral_count = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); // Vespene Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				vespene_gas_count = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); //Worker Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				worker = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); //BaseBuilding Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				basebuilding = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();


			} else if (temp.compare(2, 4, "Zerg") == 0 && race.compare("sc2-hots-zerg") == 0) {
				// Hier kommt alles rein was in der Config die Zerg betrifft
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // overlord Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				overlord = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();
				file.getline(line, 1024); // larvae Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				larvae = static_cast<unsigned int>(strtol(temp.c_str(), nullptr, 10));
				temp.clear();

			}
		}
		file.close();
	} else {
		LOG_ERROR("Config-File not found");

	}
	std::unique_ptr<ZergGame> game_setup(new ZergGame(mineral_count, vespene_gas_count)); //Zerg game erstellen
	//Einlesen der Techtrees	
	if (race.compare("sc2-hots-zerg") == 0) {

		for (unsigned int runs = 0; runs < 2; runs++) {
			file.open(techtreefilename, ios::in);
			if (file.good()) {
				while (!file.eof()) {
					file.getline(line, 1024);
					std::string temp = line;

					if (temp.length() < 1) {
						continue;
					}
					if (temp.compare(1, 4, "Zerg") == 0) {
						file.getline(line, 1024); //### zeile überspringen
						file.getline(line, 1024); //  erste Unit einlesen
						temp = line;
						while (temp.compare(1, 9, "buildings") != 0) {
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0) {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supcosttemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int productmaxtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int minharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vespaharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								bool isbuildertemp = false;
								unsigned int lifedurtemp = 0;
								if (minharvtemp != 0 && vespaharvtemp != 0) {
									isbuildertemp = true;
								}



								game_setup->add_unit_blueprint(std::shared_ptr<UnitBlueprint>(new UnitBlueprint(
									Race::Zerg, // Race
									nametemp, // Name
									mineraltemp, // Mineral costs
									vesptemp, // Vespene gas costs
									{}, // Dependency blueprints
									buildtimetemp, // Creation duration in seconds
									lifedurtemp, // Life duration in seconds (0 = infinite)
									startentemp, // Start energy
									maxenergtemp, // Max energy
									supprovtemp, // Supply provided
									{}, // Morphable blueprints
									supcosttemp, // Supply costs,
									minharvtemp, // Mineral collection rate in ms
									vespaharvtemp, // Vespine gas collection rate in ms
									isbuildertemp))); // Is builder
							} else {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));



								//dependency

								std::string  dependtemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), dependtemp.length() + 1);

								//Produced by
								std::string  prodbytemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), prodbytemp.length() + 1);

								//morph

								dummytemp = temp.substr(temp.find_last_of(",") + 1);
								unsigned int morphtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));

								postemp = 0;
								while (postemp < dependtemp.length()) {
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_unit_blueprint_by_name(nametemp)->add_dependency_blueprint(*game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0) {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										game_setup->find_building_blueprint_by_name(dumtemp)->add_producible_unit_blueprint(*game_setup->find_unit_blueprint_by_name(nametemp));
									}
								} else {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;

										if (UnitBlueprint* unit_blueprint = game_setup->find_unit_blueprint_by_name(dumtemp)) {
											if (morphtemp == 1) {
												unit_blueprint->add_morphable_blueprints({ *game_setup->find_unit_blueprint_by_name(nametemp) });
											} else {
												unit_blueprint->add_morphable_blueprints({ *game_setup->find_unit_blueprint_by_name(nametemp), *game_setup->find_unit_blueprint_by_name(nametemp) });
											}
										} else {
											game_setup->find_building_blueprint_by_name(dumtemp)->add_morphable_building_blueprint({ *game_setup->find_building_blueprint_by_name(nametemp) });
										}
									}
								}
							}

							temp.clear();
							file.getline(line, 1024); //  Units einlesen
							temp = line;
						}
						file.getline(line, 1024); // erstes Gebäude einlesen
						temp = line;
						//Ab hier beginnt das einlesen der Gebäude
						while (temp.compare(0, 1, "#") != 0 && temp.size() > 0) {
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0) {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int supcosttemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int productmaxtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//	unsigned int minharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int vespaharvtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								unsigned int lifedurtemp = 0;

								game_setup->add_building_blueprint(std::shared_ptr<BuildingBlueprint>(new BuildingBlueprint(
									Race::Zerg, // Race
									nametemp, // Name
									mineraltemp, // Mineral costs
									vesptemp, // Vespene gas costs
									{}, // Dependency blueprints
									buildtimetemp, // Creation duration in seconds
									lifedurtemp, // Life duration in seconds (0 = infinite)
									startentemp, // Start energy
									maxenergtemp, // Max energy
									supprovtemp, // Supply provided
									{}, // Morphable blueprints
									{}, // Producible unit blueprints
									productmaxtemp))); // Max concurrent unit production count
							} else {

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));



								//dependency

								std::string  dependtemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), dependtemp.length() + 1);

								//Produced by
								std::string  prodbytemp = temp.substr(temp.find_last_of(",") + 1);
								temp.erase(temp.find_last_of(","), prodbytemp.length() + 1);

								//morph
								dummytemp = temp.substr(temp.find_last_of(",") + 1);
								unsigned int morphtemp = static_cast<unsigned int>(strtol(dummytemp.c_str(), nullptr, 10));

								postemp = 0;
								while (postemp < dependtemp.length()) {
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_building_blueprint_by_name(nametemp)->add_dependency_blueprint(*game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0) {
									//while (postemp<prodbytemp.length())
									//	{
									//std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
									//	postemp += dumtemp.length() + 1;
									//game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									//}
								} else {
									while (postemp < prodbytemp.length()) {
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										if (UnitBlueprint* unit_blueprint = game_setup->find_unit_blueprint_by_name(dumtemp)) {
											unit_blueprint->add_morphable_blueprints({ *game_setup->find_building_blueprint_by_name(nametemp) });
										} else {
											game_setup->find_building_blueprint_by_name(dumtemp)->add_morphable_building_blueprint({ *game_setup->find_building_blueprint_by_name(nametemp) });
										}
									}
								}

							}
							temp.clear();
							file.getline(line, 1024); //  Buildings einlesen
							temp = line;

						}
					}
				}
			}
			file.close();

		}
		for (unsigned int i = 0; i < worker; i++) {
			game_setup->add_unit_by_name("drone");
		}
		for (unsigned int a = 0; a < basebuilding; a++) {
			game_setup->add_building_by_name("hatchery");
		}
		for (unsigned int b = 0; b < overlord; b++) {
			game_setup->add_unit_by_name("overlord");
		}
		for (unsigned int c = 0; c < larvae; c++) {
			game_setup->add_unit_by_name("larva");
		}

	} else {
		LOG_ERROR("No such Race");

	}
	return game_setup;


}

std::unique_ptr<Game> ConfigParser::create_game(int argc, char** argv) {
	if (argc == 5) {
		const std::string& race = argv[3];

		if (race.compare("sc2-hots-terran") == 0) {
			return Terraninitialize(argv[3], argv[1], argv[2]);
		} else if (race.compare("sc2-hots-protoss") == 0) {
			return Protossinitialize(argv[3], argv[1], argv[2]);
		} else if (race.compare("sc2-hots-zerg") == 0) {
			return Zerginitialize(argv[3], argv[1], argv[2]);
		} else {
			LOG_ERROR("Wrong Race Name, pls use sc2-hots-terran, sc2-hots-protoss or  sc2-hots-zerg");
			return std::unique_ptr<Game>();
		}
	} else if (argc == 4) {
		const std::string& race = argv[3];
		if (race.compare("terran_rush_A") == 0 || race.compare("terran_rush_B") == 0 || race.compare("terran_push_A") == 0 || race.compare("terran_push_B") == 0) {
			return Terraninitialize("sc2-hots-terran", argv[1], argv[2]);
		} else if (race.compare("protoss_rush_A") == 0 || race.compare("protoss_rush_B") == 0 || race.compare("protoss_push_A") == 0 || race.compare("protoss_push_B") == 0) {
			return Protossinitialize("sc2-hots-protoss", argv[1], argv[2]);
		} else if (race.compare("zerg_rush_A") == 0 || race.compare("zerg_rush_B") == 0 || race.compare("zerg_push_A") == 0 || race.compare("zerg_push_B") == 0) {
			return Zerginitialize("sc2-hots-zerg", argv[1], argv[2]);
		} else {
			LOG_ERROR("Wrong Parameter , pls use terran_rush_A, terran_rush_B or  other");
			return std::unique_ptr<Game>();
		}
	} else {
		LOG_ERROR("Wrong Number of Parameters");
		return std::unique_ptr<Game>();
	}
}

std::list<std::string> ConfigParser::create_build_list(int argc, char** argv) {
	if (argc != 5) {
		return std::list<std::string>();
	}
	std::list<std::string> buildlist; //Die eingelesene Liste mit den Aufträgen
	ifstream file;
	char    line[1024];
	file.open(argv[4], ios::in);
	if (file.good()) {
		while (!file.eof()) {
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			buildlist.push_back(temp);
		}
		file.close();
		buildlist.remove_if([](const string& buildlist_entry) { return buildlist_entry == ""; });
		return buildlist;
	} else {
		LOG_ERROR("Error couldn't read Buildlist");
		return std::list<std::string>();
	}
}
