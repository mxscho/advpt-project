#include "game/worker_unit_allocation.h"

WorkerUnitAllocation::WorkerUnitAllocation(unsigned int mineral_collecting_count, unsigned int vespene_gas_collecting_count)
	: m_mineral_collecting_count(mineral_collecting_count),
	m_vespene_gas_collecting_count(vespene_gas_collecting_count) {
}

unsigned int WorkerUnitAllocation::get_mineral_collecting_count() const {
	return m_mineral_collecting_count;
}
unsigned int WorkerUnitAllocation::get_vespene_gas_collecting_count() const {
	return m_vespene_gas_collecting_count;
}
