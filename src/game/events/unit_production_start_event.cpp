#include "game/events/unit_production_start_event.h"

UnitProductionStartEvent::UnitProductionStartEvent(const std::shared_ptr<UnitProduction>& unit_production)
	: UnitProductionEvent(unit_production) {
}
