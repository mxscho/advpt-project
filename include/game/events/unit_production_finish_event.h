#pragma once 

#include "game/events/unit_production_event.h"

class UnitProduction;

class UnitProductionFinishEvent : public UnitProductionEvent {
public:
	explicit UnitProductionFinishEvent(const std::shared_ptr<UnitProduction>& unit_production);
};
