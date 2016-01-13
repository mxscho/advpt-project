#include "debug.h"

#include "game/building_blueprint.h"
#include "game/events/unit_production_finish_event.h"
#include "game/events/unit_production_start_event.h"
#include "game/events/building_construction_finish_event.h"
#include "game/events/building_construction_start_event.h"
#include "game/protoss_game.h"
#include "game/terran_game.h"
#include "game/unit_blueprint.h"
#include "game/zerg_game.h"

#include <iostream>
#include <fstream>

using namespace std;

std::unique_ptr<TerranGame> Terraninitialize(const std::string& race) {
	LOG_INFO("Default log temp.");
	LOG_DEBUG("Debug log temp.");
	LOG_ERROR("Error log temp.");

	char     configfilename[128] = "../config.csv";
	char     techtreefilename[128] = "../techtrees.csv";
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
	if (file.good())
	{
		while (!file.eof())
		{
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			if (temp.length() < 2) {
				continue;
			}
			if (temp.compare(2, 7, "General") == 0)
			{
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // Mineralien Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				mineral_count = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); // Vespene Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				vespene_gas_count = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); //Worker Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				worker = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); //BaseBuilding Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				basebuilding = std::stoi(temp);
				temp.clear();

			}
			else if (temp.compare(2, 4, "Terran") == 0 && race.compare("sc2-hots-terran") == 0)
			{
				// Hier kommt alles rein was in der Config die Terraner betrifft, bisher nichts
			}			
		}
		file.close();
	}
	else
	{
		LOG_ERROR("Config-File not found");

	}
	//Terran game erstellen
	std::unique_ptr<TerranGame> game_setup(new TerranGame(mineral_count, vespene_gas_count));
	//Einlesen der Techtrees
	if (race.compare("sc2-hots-terran") == 0)
	{
		
		for (unsigned int runs = 0;runs < 2;runs++)
		{
			file.open(techtreefilename, ios::in);
			if (file.good())
			{
				while (!file.eof())
				{
					file.getline(line, 1024);
					std::string temp = line;

					if (temp.compare(1, 6, "Terran") == 0)
					{
						file.getline(line, 1024); //### zeile überspringen
						file.getline(line, 1024); //  erste Unit einlesen
						temp = line;
						while (temp.compare(1, 9, "buildings") != 0)
						{
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0)
							{

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supcosttemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int productmaxtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int minharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vespaharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								bool isbuildertemp = false;
								unsigned int lifedurtemp = 0;
								if (minharvtemp != 0 && vespaharvtemp != 0)
								{
									isbuildertemp = true;
								}



								game_setup->add_unit_blueprint(UnitBlueprint(
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
									isbuildertemp)); // Is builder
							}
							else
							{

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
								unsigned int morphtemp = std::stoi(dummytemp);

								postemp = 0;
								while (postemp<dependtemp.length())
								{
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_unit_blueprint_by_name(nametemp).add_dependency_blueprint(game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0)
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									}
								}
								else
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										try
										{
											game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_unit_blueprint_by_name(nametemp) });
										}
										catch (const std::out_of_range)
										{
											game_setup->find_building_blueprint_by_name(dumtemp).add_morphable_building_blueprint({ game_setup->find_building_blueprint_by_name(nametemp) });
										}

									}
								}
							}

							temp.clear();
							file.getline(line, 1024); //  Units einlesen
							temp = line;
						}

						if (runs == 0)
						{
							/////////////////////////////Special Terran MULE Beginn
							game_setup->add_unit_blueprint(UnitBlueprint(
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
								false)); // Is builder
										 /////////////////////////////Special Terran MULE End
						}
						file.getline(line, 1024); // erstes Gebäude einlesen
						temp = line;
						//Ab hier beginnt das einlesen der Gebäude
						while (temp.compare(0, 1, "#") != 0 && temp.size()>0)
						{
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0)
							{

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int supcosttemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int productmaxtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int minharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int vespaharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								unsigned int lifedurtemp = 0;

								game_setup->add_building_blueprint(BuildingBlueprint(
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
									productmaxtemp)); // Max concurrent unit production count
							}
							else
							{

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
								unsigned int morphtemp = std::stoi(dummytemp);

								postemp = 0;
								while (postemp<dependtemp.length())
								{
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_building_blueprint_by_name(nametemp).add_dependency_blueprint(game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0)
								{
									//while (postemp<prodbytemp.length())
									//	{
									//std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
									//	postemp += dumtemp.length() + 1;
									//game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									//}
								}
								else
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;

										try
										{
											game_setup->find_building_blueprint_by_name(dumtemp).add_morphable_building_blueprint({ game_setup->find_building_blueprint_by_name(nametemp) });
										}
										catch (const std::out_of_range)
										{

											game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_unit_blueprint_by_name(nametemp) });
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
		for (unsigned int i = 0;i<worker;i++)
		{
			game_setup->add_unit_by_name("scv");
		}
		for (unsigned int a = 0;a < basebuilding;a++)
		{
			game_setup->add_building_by_name("command_center");
		}
		
	}
	
	else
	{
		LOG_ERROR("No such Race");

	}
	return game_setup;


}

