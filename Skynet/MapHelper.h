#pragma once

#include "Interface.h"

namespace MapHelper
{
	TilePosition spiralSearch(TilePosition startLocation, std::tr1::function<bool (TilePosition)> testFunction, int maxRadius = 0);
	std::set<TilePosition> floodFill(TilePosition start, std::tr1::function<bool (TilePosition)> testFunction, std::set<TilePosition> targets = std::set<TilePosition>(), std::set<TilePosition> ignoreTiles = std::set<TilePosition>());

	std::map<WalkPosition, int> walkSearch(WalkPosition start, std::tr1::function<bool (WalkPosition)> testFunction, std::set<WalkPosition> targets = std::set<WalkPosition>(), std::set<WalkPosition> ignoreTiles = std::set<WalkPosition>());

	bool isAnyVisible(TilePosition location, BWAPI::UnitType type);
	bool isAllVisible(TilePosition location, BWAPI::UnitType type);

	bool isTileWalkable(TilePosition location);
	bool isTileWalkable(int x, int y);

	bool mapIs(std::string name);

	int getGroundDistance(Position start, Position end);
}