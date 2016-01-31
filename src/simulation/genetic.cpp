#include "simulation/genetic.h"

#include "simulation/forward_simulator.h"

#define push 1
#define rush 0
#define individuals 20

Genetic::Genetic(const string& unit, Game& game, int mode, const string& race)
{
	// Set seed for randoms.
	srand(0);

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
	bool depend = false;
	bool marphab = false;
	std::string temp = "";
	int tries = 0;
	//Step 1 build a initialize population

	for (int a=0; a < individuals;a++)
	{
		correctunit = false;
		listvalid = true;
		randnmb = 0;
		rndtyp = 0;
		
		buildlist.clear();
		while (listvalid)
		{
			depend = false;
			marphab = false;
			while(!depend)
			{
				if (rndtyp == 0)
				{
					//Alle Units einlesen
					std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();				
					std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();
					//Zufällige Unit wählen
					std::advance(it, randnmb);
					//Liste aller Gebäude
					std::list<std::shared_ptr<BuildingBlueprint>> i_building = game.get_building_blueprints();
					//Name der Unit
					temp = (*it)->get_name();
					//Alle Gebäude durchlaufen
					const std::list<std::reference_wrapper<const Blueprint>> tdepe = (*it)->get_dependency_blueprints();
					if (tdepe.empty()&& race.compare("zerg")==0)
						depend = true;

					for (std::list<std::reference_wrapper<const Blueprint>>::const_iterator ite = tdepe.begin(); ite != tdepe.end(); ++ite)
					{
						if (ite->get().get_name().compare(temp) == 0)
						{
							depend = true;
							break;
						}
						for (std::list<std::string>::iterator its = buildlist.begin(); its != buildlist.end(); ++its)
						{
							if (its->compare(ite->get().get_name()) == 0)
							{
								depend = true;
								break;
							}

						}

					}




					for (std::list<std::shared_ptr<BuildingBlueprint>>::iterator it = i_building.begin(); it != i_building.end(); ++it)
					{
						//WAs kann das gebäude alles Produzieren
						const std::list<std::reference_wrapper<const UnitBlueprint>> tep=(*it)->get_producible_unit_blueprints();	
						//Liste davon durchlaufen
						for (std::list<std::reference_wrapper<const UnitBlueprint>>::const_iterator ite = tep.begin(); ite != tep.end(); ++ite)
							{
								//Ist meine Unit mit auf dem Plan
								if (ite->get().get_name().compare(temp) == 0)
								{
									//Ist es das Command center bzw das erste GEbäude
									if (it == i_building.begin())
									{
										//Wenn ja fertig
										depend = true;
										ite = tep.end();
										it = i_building.end();
										it--;
										break;
									}
									else
									{
										//Buildliste durchlaufen ob wir das Gebäude schon haben
										for (std::list<std::string>::iterator its = buildlist.begin(); its != buildlist.end(); ++its)
										{
											if (its->compare((*it)->get_name()) == 0)
											{
												depend = true;
												its = buildlist.end();
												ite = tep.end();
												ite--;
												it = i_building.end();
												it--;
												break;
											}
										}
									}
								}
								
							}
					}			
				}
				else
				{

					std::list<std::shared_ptr<BuildingBlueprint>> i_building = game.get_building_blueprints();
					std::list<std::shared_ptr<BuildingBlueprint>>::iterator it = i_building.begin();
	
					std::advance(it, randnmb);
					
					const std::list<std::reference_wrapper<const Blueprint>> tep = (*it)->get_dependency_blueprints();
					temp = (*it)->get_name();
				
						for (std::list<std::shared_ptr<BuildingBlueprint>>::iterator ito = i_building.begin(); ito != i_building.end(); ++ito)
						{						
							const std::list<std::reference_wrapper<const BuildingBlueprint>> tmorp = (*ito)->get_morphable_building_blueprints();
							
							for (std::list<std::reference_wrapper<const BuildingBlueprint>>::const_iterator ite = tmorp.begin(); ite != tmorp.end(); ++ite)
							{
								if (ite->get().get_name().compare(temp) == 0)
								{

									marphab = true;
									if ((*ito)->get_name().compare((*i_building.begin())->get_name())== 0)
									{
										depend = true;						
										ito = i_building.end();
										ito--;
										break;
									}


									for (std::list<std::string>::iterator it = buildlist.begin(); it != buildlist.end(); ++it)
									{
										if (it->compare((*ito)->get_name())==0)
										{
											depend = true;
											ite = tmorp.end();
											ite--;
											ito = i_building.end();
											ito--;
											break;
									
										}

									}
								}

							}

							
						}
						if (!marphab)
						{
							if (tep.empty())
								depend = true;
							for (std::list<std::reference_wrapper<const Blueprint>>::const_iterator ite = tep.begin(); ite != tep.end(); ++ite)
							{
								if (ite->get().get_name().compare((*i_building.begin())->get_name()) == 0)
								{
									depend = true;
									break;
								}
								for (std::list<std::string>::iterator it = buildlist.begin(); it != buildlist.end(); ++it)
								{
									if (it->compare(ite->get().get_name()) == 0)
									{
										depend = true;
										break;
									}

								}

							}
						}
										
					}


				
			
				if (!depend)
				{
					marphab = false;
					rndtyp = rand() % 2;
					if (rndtyp == 0)
					{
						randnmb = rand() % game.get_unit_blueprints().size();
					}
					else
					{
						randnmb = rand() % (game.get_building_blueprints().size()-1)+1;
					}
				}
			
			}
			buildlist.push_back(temp);


			if (m_mode == rush)
			{
				if (temp.compare(unit) == 0 || correctunit )
				{
					int timefor = forwardSimulator(m_race, game, buildlist, 400);
					
					if (timefor>0)
					{
						correctunit = true;
						tries = 0;
						if (timefor > 360 && m_mode == rush)	//wenn bei rush, abbrechen, wenn die liste länger als 6 min ist erstmal 600, später dann 360
						{
							buildlist.pop_back();
							listvalid = false;

							break;
						}					
					}
					else
					{
						if (correctunit)
						{
							buildlist.pop_back();
							tries++;
							if (tries == 100)
							{
								listvalid = false;
								tries = 0;
								break;
							}

						}
						else
						{
							listvalid = true;
							break;
						}
					}


				}
				
			}
			else
			{
				if (temp.compare(unit) == 0)
				{					
					if (forwardSimulator(m_race, game, buildlist,10000)>0)
					{				
						correctunit = true;
						listvalid = false;
						break;						
					}
					else
					{
						listvalid = false;
						break;
					}
				}
				
			}
			rndtyp = rand() % 2;
			if (rndtyp == 0)
			{
				randnmb = rand() % game.get_unit_blueprints().size();
			}
			else
			{
				randnmb = rand() % (game.get_building_blueprints().size() - 1) + 1;
			}

			if (buildlist.size() > 30 && m_mode == rush)
				listvalid = false;
			else if(buildlist.size() > 100)
				listvalid = false;
		}
		//Step 2 direkt die Fitness mitberechnen
		



		if (correctunit == true)
		{
			std::pair<int, std::list<std::string>> listemp;
			if (m_mode == push)
			{
				listemp.first = forwardSimulator(m_race, game, buildlist,1000);
			}
			else
			{
				
				listemp.first = fitness(buildlist);
			}
			listemp.second = buildlist;
			population.push_back(listemp);
		}
		else
		{

			if (m_mode == rush)
			{
			//	std::pair<int, std::list<std::string>> listemp;
			//	listemp.first = fitness(buildlist);
			//	listemp.second = buildlist;
			//	population.push_back(listemp);
				a--;
			}
			else
			{
				a--;
			}
		}

	}

	//Ab hier die Genetische Schleife 50 Generationen
	for (int b = 0; b < 5;b++)
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
		newpopulation.clear();
		
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
	int result=	forwardSimulator(m_race, game, bestsolution->second,1000);
	if (result > 360 && m_mode == rush)
	{
		bestsolution->second.pop_back();
		bestsolution->second.pop_back();
	}
	//hier ausgabe hinzufügen
	if (m_mode == rush)
	{
		ForwardSimulator forward_simulator(game);
		forward_simulator.simulate(bestsolution->second, 360);
		forward_simulator.get_output_formatter().print();
	}
	else
	{
		ForwardSimulator forward_simulator(game);
		forward_simulator.simulate(bestsolution->second, 1000);
		forward_simulator.get_output_formatter().print();
	}

}

