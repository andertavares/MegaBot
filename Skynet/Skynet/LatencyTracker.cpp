#include "LatencyTracker.h"

const int StormRadius = 64;
const int StasisRadius = 96;

void LatencyTrackerClass::update()
{
	const int time = BWAPI::Broodwar->getFrameCount();
	for(std::map<Unit, std::pair<Position, int>>::iterator it = mStormedPositions.begin(); it != mStormedPositions.end();)
	{
		if(!it->first->exists() || (it->first->getOrder() != BWAPI::Orders::CastPsionicStorm && time > it->second.second))
			mStormedPositions.erase(it++);
		else
			++it;
	}

	for(std::map<Unit, std::pair<Position, int>>::iterator it = mStasisPositions.begin(); it != mStasisPositions.end();)
	{
		if(!it->first->exists() || (it->first->getOrder() != BWAPI::Orders::CastStasisField && time > it->second.second))
			mStasisPositions.erase(it++);
		else
			++it;
	}
}

void LatencyTrackerClass::placingStorm(Unit unit, Position pos)
{
	mStormedPositions[unit] = std::make_pair(pos, BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames());
}

void LatencyTrackerClass::placingStasis(Unit unit, Position pos)
{
	mStasisPositions[unit] = std::make_pair(pos, BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames());
}

bool LatencyTrackerClass::isStormInRange(Unit unit)
{
	if(mStormedPositions.empty())
		return false;

	for(std::map<Unit, std::pair<Position, int>>::iterator it = mStormedPositions.begin(); it != mStormedPositions.end(); ++it)
	{
		if(it->second.first.getApproxDistance(unit->getPosition()) <= StormRadius)
			return true;
	}

	return false;
}

bool LatencyTrackerClass::isStasisInRange(Unit unit)
{
	if(mStasisPositions.empty())
		return false;

	for(std::map<Unit, std::pair<Position, int>>::iterator it = mStasisPositions.begin(); it != mStasisPositions.end(); ++it)
	{
		if(it->second.first.getApproxDistance(unit->getPosition()) <= StasisRadius)
			return true;
	}

	return false;
}