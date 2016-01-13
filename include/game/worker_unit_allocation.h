#pragma once 

#include "game/unit.h"

#include <list>
#include <memory>

class WorkerUnitAllocation {
public:
	WorkerUnitAllocation(const std::list<std::shared_ptr<Unit>>& mineral_collecting_worker_units,
		const std::list<std::shared_ptr<Unit>>& vespene_gas_collecting_worker_units);

	const std::list<std::shared_ptr<Unit>>& get_mineral_collecting_worker_units() const;
	const std::list<std::shared_ptr<Unit>>& get_vespene_gas_collecting_worker_units() const;
private:
	std::list<std::shared_ptr<Unit>> m_mineral_collecting_worker_units;
	std::list<std::shared_ptr<Unit>> m_vespene_gas_collecting_worker_units;
};