void Genetic::mutation()
{
	for (int a = 0;a < 4;a++)
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
	for (int a = 0; a < individuals-4;a++)
	{
		// 4 zufällige Individueen //gewichtet nach den besten 	Fitness werten
		for (int b = 0; b < 4;b++)
		{
			for (std::list<std::pair<int, std::list<std::string>>>::iterator it = population.begin(); it != population.end(); ++it)
			{
				if (m_mode == rush)//rush
				{
					if (it->first > rand() % (m_bestfitnes+1))
					{
						if (b == 0)
						{
							firstpro = it;
							break;
						}
						else if (b == 1 && firstpro != it)
						{
							secondpro = it;
							break;
						}
						else if (b == 2 && firstpro != it && secondpro != it)
						{
							thirdpro = it;
							break;
						}
						else if (b == 3 && firstpro != it && secondpro != it && thirdpro != it)
						{
							fourthpro = it;
							break;
						}
						

					}
				}
				else
				{
					if ((m_fitnesspushmax- it->first )> rand() % (m_fitnesspushmax-m_bestfitnes))
					{
						if (b == 0)
						{
							firstpro = it;
							break;
						}
						else if (b == 1 && firstpro != it)
						{
							secondpro = it;
							break;
						}
						else if (b == 2 && firstpro != it && secondpro != it)
						{
							thirdpro = it;
							break;
						}
						else if (b == 3 && firstpro != it && secondpro != it && thirdpro != it)
						{
							fourthpro = it;
							break;
						}
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

			 itfirstpro = firstpro->second.begin();
			 itsecondpro = secondpro->second.begin();
			itthirdpro = thirdpro->second.begin();
			itfourthpro = fourthpro->second.begin();
		
		
		winnerone = firstpro;
		winnertwo = secondpro;

		int distanzbest = 0;
		int distanztemp = 0;
		int counter = 0;
		for (int d = 0;d < 3;d++)
		{ 
			counter = 0;
			distanztemp = 0;
			for (itfirstpro = firstpro->second.begin(); itfirstpro != firstpro->second.end(); ++itfirstpro)
			{
				if(d==0)
				{ 


					if (secondpro->second.end() == itsecondpro)
					{
						std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (*it)->get_name();
						secondpro->second.push_back(tempstr);
						itsecondpro--;
					}


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
					
				}
				else if (d == 1)
				{



					if (thirdpro->second.end() == itthirdpro)
					{
						std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (*it)->get_name();
						thirdpro->second.push_back(tempstr);
						itthirdpro--;
					}


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
					
				}
				else if (d == 2)
				{

					if (fourthpro->second.end() == itfourthpro)
					{
						std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (*it)->get_name();
						fourthpro->second.push_back(tempstr);
						itfourthpro--;
					}

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
					
				
				}
			}
	
		}
		itfirstpro = firstpro->second.begin();
		itsecondpro = secondpro->second.begin();
		itthirdpro = thirdpro->second.begin();
		itfourthpro = fourthpro->second.begin();
		for (int e = 0;e < 2;e++)
		{
			counter = 0;
			distanztemp = 0;
			for (itsecondpro = secondpro->second.begin(); itsecondpro != secondpro->second.end(); ++itsecondpro)
			{
				if (e == 0)
				{
					if (thirdpro->second.end() == itthirdpro)
					{
						std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (*it)->get_name();
						thirdpro->second.push_back(tempstr);
						itthirdpro--;
					}
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
					
				}
				else if (e == 1)
				{
					if (fourthpro->second.end() == itfourthpro)
					{
						std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
						std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

						string tempstr = (*it)->get_name();
						fourthpro->second.push_back(tempstr);
						itfourthpro--;
					}
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
					
				}
			}
		}
		
		
		itfirstpro = firstpro->second.begin();
		itsecondpro = secondpro->second.begin();
		itthirdpro = thirdpro->second.begin();
		itfourthpro = fourthpro->second.begin();
		counter = 0;
		distanztemp = 0;
		for (itthirdpro = thirdpro->second.begin(); itthirdpro != thirdpro->second.end(); ++itthirdpro)
		{
			if (fourthpro->second.end() == itfourthpro)
			{
				std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
				std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

				string tempstr = (*it)->get_name();
				fourthpro->second.push_back(tempstr);
				itfourthpro--;
			}
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
				
			}

		
		//Bei jedem Gen zufällig wählen welches, aber jedesmal prüfen ob es geht.
	
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
				std::list<std::shared_ptr<UnitBlueprint>> i_UnitBlueprint = game.get_unit_blueprints();
				std::list<std::shared_ptr<UnitBlueprint>>::iterator it = i_UnitBlueprint.begin();

				string tempstr = (*it)->get_name();
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
			int ti = 0 ;
			if (m_mode == rush)
			{
				ti = forwardSimulator(m_race, game, tempbuildlist, 400);
			}
			else
			{
				ti = forwardSimulator(m_race, game, tempbuildlist, 1000);
			}
				if ( ti> 0)
				{
					genrand = rand() % 2;
					itwinnerone++;
					itwinnertwo++;
					genable = true;
				}
			
			else
			{

				tempbuildlist.pop_back();


				if (!genable)
				{
					
					break;
					//abbruch der schleife, da keine der beiden Gene machbar ist
				}


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
				

			}
		}
		std::pair<int, std::list<std::string>> listemp;
		if (m_mode == rush)
		{
			listemp.first = fitness(tempbuildlist);
		}
		else
		{
			listemp.first = forwardSimulator(m_race, game, tempbuildlist,1000);
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
	if (m_mode == push) //rush
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
			tempfirst = it;
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
					ite--;
				}
			}
		}

		//Option:Unit with most Construction structures 


		if (m_mode == push) //push
		{
			//find best
			if (it->first < fitness)
			{
				fitness = it->first;
			}
		}
		else //rush
		{
			if (it->first > fitness)
			{
				fitness = it->first;
			}
		}	
	}
	m_bestfitnes = fitness;
	if (m_mode == push) //push
	{
		tempit->first = m_bestfitnes+1;
		tempfirst->first = m_bestfitnes+1;
		//tempprod->first = m_bestfitnes + 1;
	}
	else //rush
	{
		tempit->first = m_bestfitnes-1;
		tempfirst->first = m_bestfitnes-1 ;
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


int Genetic::forwardSimulator(const string& race, Game& game, std::list<std::string> buildlist, int time) {
	ForwardSimulator forward_simulator(game);
	forward_simulator.simulate(buildlist, time);
	return forward_simulator.is_successful() ? forward_simulator.get_time() : 0;
}
/*
//TODO:: auf Integer umändern, so dass man die Zeit zurück bekommt, die Simuliert wurde.WEnn Zeit überschritten
//oder nicht valid, eine 0 zurückgeben
int  Genetic::forwardSimulator(const string& race, Game& game, std::list<std::string> buildlist)
{

	buildlist.remove_if([](const string& buildlist_entry) { return buildlist_entry == ""; });
	//OutputFormatter output_formatter(race);
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
				//output_formatter.add_event(time, game, events);
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
		
		//output_formatter.set_validity(true);
		// TODO: This needs improvement.
		// Just simulate another 200 seconds to get all finished events.
		for (unsigned int i = 0; i < 200; ++i) {
			time++;
			std::list<std::unique_ptr<Event>> events = game.update(1);
			if (events.size() > 0) {
				//output_formatter.add_event(time, game, events);
			}
			else
			{
				//output_formatter.print();
				return time;
			}
		}
	}
	else {
		return 0; // Angenommen er kann nicht gebaut werden
		//output_formatter.set_validity(false);
	}
	//output_formatter.print();
	return time;
}

*/

