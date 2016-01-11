#pragma once 

#include "game/unit_blueprint.h"

#include <functional>
#include <list>
#include <type_traits>

class Blueprint;

class Unit {
public:
	explicit Unit(const UnitBlueprint& unit_blueprint);

	const UnitBlueprint& get_unit_blueprint() const;

	template<class T>
	bool can_morph(const std::list<std::reference_wrapper<const T>>& blueprints) const {
		static_assert(std::is_base_of<Blueprint, T>::value, "T has to be derived class of Blueprint.");

		auto& morphable_blueprints = m_unit_blueprint.get_morphable_blueprints();
		for (auto i = morphable_blueprints.begin(); i != morphable_blueprints.end(); ++i) {
			if (*i == blueprints) {
				return true;
			}
		}
		return false;
	}
private:
	const UnitBlueprint& m_unit_blueprint;
};
