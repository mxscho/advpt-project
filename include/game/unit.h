#pragma once 

#include "game/energetic.h"
#include "game/identifiable.h"
#include "game/mortal.h"
#include "game/unit_blueprint.h"

#include <functional>
#include <list>
#include <type_traits>

class Blueprint;

class Unit : public Identifiable, public Energetic, public Mortal {
public:
	explicit Unit(const UnitBlueprint& unit_blueprint);

	const UnitBlueprint& get_unit_blueprint() const;
	bool is_building() const;
	void toggle_building();

	template<class T>
	bool can_morph(const std::list<std::reference_wrapper<const T>>& blueprints) const {
		static_assert(std::is_base_of<Blueprint, T>::value, "T has to be derived class of Blueprint.");

		if (m_is_building) {
			return false;
		}
		auto& morphable_blueprints = m_unit_blueprint.get_morphable_blueprints();
		for (auto i = morphable_blueprints.begin(); i != morphable_blueprints.end(); ++i) {
			if (*i == blueprints) {
				return true;
			}
		}
		return false;
	}

	virtual std::list<std::unique_ptr<Event>> update(unsigned int elapsed_time_seconds) override;
private:
	const UnitBlueprint& m_unit_blueprint;

	bool m_is_building;
};
