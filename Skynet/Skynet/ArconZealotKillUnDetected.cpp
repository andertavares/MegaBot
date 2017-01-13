#include "ArconZealotKillUnDetected.h"

#include "UnitTracker.h"
#include "UnitInformation.h"
#include "LatencyTracker.h"
#include "Logger.h"

using namespace SkynetBot;

bool ArconZealotKillUnDetected::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	if(mUnit->getOrder() == BWAPI::Orders::CompletingArchonsummon)
		return true;

	const int minDistance = 32*5;

	Unit lurker;
	int lurkerDistance = std::numeric_limits<int>::max();
	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		if(unit->exists() && !unit->isDetected() && (unit->isCloaked() || unit->getType().hasPermanentCloak() || (unit->getType() == BWAPI::UnitTypes::Zerg_Lurker && unit->isBurrowed())))
		{
			int thisDistance = mUnit->getDistance(unit);
			if(thisDistance < lurkerDistance)
			{
				lurkerDistance = thisDistance;
				lurker = unit;
			}
		}
	}

	if(!lurker || lurkerDistance > minDistance)
		return false;

	BWAPI::UnitType typeToFind = mUnit->getType() == BWAPI::UnitTypes::Protoss_Archon ? BWAPI::UnitTypes::Protoss_Zealot : BWAPI::UnitTypes::Protoss_Archon;
	Unit other = UnitTracker::Instance().selectAllUnits(typeToFind).getClosestUnit(lurker);

	if(!other)
		return false;

	int otherDistance = other->getDistance(lurker);
	if(otherDistance > minDistance)
		return false;

	if(typeToFind == BWAPI::UnitTypes::Protoss_Archon)
	{
		if(mUnit != UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_Zealot).getClosestUnit(lurker))
			return false;

		mUnit->move(lurker->getPosition());
		return true;
	}
	else
	{
		if(otherDistance <= 14)
		{
			mUnit->attack(other);
			return true;
		}
		else
		{
			mUnit->move(other->getPosition());
			return true;
		}
	}

	return false;
}