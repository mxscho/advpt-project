#include "game/genetic.h"
#define push 1
#define rush 0


Genetic::Genetic(const string& unit, Game& game, int mode, const string& race)
{
	//Step 0 initialize
	//std::list<std::list<std::string>> population;
	m_mode = mode;
	m_unit = unit;
	m_race = race;
	m_fitnesspushmax = 100;

	std::list<std::string> buildlist;
	int randnmb = 0;
	int rndtyp = 0;
	bool correctunit = false;
	bool listvalid = true;
	std::string temp = "";
	//Step 1 build a initialize population

	for (int a=0; a < 20;a++)
	{
		correctunit = false;
		listvalid = true;
		randnmb = 0;
		rndtyp = 0;
		while (listvalid)
		{
			
			if (rndtyp == 0)
			{
				std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
				std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();
				std::advance(it, randnmb);
				
				temp = (it)->get_name();
			}
			else
			{
				std::list<BuildingBlueprint> i_building = game.get_building_blueprints();
				std::list<BuildingBlueprint>::iterator it = i_building.begin();
				std::advance(it, randnmb);
				temp = (it)->get_name();
			}
						
			buildlist.push_back(temp);
			int timefor = forwardSimulator(m_race, game, buildlist);
			if (timefor>0)
			{
				if (timefor > 360 && m_mode == rush)	//wenn bei rush, abbrechen, wenn die liste länger als 6 min ist erstmal 600, später dann 360
				{
					buildlist.pop_back();
						listvalid = false;
					
					break;
				}
			
			}
			else
			{
				buildlist.pop_back();
			}
			//Randomizer ob Unit oder Building
			//Randomizer welche Unit/Building
			rndtyp = rand() % 2;
			if (rndtyp == 0)
			{
				randnmb = rand() % game.get_unit_blueprints().size();
			}
			else
			{
				randnmb = rand() % game.get_building_blueprints().size();
			}					
			std::list<std::string>::iterator ite = buildlist.end();		
			ite--;
			temp = { *ite };

			if (temp.compare(unit)==0)
			{				
				correctunit = true;
				if(m_mode==push)
					listvalid = false;				
			}
		}
		//Step 2 direkt die Fitness mitberechnen
		if (correctunit == true)
		{
			std::pair<int, std::list<std::string>> listemp;
			if (m_mode == push)
			{
				listemp.first = fitness(buildlist);
			}
			else
			{
				listemp.first = forwardSimulator(m_race, game, buildlist);
			}
			listemp.second = buildlist;
			population.push_back(listemp);
		}
		else
			a--;

	}

	//Ab hier die Genetische Schleife 50 Generationen
	for (int b = 0; b < 50;b++)
	{

		//Step 3 Selection
		// Selection der besten
		selection();

		//Step 4 Reproduction inkl Fitness
		//Hierfür werden jeweils 4 Individuuen zufällig ausgewählt, peer distance verglichen. das paar mit
		//der größten distanz wird gewählt. (jedes Gen wird verglichen, sind sie gleich 0 sind sie unterschiedl
		//ich 1) das gen wird so gewählt, sollta das Gen invalides ergebnis liefer, wird das andere gewählt
		reproduction(game);
		//Step 5 Mutation nimm die besten 5 und führe sie direkt in die neue Generation über
		mutation();
		//Step 6 Newpopulation to Population
		population.clear();
		population = newpopulation;
		
	}
	//Step 7 Denn besten auswählen
	int bestfitness = 0;
	if (m_mode == push)//push
	{
		bestfitness = m_fitnesspushmax;
	}
	else //rush
	{
		bestfitness = 0;
	}
	std::list<std::pair<int, std::list<std::string>>>::iterator bestsolution;
	for (std::list<std::pair<int, std::list<std::string>>>::iterator it = population.begin(); it != population.end(); ++it)
	{
		if (it->first >bestfitness && m_mode==rush)
		{
			bestfitness = it->first;
			bestsolution = it;
		}
		else if (it->first <bestfitness && m_mode == push)
		{
			bestfitness = it->first;
			bestsolution = it;
		}
	}
	//Step 8 Best solution nochmal durchlaufen lassen
	forwardSimulator(m_race, game, bestsolution->second);
}

