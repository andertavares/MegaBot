#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Unit.h"
#include "UnitGroup.h"

using namespace SkynetBot;

class UnitPredictorClass
{
public:
	UnitPredictorClass();

	Unit createPrediction(Position pos, BWAPI::UnitType type, int startTime);

	void savePrediction(Unit unit);
	void deletePrediction(Unit unit);

	Unit onNewUnit(BWAPI::Unit* newUnit);

private:
	UnitGroup mUnits;
};

typedef Singleton<UnitPredictorClass> UnitPredictor;