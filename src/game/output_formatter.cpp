#include "game/output_formatter.h"

#include "game/building_construction.h"
#include "game/events/building_construction_start_event.h"
#include "game/events/building_construction_finish_event.h"
#include "game/events/unit_production_start_event.h"
#include "game/events/unit_production_finish_event.h"

#include "boost/property_tree/json_parser.hpp"
#include <iostream>

OutputFormatter::OutputFormatter(std::string game) {
	output.put("game", game);
	output.put("buildListValid", "1");
}

void OutputFormatter::add_event(unsigned int time, Game& game, const std::list<std::unique_ptr<Event>>& events) {
	ptree message;
	message.put("time", time);

	ptree status;
	ptree resources;
	resources.put("minerals", game.get_mineral_count());
	resources.put("vespene", game.get_vespene_gas_count());
	resources.put("supply", game.get_supply_available());
	resources.put("supply-used", game.get_supply_used());

	ptree workers;
	workers.put("minerals", game.get_worker_unit_allocation().get_mineral_collecting_worker_units().size());
	workers.put("vespene", game.get_worker_unit_allocation().get_vespene_gas_collecting_worker_units().size());

	status.add_child("resources", resources);
	status.add_child("workers", workers);
	message.add_child("status", status);

	ptree event_array;
	for (auto event = events.begin(); event != events.end(); ++event) {
		ptree entry;

		// if (BuildingConstructionStartEvent* building_construction_start_event = dynamic_cast<BuildingConstructionStartEvent*>(j->get())) {
		if (BuildingConstructionStartEvent* e = dynamic_cast<BuildingConstructionStartEvent*>(event->get())) {
			std::shared_ptr<Building> building = e->get_building_construction().get_building();

			entry.put("type", "build-start");
			entry.put("name", building->get_building_blueprint().get_name());
		}

		if (BuildingConstructionFinishEvent* e = dynamic_cast<BuildingConstructionFinishEvent*>(event->get())) {
			std::shared_ptr<Building> building = e->get_building_construction().get_building();

			entry.put("type", "build-end");
			entry.put("name", building->get_building_blueprint().get_name());
		}

		if (UnitProductionStartEvent* e = dynamic_cast<UnitProductionStartEvent*>(event->get())) {
			std::shared_ptr<Unit> unit = e->get_unit_production().get_unit();

			entry.put("type", "build-start");
			entry.put("name", unit->get_unit_blueprint().get_name());
		}

		if (UnitProductionFinishEvent* e = dynamic_cast<UnitProductionFinishEvent*>(event->get())) {
			std::shared_ptr<Unit> unit = e->get_unit_production().get_unit();

			entry.put("type", "build-end");
			entry.put("name", unit->get_unit_blueprint().get_name());
		}

		event_array.push_back(std::make_pair("", entry));
	}

	message.add_child("events", event_array);
	messages.push_back(std::make_pair("", message));
}

//void OutputFormatter::initialize() {}

void OutputFormatter::print() {
	// only add messages if the build list is considered valid
	if (output.get<std::string>("buildListValid") == "1") {
		output.add_child("messages", messages);
	}

	// write json to output
	boost::property_tree::json_parser::write_json(std::cout, output);
}

void OutputFormatter::set_validity(bool valid) {
	if (valid) {
		output.put("buildListValid", "1");
	} else {
		output.put("buildListValid", "0");
	}
}