#pragma once 

#include "game/race.h"

#include <functional>
#include <list>
#include <string>
#include <type_traits>

class Blueprint {
public:
	Blueprint(const Race& race,
		const std::string& name,
		unsigned int mineral_costs,
		unsigned int vespene_gas_costs,
		const std::list<std::reference_wrapper<const Blueprint>>& dependency_blueprints,
		unsigned int creation_duration_seconds,
		unsigned int life_duration_seconds,
		unsigned int start_energy,
		unsigned int max_energy,
		unsigned int supply_provided);
	virtual ~Blueprint() = 0;

	const Race& get_race() const;
	const std::string& get_name() const;
	unsigned int get_mineral_costs() const;
	unsigned int get_vespene_gas_costs() const;
	const std::list<std::reference_wrapper<const Blueprint>>& get_dependency_blueprints() const;
	void add_dependency_blueprint(const Blueprint& blueprint);
	unsigned int get_creation_duration_seconds() const;
	unsigned int get_life_duration_seconds() const;
	unsigned int get_start_energy() const;
	unsigned int get_max_energy() const;
	unsigned int get_supply_provided() const;
private:
	Race m_race;
	std::string m_name;
	unsigned int m_mineral_costs;
	unsigned int m_vespene_gas_costs;
	std::list<std::reference_wrapper<const Blueprint>> m_dependency_blueprints;
	unsigned int m_creation_duration_seconds;
	unsigned int m_life_duration_seconds;
	unsigned int m_start_energy;
	unsigned int m_max_energy;
	unsigned int m_supply_provided;
};

template<class T_a, class T_b>
bool operator==(const std::list<std::reference_wrapper<const T_a>>& blueprints_a, const std::list<std::reference_wrapper<const T_b>>& blueprints_b) {
	static_assert(std::is_base_of<Blueprint, T_a>::value, "T_a has to be derived class of Blueprint.");
	static_assert(std::is_base_of<Blueprint, T_b>::value, "T_b has to be derived class of Blueprint.");

	std::list<std::reference_wrapper<const T_b>> blueprints_b_copy = blueprints_b;
	for (auto i_blueprints_a = blueprints_a.begin(); i_blueprints_a != blueprints_a.end(); ++i_blueprints_a) {
		auto i_blueprints_b_copy = blueprints_b_copy.begin();
		bool is_erased = false;
		while (i_blueprints_b_copy != blueprints_b_copy.end()) {
			if (&i_blueprints_a->get() == &i_blueprints_b_copy->get()) {
				i_blueprints_b_copy = blueprints_b_copy.erase(i_blueprints_b_copy);
				is_erased = true;
				break;
			} else {
				++i_blueprints_b_copy;
			}
		}
		if (!is_erased) {
			return false;
		}
	}
	if (blueprints_b_copy.size() > 0) {
		return false;
	}
	return true;
}
