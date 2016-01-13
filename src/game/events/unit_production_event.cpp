#include "game/events/unit_production_event.h"

UnitProductionEvent::UnitProductionEvent(const std::shared_ptr<UnitProduction>& unit_production)
	: Event(),
	m_unit_production(unit_production) {
}

const UnitProduction& UnitProductionEvent::get_unit_production() const {
	return *m_unit_production;
}
