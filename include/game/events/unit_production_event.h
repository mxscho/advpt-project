#pragma once 

#include "game/events/event.h"

#include <memory>

class UnitProduction;

class UnitProductionEvent : public Event {
public:
	explicit UnitProductionEvent(const std::shared_ptr<UnitProduction>& unit_production);

	const UnitProduction& get_unit_production() const;
private:
	std::shared_ptr<UnitProduction> m_unit_production;
};
