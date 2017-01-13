#pragma once

#include "Interface.h"

class RegionClass;
typedef std::tr1::shared_ptr<RegionClass> Region;
typedef std::tr1::weak_ptr<RegionClass> WeakRegion;

class ChokepointClass
{
public:
	ChokepointClass(Position side1, Position side2, int clearance);

	std::pair<Position, Position> getSides() { return mSides; }
	Position getCenter() { return mCenter; }

	std::pair<Region, Region> getRegions();

	const std::pair<TilePosition, TilePosition> &getBuildTiles() { return mChoketiles; }
	void addChokepointTile(WeakRegion region, TilePosition tile);

	void draw(BWAPI::Color color);

	void setRegion1(WeakRegion region) { mRegions.first = region; }
	void setRegion2(WeakRegion region) { mRegions.second = region; }

	int getClearance() const { return mClearance; }

private:
	std::pair<WeakRegion, WeakRegion> mRegions;

	std::pair<TilePosition, TilePosition> mChoketiles;

	Position mCenter;
	std::pair<Position, Position> mSides;
	int mClearance;
};

typedef std::tr1::shared_ptr<ChokepointClass> Chokepoint;
typedef std::tr1::weak_ptr<ChokepointClass> WeakChokepoint;