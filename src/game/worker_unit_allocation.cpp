#include "game/worker_unit_allocation.h"

WorkerUnitAllocation::WorkerUnitAllocation(const std::list<std::shared_ptr<Unit>>& mineral_collecting_worker_units,
	const std::list<std::shared_ptr<Unit>>& vespene_gas_collecting_worker_units)
	: m_mineral_collecting_worker_units(mineral_collecting_worker_units),
	m_vespene_gas_collecting_worker_units(vespene_gas_collecting_worker_units) {
}

const std::list<std::shared_ptr<Unit>>& WorkerUnitAllocation::get_mineral_collecting_worker_units() const {
	return m_mineral_collecting_worker_units;
}
const std::list<std::shared_ptr<Unit>>& WorkerUnitAllocation::get_vespene_gas_collecting_worker_units() const {
	return m_vespene_gas_collecting_worker_units;
}
