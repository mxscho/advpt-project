#pragma once 

#include "game/events/unit_production_event.h"

class UnitProduction;

class UnitProductionStartEvent : public UnitProductionEvent {
public:
	explicit UnitProductionStartEvent(const std::shared_ptr<UnitProduction>& unit_production);
};