void Genetic::mutation()
{
	for (int a = 0;a < 5;a++)
	{
		int bestfitness = 0;
		if (m_mode == push)//push
		{
			bestfitness = m_fitnesspushmax;
		}
		else //rush
		{
			bestfitness = 0;
		}
		std::list<std::pair<int, std::list<std::string>>>::iterator bestsolution;
		for (std::list<std::pair<int, std::list<std::string>>>::iterator it = population.begin(); it != population.end(); ++it)
		{
			if (it->first > bestfitness && m_mode == rush)
			{
				bestfitness = it->first;
				bestsolution = it;
			}
			else if (it->first < bestfitness && m_mode == push)
			{
				bestfitness = it->first;
				bestsolution = it;
			}
		}
		std::pair<int, std::list<std::string>> listemp;
		if (m_mode == push)
		{
			listemp.first = bestsolution->first;
			bestsolution->first = m_fitnesspushmax;
		}
		else
		{
			listemp.first = bestsolution->first;
			bestsolution->first = 0;
		}
		listemp.second = bestsolution->second;
		newpopulation.push_back(listemp);
	}

}
void Genetic::reproduction(Game& game)
{
	std::list<std::pair<int, std::list<std::string>>> temp;
	std::list<std::pair<int, std::list<std::string>>>::iterator firstpro;
	std::list<std::pair<int, std::list<std::string>>>::iterator secondpro;
	std::list<std::pair<int, std::list<std::string>>>::iterator thirdpro;
	std::list<std::pair<int, std::list<std::string>>>::iterator fourthpro;
	std::list<std::pair<int, std::list<std::string>>>::iterator winnerone;
	std::list<std::pair<int, std::list<std::string>>>::iterator winnertwo;

	// 15 neue Individueen und die 5 besten alten wieder übernehmen
	for (int a = 0; a < 15;a++)
	{
		// 4 zufällige Individueen //gewichtet nach den besten 	Fitness werten
		for (int b = 0; b < 4;b++)
		{
			for (std::list<std::pair<int, std::list<std::string>>>::iterator it = population.begin(); it != population.end(); ++it)
			{
				if (m_mode == rush)//rush
				{
					if (it->first > rand() % (m_bestfitnes))
					{
						if (b == 0)
							firstpro = it;
						else if (b == 1)
							secondpro = it;
						else if (b == 2)
							thirdpro = it;
						else if (b == 3)
							fourthpro = it;

						it = population.end();
					}
				}
				else
				{
					if ((m_fitnesspushmax- it->first )> rand() % (m_fitnesspushmax-m_bestfitnes))
					{
						if (b == 0)
							firstpro = it;
						else if (b == 1)
							secondpro = it;
						else if (b == 2)
							thirdpro = it;
						else if (b == 3)
							fourthpro = it;

						it = population.end();
					}
				}
				
			}

		}	
			//Distanz berrechnen und die zwei mit der größen distanz auswählen
			
		std::list<std::string>::iterator itfirstpro = firstpro->second.begin();
		std::list<std::string>::iterator itsecondpro = secondpro->second.begin();
		std::list<std::string>::iterator itthirdpro = thirdpro->second.begin();
		std::list<std::string>::iterator itfourthpro = fourthpro->second.begin();
		
		
		int firstsize = firstpro->second.size();
		int secondsize = secondpro->second.size();
		int thirdsize = thirdpro->second.size();
		int fourthsize = fourthpro->second.size();



		
			//Sortieren nach Größe
			for (int aswap = 0;aswap < 3;aswap++)
			{
				firstsize = firstpro->second.size();
				secondsize = secondpro->second.size();
				thirdsize = thirdpro->second.size();
				fourthsize = fourthpro->second.size();
				if (firstsize < secondsize)
				{
					std::list<std::pair<int, std::list<std::string>>>::iterator temp;
					temp = firstpro;
					firstpro = secondpro;
					secondpro = temp;
				}
				else if (firstsize < thirdsize)
				{
					std::list<std::pair<int, std::list<std::string>>>::iterator temp;
					temp = firstpro;
					firstpro = thirdpro;
					thirdpro = temp;
				}
				else if (firstsize < fourthsize)
				{
					std::list<std::pair<int, std::list<std::string>>>::iterator temp;
					temp = firstpro;
					firstpro = fourthpro;
					fourthpro = temp;
				}
			}
			for (int aswap = 0;aswap < 2;aswap++)
			{

				secondsize = secondpro->second.size();
				thirdsize = thirdpro->second.size();
				fourthsize = fourthpro->second.size();

				if (secondsize < thirdsize)
				{
					std::list<std::pair<int, std::list<std::string>>>::iterator temp;
					temp = secondpro;
					secondpro = thirdpro;
					thirdpro = temp;
				}
				else if (secondsize < fourthsize)
				{
					std::list<std::pair<int, std::list<std::string>>>::iterator temp;
					temp = secondpro;
					secondpro = fourthpro;
					fourthpro = temp;
				}
			}

			thirdsize = thirdpro->second.size();
			fourthsize = fourthpro->second.size();
			if (thirdsize < fourthsize)
			{
				std::list<std::pair<int, std::list<std::string>>>::iterator temp;
				temp = thirdpro;
				thirdpro = fourthpro;
				fourthpro = temp;
			}


		







		int distanzbest = 0;
		int distanztemp = 0;
		int counter = 0;
		for (int d = 0;d < 3;d++)
		{ 
			counter = 0;
			distanztemp = 0;
			itfirstpro = firstpro->second.begin();
			for (itfirstpro; itfirstpro != firstpro->second.end(); ++itfirstpro)
			{
				if(d==0)
				{ 
					if (itfirstpro->compare(itsecondpro->c_str()) == 0)
					{
						//do Nothing
					}
					else
					{
						distanztemp += (firstsize- counter);
						//Distanz hochzählen
						if (distanztemp > distanzbest)
						{
							distanzbest = distanztemp;
							winnerone = firstpro;
							winnertwo = secondpro;
						}
					}
					counter++;
					itsecondpro++;
					if (secondpro->second.end() == itsecondpro)
					{
						std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (it)->get_name();
						secondpro->second.push_back(tempstr);
					}
				}
				else if (d == 1)
				{
					if (itfirstpro->compare(itthirdpro->c_str()) == 0)
					{
						//do Nothing
					}
					else
					{
						distanztemp += (firstsize - counter);
						//Distanz hochzählen
						if (distanztemp > distanzbest)
						{
							distanzbest = distanztemp;
							winnerone = firstpro;
							winnertwo = thirdpro;
						}
					}
					counter++;
					
					itthirdpro++;
					if (thirdpro->second.end() == itthirdpro)
					{
						std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (it)->get_name();
						thirdpro->second.push_back(tempstr);
					}
				}
				else if (d == 2)
				{
					if (itfirstpro->compare(itfourthpro->c_str()) == 0)
					{
						//do Nothing
					}
					else
					{
						distanztemp += (firstsize - counter);
						//Distanz hochzählen
						if (distanztemp > distanzbest)
						{
							distanzbest = distanztemp;
							winnerone = firstpro;
							winnertwo = fourthpro;
						}
					}
					counter++;
					itfourthpro++;
					if (fourthpro->second.end() == itfourthpro)
					{
						std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (it)->get_name();
						fourthpro->second.push_back(tempstr);
					}
				
				}
			}
	
		}
		for (int e = 0;e < 2;e++)
		{
			counter = 0;
			distanztemp = 0;
			itsecondpro = secondpro->second.begin();
			for (itsecondpro; itsecondpro != secondpro->second.end(); ++itsecondpro)
			{
				if (e == 0)
				{
					if (itsecondpro->compare(itthirdpro->c_str()) == 0)
					{
						//do Nothing
					}
					else
					{
						distanztemp += (secondsize - counter);
						//Distanz hochzählen
						if (distanztemp > distanzbest)
						{
							distanzbest = distanztemp;
							winnerone = secondpro;
							winnertwo = thirdpro;
						}
					}
					counter++;
					
					itthirdpro++;
					if (thirdpro->second.end() == itthirdpro)
					{
						std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (it)->get_name();
						thirdpro->second.push_back(tempstr);
					}
				}
				else if (e == 1)
				{
					if (itsecondpro->compare(itfourthpro->c_str()) == 0)
					{
						//do Nothing
					}
					else
					{
						distanztemp += (secondsize - counter);
						//Distanz hochzählen
						if (distanztemp > distanzbest)
						{
							distanzbest = distanztemp;
							winnerone = secondpro;
							winnertwo = fourthpro;
						}
					}
					counter++;
					
					itfourthpro++;
					if (fourthpro->second.end() == itfourthpro)
					{
						std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (it)->get_name();
						fourthpro->second.push_back(tempstr);
					}
				}
			}
		}
		counter = 0;
		distanztemp = 0;
		itthirdpro = thirdpro->second.begin();
		for (itthirdpro; itthirdpro != thirdpro->second.end(); ++itthirdpro)
		{
			
				if (itthirdpro->compare(itfourthpro->c_str()) == 0)
				{
					//do Nothing
				}
				else
				{
					distanztemp += (thirdsize - counter);
					//Distanz hochzählen
					if (distanztemp > distanzbest)
					{
						distanzbest = distanztemp;
						winnerone = thirdpro;
						winnertwo = fourthpro;
					}
				}
				counter++;
				
				itfourthpro++;
				if (fourthpro->second.end() == itfourthpro)
				{
					std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
					std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

					string tempstr = (it)->get_name();
					fourthpro->second.push_back(tempstr);
				}
			}

		
		//Bei jedem Gen zufällig wählen welches, aber jedesmal prüfen ob es geht.
		//Copy von game
		std::list<std::string> tempbuildlist;
		std::list<std::string>::iterator itwinnerone = winnerone->second.begin();
		std::list<std::string>::iterator itwinnertwo = winnertwo->second.begin();
		if (winnerone->second.size() < winnertwo->second.size())
		{
			std::list<std::pair<int, std::list<std::string>>>::iterator temp;

			temp = winnerone;
			winnerone = winnertwo;
			winnertwo = temp;

			itwinnerone = winnerone->second.begin();
			itwinnertwo = winnertwo->second.begin();


		}
		int genrand = rand() % 2;
		bool genable = true;
		
		while(itwinnerone!= winnerone->second.end())
		{ 
			if (winnertwo->second.end() == itwinnertwo)
			{
				std::list<UnitBlueprint> i_UnitBlueprint = game.get_unit_blueprints();
				std::list<UnitBlueprint>::iterator it = i_UnitBlueprint.begin();

				string tempstr = (it)->get_name();
				winnertwo->second.push_back(tempstr);
			}
			if (genrand == 0)
			{
				tempbuildlist.push_back(*itwinnerone);
			}
			else
			{
				tempbuildlist.push_back(*itwinnertwo);
			}

			if (forwardSimulator(m_race, game, tempbuildlist) > 0)
			{
				genrand = rand() % 2;
				itwinnerone++;
				itwinnertwo++;
			}
			else
			{

				tempbuildlist.pop_back();
				if (genrand == 0)
				{				
					genrand = 1;
					genable = false;
				}
				else
				{
					genrand = 0;
					genable = false;
				}
				if (!genable)
				{
					a--;
					break;
					//abbruch der schleife, da keine der beiden Gene machbar ist
				}

			}
		}
		std::pair<int, std::list<std::string>> listemp;
		if (m_mode == rush)
		{
			listemp.first = fitness(tempbuildlist);
		}
		else
		{
			listemp.first = forwardSimulator(m_race, game, tempbuildlist);
		}
		listemp.second = tempbuildlist;
		newpopulation.push_back(listemp);

	}

}

