#include "MineDragAction.h"

#include <limits>

#include "UnitTracker.h"
#include "UnitInformation.h"

using namespace SkynetBot;

bool MineDragAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	for each(Unit unit in UnitInformation::Instance().getUnitsTargetting(mUnit))
	{
		if(unit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		{
			int distance = std::numeric_limits<int>::max();
			Unit closestUnit;

			for each(Unit enemyUnit in UnitTracker::Instance().selectAllEnemy())
			{
				if(enemyUnit->getType().isFlyer() || enemyUnit->isLifted() || enemyUnit->getType().isBuilding() || enemyUnit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
					continue;

				int thisDistance = mUnit->getDistance(enemyUnit);
				if(thisDistance < distance)
				{
					distance = distance;
					closestUnit = enemyUnit;
				}
			}
			
			if(closestUnit && distance < 32*5)
			{
				mUnit->attack(closestUnit);
				return true;
			}
		}
	}

	return false;
}