std::unique_ptr<ProtossGame> Protossinitialize(const std::string& race) {
	LOG_INFO("Default log temp.");
	LOG_DEBUG("Debug log temp.");
	LOG_ERROR("Error log temp.");

	char     configfilename[128] = "../config.csv";
	char     techtreefilename[128] = "../techtrees.csv";
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
	if (file.good())
	{
		while (!file.eof())
		{
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			if (temp.compare(2, 7, "General") == 0)
			{
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // Mineralien Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				mineral_count = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); // Vespene Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				vespene_gas_count = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); //Worker Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				worker = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); //BaseBuilding Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				basebuilding = std::stoi(temp);
				temp.clear();

				

			}
			else if (temp.compare(2, 4, "Protoss") == 0 && race.compare("sc2-hots-protoss") == 0)
			{
				// Hier kommt alles rein was in der Config die Protoss betrifft, bisher nichts
			}			
		}
		file.close();
	}
	else
	{
		LOG_ERROR("Config-File not found");

	}
	std::unique_ptr<ProtossGame> game_setup(new ProtossGame(mineral_count, vespene_gas_count)); //Protoss game erstellen
	//Einlesen der Techtrees
	
	if (race.compare("sc2-hots-protoss") == 0)
	{
		
		for (unsigned int runs = 0;runs < 2;runs++)
		{
			file.open(techtreefilename, ios::in);
			if (file.good())
			{
				while (!file.eof())
				{
					file.getline(line, 1024);
					std::string temp = line;

					if (temp.compare(1, 7, "Protoss") == 0)
					{
						file.getline(line, 1024); //### zeile überspringen
						file.getline(line, 1024); //  erste Unit einlesen
						temp = line;
						while (temp.compare(1, 9, "buildings") != 0)
						{
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0)
							{

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supcosttemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int productmaxtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int minharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vespaharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								bool isbuildertemp = false;
								unsigned int lifedurtemp = 0;
								if (minharvtemp != 0 && vespaharvtemp != 0)
								{
									isbuildertemp = true;
								}



								game_setup->add_unit_blueprint(UnitBlueprint(
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
									isbuildertemp)); // Is builder
							}
							else
							{

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
								unsigned int morphtemp = std::stoi(dummytemp);

								postemp = 0;
								while (postemp<dependtemp.length())
								{
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_unit_blueprint_by_name(nametemp).add_dependency_blueprint(game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0)
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									}
								}
								else
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										try
										{
											game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_unit_blueprint_by_name(nametemp) });
										}
										catch (const std::out_of_range)
										{
											game_setup->find_building_blueprint_by_name(dumtemp).add_morphable_building_blueprint({ game_setup->find_building_blueprint_by_name(nametemp) });
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
						while (temp.compare(0, 1, "#") != 0 && temp.size()>0)
						{
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0)
							{

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int supcosttemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int productmaxtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int minharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int vespaharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								unsigned int lifedurtemp = 0;

								game_setup->add_building_blueprint(BuildingBlueprint(
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
									productmaxtemp)); // Max concurrent unit production count
							}
							else
							{

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
								unsigned int morphtemp = std::stoi(dummytemp);

								postemp = 0;
								while (postemp<dependtemp.length())
								{
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_building_blueprint_by_name(nametemp).add_dependency_blueprint(game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0)
								{
									//while (postemp<prodbytemp.length())
									//	{
									//std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
									//	postemp += dumtemp.length() + 1;
									//game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									//}
								}
								else
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										try
										{
											game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_unit_blueprint_by_name(nametemp) });
										}
										catch (const std::out_of_range)
										{
											game_setup->find_building_blueprint_by_name(dumtemp).add_morphable_building_blueprint({ game_setup->find_building_blueprint_by_name(nametemp) });
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
		for (unsigned int i = 0;i<worker;i++)
		{
			game_setup->add_unit_by_name("probe");
		}
		for (unsigned int a = 0;a < basebuilding;a++)
		{
			game_setup->add_building_by_name("nexus");
		}
		
	}
	else
	{
		LOG_ERROR("No such Race");

	}
	return game_setup;
}

std::unique_ptr<ZergGame> Zerginitialize(const std::string& race) {
	LOG_INFO("Default log temp.");
	LOG_DEBUG("Debug log temp.");
	LOG_ERROR("Error log temp.");

	char     configfilename[128] = "../config.csv";
	char     techtreefilename[128] = "../techtrees.csv";
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
	if (file.good())
	{
		while (!file.eof())
		{
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			if (temp.compare(2, 7, "General") == 0)
			{
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // Mineralien Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				mineral_count = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); // Vespene Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				vespene_gas_count = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); //Worker Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				worker = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); //BaseBuilding Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				basebuilding = std::stoi(temp);
				temp.clear();


			}			
			else if (temp.compare(2, 4, "Zerg") == 0 && race.compare("sc2-hots-zerg") == 0)
			{
				// Hier kommt alles rein was in der Config die Zerg betrifft
				file.getline(line, 1024); // ### Zeile einlesen
				file.getline(line, 1024); // overlord Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				overlord = std::stoi(temp);
				temp.clear();
				file.getline(line, 1024); // larvae Zeile einlesen
				temp = line;
				temp = temp.substr(temp.find(",") + 1);
				larvae = std::stoi(temp);
				temp.clear();
				
			}
		}
		file.close();
	}
	else
	{
		LOG_ERROR("Config-File not found");

	}
	std::unique_ptr<ZergGame> game_setup(new ZergGame(mineral_count, vespene_gas_count)); //Zerg game erstellen
	//Einlesen der Techtrees	
	if (race.compare("sc2-hots-zerg") == 0)
	{
		
		for (unsigned int runs = 0;runs < 2;runs++)
		{
			file.open(techtreefilename, ios::in);
			if (file.good())
			{
				while (!file.eof())
				{
					file.getline(line, 1024);
					std::string temp = line;

					if (temp.compare(1, 4, "Zerg") == 0)
					{
						file.getline(line, 1024); //### zeile überspringen
						file.getline(line, 1024); //  erste Unit einlesen
						temp = line;
						while (temp.compare(1, 9, "buildings") != 0)
						{
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0)
							{

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supcosttemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int productmaxtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int minharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vespaharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								bool isbuildertemp = false;
								unsigned int lifedurtemp = 0;
								if (minharvtemp != 0 && vespaharvtemp != 0)
								{
									isbuildertemp = true;
								}



								game_setup->add_unit_blueprint(UnitBlueprint(
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
									isbuildertemp)); // Is builder
							}
							else
							{

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
								unsigned int morphtemp = std::stoi(dummytemp);

								postemp = 0;
								while (postemp<dependtemp.length())
								{
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_unit_blueprint_by_name(nametemp).add_dependency_blueprint(game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0)
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									}
								}
								else
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										try
										{
											if (morphtemp == 1)
											{

												game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_unit_blueprint_by_name(nametemp) });
											}
											else
											{
												game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_unit_blueprint_by_name(nametemp),  game_setup->find_unit_blueprint_by_name(nametemp) });

											}
										}
										catch (const std::out_of_range)
										{
											game_setup->find_building_blueprint_by_name(dumtemp).add_morphable_building_blueprint({ game_setup->find_building_blueprint_by_name(nametemp) });
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
						while (temp.compare(0, 1, "#") != 0 && temp.size()>0)
						{
							unsigned int postemp = 0;
							std::string  dummytemp;
							if (runs == 0)
							{

								//Name
								std::string  nametemp = temp.substr(postemp, temp.find(","));
								postemp += nametemp.length() + 1;
								//MineralCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int mineraltemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//VespereCost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int vesptemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Buildtime
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int buildtimetemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplycost
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int supcosttemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Supplyprovide
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int supprovtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Startenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int startentemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Maxenergy
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int maxenergtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Race
								std::string  racetemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								postemp += racetemp.length() + 1;
								//productionmax
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								unsigned int productmaxtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Mineralharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
							//	unsigned int minharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;
								//Vesparharvest
								dummytemp = temp.substr(postemp, temp.find(",", postemp) - postemp);
								//unsigned int vespaharvtemp = std::stoi(dummytemp);
								postemp += dummytemp.length() + 1;

								dummytemp.clear();
								unsigned int lifedurtemp = 0;

								game_setup->add_building_blueprint(BuildingBlueprint(
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
									productmaxtemp)); // Max concurrent unit production count
							}
							else
							{

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
								unsigned int morphtemp = std::stoi(dummytemp);

								postemp = 0;
								while (postemp<dependtemp.length())
								{
									std::string  dumtemp = dependtemp.substr(postemp, dependtemp.find("/", postemp + 1) - postemp);
									postemp += dumtemp.length() + 1;
									game_setup->find_building_blueprint_by_name(nametemp).add_dependency_blueprint(game_setup->find_building_blueprint_by_name(dumtemp));
								}
								postemp = 0;
								if (morphtemp == 0)
								{
									//while (postemp<prodbytemp.length())
									//	{
									//std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
									//	postemp += dumtemp.length() + 1;
									//game_setup->find_building_blueprint_by_name(dumtemp).add_producible_unit_blueprint(game_setup->find_unit_blueprint_by_name(nametemp));
									//}
								}
								else
								{
									while (postemp<prodbytemp.length())
									{
										std::string  dumtemp = prodbytemp.substr(postemp, prodbytemp.find("/", postemp + 1) - postemp);
										postemp += dumtemp.length() + 1;
										try
										{
											game_setup->find_unit_blueprint_by_name(dumtemp).add_morphable_blueprints({ game_setup->find_building_blueprint_by_name(nametemp) });
										}
										catch (const std::out_of_range)
										{
											game_setup->find_building_blueprint_by_name(dumtemp).add_morphable_building_blueprint({ game_setup->find_building_blueprint_by_name(nametemp) });
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
		for (unsigned int i = 0;i<worker;i++)
		{
			game_setup->add_unit_by_name("drone");
		}
		for (unsigned int a = 0;a < basebuilding;a++)
		{
			game_setup->add_building_by_name("hatchery");
		}
		for (unsigned int b = 0;b < overlord;b++)
		{
			game_setup->add_unit_by_name("overlord");
		}
		for (unsigned int c = 0;c < larvae;c++)
		{
			game_setup->add_unit_by_name("larva");
		}
		
	}
	else
	{
		LOG_ERROR("No such Race");

	}
	return game_setup;


}
void example() {
	LOG_INFO("Default log test.");
	LOG_DEBUG("Debug log test.");
	LOG_ERROR("Error log test.");

	TerranGame terran_game(50, 0);
	terran_game.add_unit_blueprint(UnitBlueprint(
		Race::Terran, // Race
		"SCV", // Name
		50, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		5, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{}, // Morphable blueprints
		1, // Supply costs,
		0, // Mineral collection rate
		0, // Vespine gas collection rate
		true)); // Is builder
	terran_game.add_building_blueprint(BuildingBlueprint(
		Race::Terran, // Race
		"Command Center", // Name
		400, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		60, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		50, // Start energy
		200, // Max energy
		9, // Supply provided
		{}, // Morphable blueprints
		{ terran_game.find_unit_blueprint_by_name("SCV") /* We add the SCV here already. */ }, // Producible unit blueprints
		1)); // Max concurrent unit production count

	terran_game.add_building_by_name("Command Center");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");
	terran_game.add_unit_by_name("SCV");

	// Can we produce a SCV?
	std::cout << "Expected: true, Is: " << (terran_game.can_produce_units_by_names({ "SCV" }) ? "true" : "false") << std::endl;
	terran_game.produce_units_by_names({ "SCV" });
	// Can we produce another one? (Obviously no because no minerals are left and all buildings are occupied.)
	std::cout << "Expected: false, Is: " << (terran_game.can_produce_units_by_names({ "SCV" }) ? "true" : "false") << std::endl;

	ZergGame zerg_game(50, 0);
	zerg_game.add_unit_blueprint(UnitBlueprint(
		Race::Zerg, // Race
		"Larva", // Name
		0, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		30, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{ /* Drone is not yet existing, therefore we have to add it later. */ }, // Morphable blueprints
		0, // Supply costs,
		0, // Mineral collection rate
		0, // Vespine gas collection rate
		false)); // Is builder
	zerg_game.add_unit_blueprint(UnitBlueprint(
		Race::Zerg, // Race
		"Drone", // Name
		50, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		30, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{}, // Morphable blueprints
		1, // Supply costs,
		700, // Mineral collection rate
		350, // Vespine gas collection rate
		false)); // Is builder
				 // No we can add the drone to larva's morphables.
	zerg_game.find_unit_blueprint_by_name("Larva").add_morphable_blueprints({ zerg_game.find_unit_blueprint_by_name("Drone") });
	zerg_game.add_unit_blueprint(UnitBlueprint(
		Race::Zerg, // Race
		"Zergling", // Name
		25, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		40, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		0, // Start energy
		0, // Max energy
		0, // Supply provided
		{}, // Morphable blueprints
		1, // Supply costs,
		0, // Mineral collection rate
		0, // Vespine gas collection rate
		false)); // Is builder
				 // No we can add the drone to larva's morphables.
	zerg_game.find_unit_blueprint_by_name("Larva").add_morphable_blueprints({
		zerg_game.find_unit_blueprint_by_name("Zergling"),
		zerg_game.find_unit_blueprint_by_name("Zergling")
	});
	zerg_game.add_building_blueprint(BuildingBlueprint(
		Race::Zerg, // Race
		"Hatchery", // Name
		400, // Mineral costs
		0, // Vespene gas costs
		{}, // Dependency blueprints
		60, // Creation duration in seconds
		0, // Life duration in seconds (0 = infinite)
		50, // Start energy
		200, // Max energy
		9, // Supply provided
		{}, // Morphable blueprints
		{}, // Producible unit blueprints
		1)); // Max concurrent unit production count

	zerg_game.add_building_by_name("Hatchery");
	zerg_game.add_unit_by_name("Larva");
	zerg_game.add_unit_by_name("Larva");
	zerg_game.add_unit_by_name("Larva");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");
	zerg_game.add_unit_by_name("Drone");

	// Can we morph two zerglings?
	std::cout << "Expected: true, Is: " << (zerg_game.can_produce_units_by_names({ "Zergling", "Zergling" }) ? "true" : "false") << std::endl;
	zerg_game.produce_units_by_names({ "Zergling", "Zergling" });
	// Can we morph another drone? (Obviously no because no minerals are left.)
	std::cout << "Expected: false, Is: " << (zerg_game.can_produce_units_by_names({ "Drone" }) ? "true" : "false") << std::endl;

	// Example: How to simulate game and get events.
	for (unsigned int i = 0; i < 200; ++i) {
		std::cout << "> " << i << " sec until " << (i + 1) << " sec -" <<
			" Min: " << zerg_game.get_mineral_count() <<
			" Gas: " << zerg_game.get_vespene_gas_count() <<
			std::endl;
		auto events = zerg_game.Game::update(1);
		for (auto j = events.begin(); j != events.end(); ++j) {
			if (UnitProductionStartEvent* unit_production_start_event = dynamic_cast<UnitProductionStartEvent*>(j->get())) {
				std::shared_ptr<Unit> unit = unit_production_start_event->get_unit_production().get_unit();
				std::cout << "EVENT: UnitProductionStartEvent -" <<
					" ID: " << unit->get_id() <<
					" Name: " << unit->get_unit_blueprint().get_name() <<
					// The following nullptr check is important because morphing unit prodcution events don't know of a building!
					" Producer: " << (unit_production_start_event->get_unit_production().get_building() ? unit_production_start_event->get_unit_production().get_building()->get_building_blueprint().get_name() : "n/a") <<
					std::endl;
			}
			if (UnitProductionFinishEvent* unit_production_finish_event = dynamic_cast<UnitProductionFinishEvent*>(j->get())) {
				std::shared_ptr<Unit> unit = unit_production_finish_event->get_unit_production().get_unit();
				std::cout << "EVENT: UnitProductionFinishEvent -" <<
					" ID: " << unit->get_id() <<
					" Name: " << unit->get_unit_blueprint().get_name() <<
					// The following nullptr check is important because morphing unit prodcution events don't know of a building!
					" Producer: " << (unit_production_finish_event->get_unit_production().get_building() ? unit_production_finish_event->get_unit_production().get_building()->get_building_blueprint().get_name() : "n/a") <<
					std::endl;
			}
			if (BuildingConstructionStartEvent* building_construction_start_event = dynamic_cast<BuildingConstructionStartEvent*>(j->get())) {
				std::shared_ptr<Building> building = building_construction_start_event->get_building_construction().get_building();
				std::cout << "EVENT: BuildingConstructionStartEvent -" <<
					" ID: " << building->get_id() <<
					" Name: " << building->get_building_blueprint().get_name() <<
					std::endl;
			}
			if (BuildingConstructionFinishEvent* building_construction_finish_event = dynamic_cast<BuildingConstructionFinishEvent*>(j->get())) {
				std::shared_ptr<Building> building = building_construction_finish_event->get_building_construction().get_building();
				std::cout << "EVENT: BuildingConstructionFinishEvent -" <<
					" ID: " << building->get_id() <<
					" Name: " << building->get_building_blueprint().get_name() <<
					std::endl;
			}
		}
		// Let's build new hatcheries if we have 400 minerals.
		if (zerg_game.get_mineral_count() > 400) {
			zerg_game.construct_buildings_by_names({ "Hatchery" });
		}
	}
}

void forwardSimulator(Game& game_setup, const std::string& buildlistpath)
{
	std::list<std::string> buildlist; //Die eingelesene Liste mit den Aufträgen
	ifstream file;
	char    line[1024];
	file.open(buildlistpath, ios::in);
	if (file.good())
	{
		while (!file.eof())
		{
			// Die Datei zeilenweise auslesen
			file.getline(line, 1024);
			std::string temp = line;
			buildlist.push_back(temp);
		}
		file.close();
	}
	else
	{
		LOG_ERROR("Error couldn't read Buildlist");
		return;
	}
	//TODO: Hier die ForwardSimulator Routine rein
}
int main(int argc, char** argv) {
	// BspParameter : sc2-hots-terran ../terran1.txt
	if (argc == 3)
	{
		const std::string& race = argv[1];

		if (race.compare("sc2-hots-terran") == 0)
		{
			std::unique_ptr<TerranGame> terran_game = Terraninitialize(argv[1]);
			forwardSimulator(*terran_game, argv[2]);
		}
		else if (race.compare("sc2-hots-protoss") == 0)
		{
			std::unique_ptr<ProtossGame> protoss_game = Protossinitialize(argv[1]);
			forwardSimulator(*protoss_game, argv[2]);
		}
		else if (race.compare("sc2-hots-zerg") == 0)
		{
			std::unique_ptr<ZergGame> zerg_game = Zerginitialize(argv[1]);
			forwardSimulator(*zerg_game, argv[2]);
		}
		else
		{
			LOG_ERROR("Wrong Race Name, pls use sc2-hots-terran, sc2-hots-protoss or  sc2-hots-zerg");
		}
	}
	else
	{
		LOG_ERROR("Wrong Number of Parameters");
	}

	// example();
}
