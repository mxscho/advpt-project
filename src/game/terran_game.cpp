#include "game/terran_game.h"

#include <algorithm>
#include <iterator>

#include <cassert>

TerranGame::TerranGame(unsigned int mineral_count, unsigned int vespene_gas_count)
	: Game(mineral_count, vespene_gas_count) {
}

Race TerranGame::get_race() const {
	return Race::Terran;
}
TerranGame* TerranGame::clone() const {
	return new TerranGame(*this);
}

bool TerranGame::can_construct_buildings_by_names(const std::list<std::string>& names) const {
	return Game::can_construct_buildings_by_names(names) && find_builder_units().size() > 0;
}
void TerranGame::call_mule() {
	throw; // TODO
}

std::list<std::shared_ptr<Unit>> TerranGame::find_worker_units() const {
	std::list<std::shared_ptr<Unit>> worker_units = Game::find_worker_units();
	// Remove currently building worker units.
	for (auto i = m_current_building_constructions.begin(); i != m_current_building_constructions.end(); ++i) {
		if (TerranBuildingConstruction* terran_building_construction = dynamic_cast<TerranBuildingConstruction*>(i->get())) {
			if (!(*i)->is_finished()) {
				worker_units.remove_if([&terran_building_construction](const std::shared_ptr<Unit>& worker_unit) { return worker_unit.get() == &terran_building_construction->get_builder_unit(); });
			}
		}
	}
	return worker_units;
}

std::list<std::shared_ptr<Unit>> TerranGame::find_builder_units() const {
	std::list<std::shared_ptr<Unit>> builder_units;
	std::copy_if(m_units.begin(), m_units.end(), std::back_inserter(builder_units), [](const std::shared_ptr<Unit>& unit) {
		return unit->get_unit_blueprint().is_builder();
	});
	// Remove currently building worker units.
	for (auto i = m_current_building_constructions.begin(); i != m_current_building_constructions.end(); ++i) {
		if (TerranBuildingConstruction* terran_building_construction = dynamic_cast<TerranBuildingConstruction*>(i->get())) {
			if (!(*i)->is_finished()) {
				builder_units.remove_if([&terran_building_construction](const std::shared_ptr<Unit>& builder_unit) { return builder_unit.get() == &terran_building_construction->get_builder_unit(); });
			}
		}
	}
	return builder_units;
}
std::unique_ptr<BuildingConstruction> TerranGame::create_building_construction(const BuildingBlueprint& building_blueprint, bool is_morphing) {
	if (is_morphing) {
		return std::unique_ptr<BuildingConstruction>(new BuildingConstruction(building_blueprint));
	} else {
		return std::unique_ptr<BuildingConstruction>(new TerranBuildingConstruction(building_blueprint, *find_builder_units().front()));
	}
}
