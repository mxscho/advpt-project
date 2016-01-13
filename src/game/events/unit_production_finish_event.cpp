#include "game/events/unit_production_finish_event.h"

UnitProductionFinishEvent::UnitProductionFinishEvent(const std::shared_ptr<UnitProduction>& unit_production)
	: UnitProductionEvent(unit_production) {
}
