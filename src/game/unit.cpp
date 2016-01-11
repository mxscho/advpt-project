#include "game/unit.h"

Unit::Unit(const UnitBlueprint& unit_blueprint)
	: m_unit_blueprint(unit_blueprint) {
}

const UnitBlueprint& Unit::get_unit_blueprint() const {
	return m_unit_blueprint;
}
