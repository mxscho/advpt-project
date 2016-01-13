#pragma once 

#include "game/game.h"

class ZergGame : public Game {
public:
	static unsigned int c_larva_spawn_duration;

	ZergGame(unsigned int mineral_count, unsigned int vespene_gas_count);

	void inject_larva();

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	unsigned int m_larva_spawn_remaining_duration;
};
