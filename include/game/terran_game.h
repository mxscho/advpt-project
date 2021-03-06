#pragma once 

#include "game/game.h"
#include "game/terran_building_construction.h"
#include "game/updatable.h"

#include <memory>

class Event;
enum class Race;

class TerranGame : public Game {
public:
	TerranGame(unsigned int mineral_count, unsigned int vespene_gas_count);

	virtual Race get_race() const override;
	virtual TerranGame* clone() const override;

	virtual bool can_construct_buildings_by_names(const std::list<std::string>& names) const override;
	void call_mule();
protected:
	virtual std::list<std::shared_ptr<Unit>> find_worker_units() const override;
	virtual std::unique_ptr<BuildingConstruction> create_building_construction(const BuildingBlueprint& building_blueprint, bool is_morphing) override;
private:
	std::list<std::shared_ptr<Unit>> find_builder_units() const;
};
