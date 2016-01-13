#pragma once 

class WorkerUnitAllocation {
public:
	WorkerUnitAllocation(unsigned int mineral_collecting_count, unsigned int vespene_gas_collecting_count);

	unsigned int get_mineral_collecting_count() const;
	unsigned int get_vespene_gas_collecting_count() const;
private:
	unsigned int m_mineral_collecting_count;
	unsigned int m_vespene_gas_collecting_count;
};
