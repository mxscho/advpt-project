#pragma once 

#include "game/game.h"
#include "game/terran_building_construction.h"
#include "game/updatable.h"

#include <memory>

class Event;

class TerranGame : public Game {
public:
	TerranGame(unsigned int mineral_count, unsigned int vespene_gas_count);

	virtual bool can_construct_buildings_by_names(const std::list<std::string>& names) const override;
	void call_mule();
protected:
	virtual std::list<std::shared_ptr<Unit>> find_worker_units() const override;
	virtual std::unique_ptr<BuildingConstruction> create_building_construction(const BuildingBlueprint& building_blueprint) override;
private:
	std::list<std::shared_ptr<Unit>> find_builder_units() const;
};
