#pragma once 

class UnitBlueprint;

class Unit {
public:
	explicit Unit(const UnitBlueprint& unit_blueprint);

	const UnitBlueprint& get_unit_blueprint() const;
private:
	const UnitBlueprint& m_unit_blueprint;
};
