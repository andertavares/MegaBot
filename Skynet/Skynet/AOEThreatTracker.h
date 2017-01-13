#pragma once

#include "Interface.h"
#include "Singleton.h"

#include "AOEThreat.h"

class AOEThreatTrackerClass
{
public:
	void update();

	AOEThreat getClosestGroundThreat(const Position &pos) const;
	AOEThreat getClosestAirThreat(const Position &pos) const;
	AOEThreat getClosestEnergyThreat(const Position &pos) const;
	AOEThreat getClosestThreat(Unit unit) const;

	bool isTargetOfThreat(Unit unit) const;

private:
	std::set<AOEThreat> mAllThreats;
	std::map<Unit, AOEThreat> mUnitThreats;
	std::map<BWAPI::Bullet, AOEThreat> mBulletThreats;
};
typedef Singleton<AOEThreatTrackerClass> AOEThreatTracker;