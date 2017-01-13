#include "AOEThreatTracker.h"

#include <limits>

#include "UnitTracker.h"

void AOEThreatTrackerClass::update()
{
	for(std::map<Unit, AOEThreat>::iterator it = mUnitThreats.begin(); it != mUnitThreats.end();)
	{
		if(it->second->hasGone())
			mUnitThreats.erase(it++);
		else
			++it;
	}

	for(std::map<BWAPI::Bullet, AOEThreat>::iterator it = mBulletThreats.begin(); it != mBulletThreats.end();)
	{
		if(it->second->hasGone())
			mBulletThreats.erase(it++);
		else
			++it;
	}

	for(std::set<AOEThreat>::iterator it = mAllThreats.begin(); it != mAllThreats.end();)
	{
		if((*it)->hasGone())
			mAllThreats.erase(it++);
		else
			++it;
	}

	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		const BWAPI::UnitType &type = unit->getType();
		if((type == BWAPI::UnitTypes::Protoss_Scarab || type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine) && mUnitThreats.count(unit) == 0)
		{
			AOEThreat newThreat = AOEThreat(new AOEThreatClass(unit));
			mAllThreats.insert(newThreat);
			mUnitThreats[unit] = newThreat;
		}
	}

	for each(BWAPI::Bullet bullet in BWAPI::Broodwar->getBullets())
	{
		const BWAPI::BulletType &type = bullet->getType();
		if((type == BWAPI::BulletTypes::Psionic_Storm || type == BWAPI::BulletTypes::EMP_Missile) && mBulletThreats.count(bullet) == 0)
		{
			AOEThreat newThreat = AOEThreat(new AOEThreatClass(bullet));
			mAllThreats.insert(newThreat);
			mBulletThreats[bullet] = newThreat;
		}
	}
}

AOEThreat AOEThreatTrackerClass::getClosestGroundThreat(const Position &pos) const
{
	int closestDistance = std::numeric_limits<int>::max();
	AOEThreat closestThreat;

	for each(AOEThreat threat in mAllThreats)
	{
		if(!threat->isGroundThreat())
			continue;

		int distance = pos.getApproxDistance(threat->getPosition()) - threat->getRadius();
		if(distance < closestDistance)
		{
			closestDistance = distance;
			closestThreat = threat;
		}
	}

	return closestThreat;
}


AOEThreat AOEThreatTrackerClass::getClosestAirThreat(const Position &pos) const
{
	int closestDistance = std::numeric_limits<int>::max();
	AOEThreat closestThreat;

	for each(AOEThreat threat in mAllThreats)
	{
		if(!threat->isAirThreat())
			continue;

		int distance = pos.getApproxDistance(threat->getPosition()) - threat->getRadius();
		if(distance < closestDistance)
		{
			closestDistance = distance;
			closestThreat = threat;
		}
	}

	return closestThreat;
}

AOEThreat AOEThreatTrackerClass::getClosestEnergyThreat(const Position &pos) const
{
	int closestDistance = std::numeric_limits<int>::max();
	AOEThreat closestThreat;

	for each(AOEThreat threat in mAllThreats)
	{
		if(!threat->isEnergyThreat())
			continue;

		int distance = pos.getApproxDistance(threat->getPosition()) - threat->getRadius();
		if(distance < closestDistance)
		{
			closestDistance = distance;
			closestThreat = threat;
		}
	}

	return closestThreat;
}

bool AOEThreatTrackerClass::isTargetOfThreat(Unit unit) const
{
	for each(AOEThreat threat in mAllThreats)
	{
		if(threat->getTarget() == unit)
			return true;
	}

	return false;
}

AOEThreat AOEThreatTrackerClass::getClosestThreat(Unit unit) const
{
	const bool isInAir = unit->getType().isFlyer() || unit->isLifted();
	const bool usesEnergy = unit->getType().maxEnergy() > 0;

	int closestDistance = std::numeric_limits<int>::max();
	AOEThreat closestThreat;

	for each(AOEThreat threat in mAllThreats)
	{
		if(threat->isEnergyThreat())
		{
			if(!usesEnergy)
				continue;
		}
		else if(threat->isAirThreat())
		{
			if(!isInAir)
				continue;
		}
		else if(threat->isGroundThreat())
		{
			if(isInAir)
				continue;
		}

		const int distance = unit->getDistance(threat->getPosition()) - threat->getRadius();
		if(distance < closestDistance)
		{
			closestDistance = distance;
			closestThreat = threat;
		}
	}

	return closestThreat;
}