void Genetic::selection()
{
	

	std::list<std::pair<int, std::list<std::string>>>::iterator tempit;
	std::list<std::pair<int, std::list<std::string>>>::iterator tempfirst;
	std::list<std::pair<int, std::list<std::string>>>::iterator tempprod;
	int worker = 0;
	int speed = 100;
	int fitness = 0;
	if (m_mode == rush) //rush
		fitness = 100;
	for (std::list<std::pair<int, std::list<std::string>>>::iterator it = population.begin(); it != population.end(); ++it)
	{
		if (m_race.compare("zerg") == 0)
		{
			int dummyworker = 0;
			for (std::list<std::string>::iterator ite = it->second.begin(); ite != it->second.end(); ++ite)
			{
				if (ite->compare("drone") == 0)
				{
					dummyworker++;
				}
			}
			if (dummyworker > worker)
			{
				worker = dummyworker;
				tempit = it;
			}
		}
		else if (m_race.compare("terran") == 0)
		{
			int dummyworker = 0;
			for (std::list<std::string>::iterator ite = it->second.begin(); ite != it->second.end(); ++ite)
			{
				if (ite->compare("scv") == 0)
				{
					dummyworker++;
				}
			}
			if (dummyworker > worker)
			{
				worker = dummyworker;
				tempit = it;
			}
			
		}
		else if (m_race.compare("protoss") == 0)
		{
			int dummyworker = 0;
			for (std::list<std::string>::iterator ite = it->second.begin(); ite != it->second.end(); ++ite)
			{
				if (ite->compare("probe") == 0)
				{
					dummyworker++;
				}
			}
			if (dummyworker > worker)
			{
				worker = dummyworker;
				tempit = it;
			}
		}
		int tempspeed = 0;
		for (std::list<std::string>::iterator ite = it->second.begin(); ite != it->second.end(); ++ite)
		{
			tempspeed++;
			if (ite->compare(m_unit) == 0)
			{
				if (speed > tempspeed)
				{
					speed = tempspeed;
					tempfirst = it;
				}
				else
				{
					ite = it->second.end();
				}
			}
		}

		//Option:Unit with most Construction structures 


		if (m_mode == push) //push
		{
			//find best
			if (it->first > fitness)
			{
				fitness = it->first;
			}
		}
		else //rush
		{
			if (it->first < fitness)
			{
				fitness = it->first;
			}
		}	
	}
	m_bestfitnes = fitness;
	if (m_mode == push) //push
	{
		tempit->first = m_bestfitnes+1;
		tempfirst->first = m_bestfitnes + 1;
		//tempprod->first = m_bestfitnes + 1;
	}
	else //rush
	{
		tempit->first = m_bestfitnes-1;
		tempfirst->first = m_bestfitnes - 1;
		//tempprod->first = m_bestfitnes - 1;
	}
}


