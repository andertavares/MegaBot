#include "UnitPredictor.h"
#include "UnitTracker.h"
#include "Logger.h"

UnitPredictorClass::UnitPredictorClass()
{
}

Unit UnitPredictorClass::createPrediction(Position pos, BWAPI::UnitType type, int startTime)
{
	return Unit(new UnitClass(pos, type, startTime));
}

void UnitPredictorClass::savePrediction(Unit unit)
{
	mUnits.insert(unit);
	UnitTracker::Instance().onDiscover(unit);
}

void UnitPredictorClass::deletePrediction(Unit unit)
{
	if(unit->accessibility() == AccessType::Prediction)
		UnitTracker::Instance().onDestroy(unit);

	mUnits.erase(unit);
}

Unit UnitPredictorClass::onNewUnit(BWAPI::Unit newUnit)
{
	for each(Unit unit in mUnits)
	{
		if(unit->getType() == newUnit->getType() && unit->getPlayer() == newUnit->getPlayer())
		{
			if(unit->getTilePosition() == newUnit->getTilePosition() || newUnit->getPosition().getDistance(unit->getPosition()) <= TILE_SIZE * 6)
			{
				unit->promote(newUnit);
				return unit;
			}
		}
	}

	return StaticUnits::nullunit;
}