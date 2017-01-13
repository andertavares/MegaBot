#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Task.h"

class LatencyTrackerClass
{
public:
	LatencyTrackerClass(){}

	void update();

	void placingStorm(Unit unit, Position pos);
	void placingStasis(Unit unit, Position pos);

	bool isStormInRange(Unit unit);
	bool isStasisInRange(Unit unit);

private:
	std::map<Unit, std::pair<Position, int>> mStormedPositions;
	std::map<Unit, std::pair<Position, int>> mStasisPositions;
};

typedef Singleton<LatencyTrackerClass> LatencyTracker;