#pragma once

#include "Interface.h"

#include "Singleton.h"

class WallTrackerClass
{
public:
	WallTrackerClass(){}

	void onBegin();

	TilePosition getWallPosition(BWAPI::UnitType type);
	bool canForgeExpand();

	std::map<TilePosition, BWAPI::UnitType> getWallTiles();

private:
	std::map<BWAPI::UnitType, std::list<TilePosition>> mWallPositions;
};

typedef Singleton<WallTrackerClass> WallTracker;