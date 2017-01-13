#include "DetectorAction.h"

#include "Vector.h"
#include "UnitTracker.h"
#include "UnitHelper.h"

#include <limits>

using namespace SkynetBot;

bool DetectorAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	if(squadGoal.getActionType() == ActionType::Retreat || squadGoal.getActionType() == ActionType::FallBack)
		return false;

	Unit needsDetecting;
	int unitDistance = std::numeric_limits<int>::max();
	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		const BWAPI::UnitType &unitType = unit->getType();
		if(unitType.hasPermanentCloak() || unitType == BWAPI::UnitTypes::Zerg_Lurker || unit->isCloaked() || unit->isBurrowed())
		{
			if(!squadUnitGroup.isAnyInRange(unit))
				continue;

			int thisDistance = mUnit->getDistance(unit);
			if(thisDistance < unitDistance)
			{
				needsDetecting = unit;
				unitDistance = thisDistance;
			}
		}
	}

	if(needsDetecting)
	{
		if(unitDistance > BWAPI::UnitTypes::Protoss_Observer.sightRange())
		{
			mUnit->move(needsDetecting->getPosition());
			return true;
		}
	}

	UnitGroup protectionUnits;
	for each(Unit unit in squadUnitGroup)
	{
		if(!UnitHelper::isArmyUnit(unit->getType()))
			continue;

		if(unit->getType() == BWAPI::UnitTypes::Protoss_Arbiter || unit->getType().isBuilding())
			continue;

		if(mUnit->getDistance(unit) > 250)
			continue;

		protectionUnits.insert(unit);
	}

	if(!protectionUnits.empty())
	{
		protectionUnits = protectionUnits.getBestFittingToCircle(136);
		if(!protectionUnits.empty())
		{
			Position protectedArea = protectionUnits.getCenter();
			if(mUnit->getDistance(protectedArea) > 110)
			{
				mUnit->move(protectedArea);
				return true;
			}
		}
	}

	return false;
}