#pragma once
#include <boost\tr1\memory.hpp>
#include <boost\tr1\functional.hpp>
#include "Interface.h"

#include "Singleton.h"
#include "BuildTilePath.h"
#include "PositionPath.h"
#include "RegionPath.h"
#include "WalkPositionPath.h"
#include "UnitGroup.h"
#include "BuildingPlacer.h"

using namespace SkynetBot;

class DefaultGValue
{
public:
	int operator()(TilePosition currentTile, TilePosition previousTile, int gTotal)
	{
		gTotal += 10;
		if (currentTile.x() != previousTile.x() && currentTile.y() != previousTile.y())
			gTotal += 4;

		return gTotal;
	}
};

class DefaultWalkGValue
{
public:
	int operator()(WalkPosition currentTile, WalkPosition previousTile, int gTotal)
	{
		gTotal += 10;
		if (currentTile.x != previousTile.x && currentTile.y != previousTile.y)
			gTotal += 4;

		return gTotal;
	}
};

class DefaultHValue
{
public:
	int operator()(TilePosition position, TilePosition target)
	{
		int dx = abs(position.x() - target.x());
		int dy = abs(position.x() - target.y());
		return abs(dx - dy) * 10 + std::min(dx, dy) * 14;
	}
};

class DefaultWalkHValue
{
public:
	int operator()(WalkPosition position, WalkPosition target)
	{
		int dx = abs(position.x - target.x);
		int dy = abs(position.x - target.y);
		return abs(dx - dy) * 10 + std::min(dx, dy) * 14;
	}
};

class DefaultTileTest
{
public:
	bool operator()(TilePosition position)
	{
		return BuildingPlacer::Instance().isTileWalkable(position);
	}
};

class DefaultWalkTest
{
public:
	bool operator()(WalkPosition position)
	{
		return BWAPI::Broodwar->isWalkable(position.x, position.y);
	}
};

class PathFinderClass
{
public:
	BuildTilePath CreateTilePath(TilePosition start, TilePosition target, std::tr1::function<bool (TilePosition)> tileTest = DefaultTileTest(), std::tr1::function<int (TilePosition, TilePosition, int)> gFunction = DefaultGValue(), std::tr1::function<int (TilePosition, TilePosition)> hFunction = DefaultHValue(), int maxGValue = 0, bool diaganol = false);
	BuildTilePath CreateAdvancedFleePath(TilePosition start, const UnitGroup &enemies, bool stickToRegion = false);

	RegionPath CreateRegionPath(Region start, Region target);

	PositionPath CreateCheapWalkPath(Position start, Position target);

	WalkPositionPath CreateWalkPath(WalkPosition start, WalkPosition target, std::tr1::function<bool (WalkPosition)> tileTest = DefaultWalkTest(), std::tr1::function<int (WalkPosition, WalkPosition, int)> gFunction = DefaultWalkGValue(), std::tr1::function<int (WalkPosition, WalkPosition)> hFunction = DefaultWalkHValue(), int maxGValue = 0, bool diaganol = false);
};

typedef Singleton<PathFinderClass> PathFinder;