int Genetic::fitness(std::list<std::string> buildlist)
{
	int fitnes = 0;
	if (m_mode == 1)//push
	{
		for (std::list<std::string>::iterator it = buildlist.begin(); it != buildlist.end(); ++it)
		{		
			fitnes++;		
			//Besser, Zeit aus dem Forward Simulator
		}
	}
	else//rush
	{
		for (std::list<std::string>::iterator it = buildlist.begin(); it != buildlist.end(); ++it)
		{
			if (it->compare(m_unit) == 0)
			{
				fitnes++;
			}
		}
	}
	return fitnes;
}



//TODO:: auf Integer umändern, so dass man die Zeit zurück bekommt, die Simuliert wurde.WEnn Zeit überschritten
//oder nicht valid, eine 0 zurückgeben
int  Genetic::forwardSimulator(const string& race, Game& game, std::list<std::string> buildlist)
{

	buildlist.remove_if([](const string& buildlist_entry) { return buildlist_entry == ""; });
	OutputFormatter output_formatter(race);
	unsigned int time = 0;
	game.set_worker_unit_allocation_function([](const Game& game, const std::list<std::shared_ptr<Unit>>& worker_units) {
		// TODO: Improve.
		std::list<std::shared_ptr<Unit>> collecting_worker_units;
		std::copy_if(worker_units.begin(), worker_units.end(), std::back_inserter(collecting_worker_units), [](const std::shared_ptr<Unit>& worker_unit) {
			return worker_unit->get_unit_blueprint().get_mineral_collection_rate() != 0 && worker_unit->get_unit_blueprint().get_vespene_gas_collection_rate() != 0;
		});
		std::list<std::shared_ptr<Unit>> actual_collecting_worker_units;
		for (auto i_unit = game.get_units().begin(); i_unit != game.get_units().end(); ++i_unit) {
			if ((*i_unit)->get_unit_blueprint().get_name() == "refinery" ||
				(*i_unit)->get_unit_blueprint().get_name() == "assimilator" ||
				(*i_unit)->get_unit_blueprint().get_name() == "extractor") {
				if (collecting_worker_units.size() > 1) {
					std::shared_ptr<Unit> unit = collecting_worker_units.front();
					collecting_worker_units.pop_front();
					actual_collecting_worker_units.push_back(unit);
				}
			}
		}
		return WorkerUnitAllocation(collecting_worker_units, actual_collecting_worker_units);
	});
	game.update(0);
	for (auto i = buildlist.begin(); i != buildlist.end(); ++i) {
		std::list<std::string> names = { *i };
		bool building = game.can_construct_buildings_by_names(names);
		bool unit = game.can_produce_units_by_names(names);
		while (time <= 1000 && !building && !unit) {
			std::list<std::unique_ptr<Event>> events = game.update(1);
			++time;
			if (events.size() > 0) {
				output_formatter.add_event(time, game, events);
			}
			building = game.can_construct_buildings_by_names(names);
			unit = game.can_produce_units_by_names(names);
		}
		if (time > 1000) {
			
			break;
		}
		else {
			if (building) {
				game.construct_buildings_by_names(names);
			}
			else if (unit) {
				game.produce_units_by_names(names);
			}
		}
	}
	if (time <= 1000) {
		
		output_formatter.set_validity(true);
		// TODO: This needs improvement.
		// Just simulate another 200 seconds to get all finished events.
		for (unsigned int i = 0; i < 200; ++i) {
			time++;
			std::list<std::unique_ptr<Event>> events = game.update(1);
			if (events.size() > 0) {
				output_formatter.add_event(time, game, events);
			}
			else
			{
				output_formatter.print();
				return time;
			}
		}
	}
	else {
		return 0; // Angenommen er kann nicht gebaut werden
		output_formatter.set_validity(false);
	}
	output_formatter.print();
	return time;
}



