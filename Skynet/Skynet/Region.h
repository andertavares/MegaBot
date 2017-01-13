#pragma once

#include "Interface.h"

class ChokepointClass;
typedef std::tr1::shared_ptr<ChokepointClass> Chokepoint;
typedef std::tr1::weak_ptr<ChokepointClass> WeakChokepoint;

class BaseClass;
typedef std::tr1::shared_ptr<BaseClass> Base;

class RegionClass
{
public:
	RegionClass(Position center, int clearance);

	std::set<Chokepoint> getChokepoints() const;

	void addChokepoint(WeakChokepoint chokepoint);
	void addChokepointTile(WeakChokepoint chokepoint, TilePosition tile);

	const std::set<TilePosition> &getChokepointTiles() { return mChokePointTiles; }

	void addBase(Base base) { mBases.insert(base); }
	const std::set<Base> &getBases() const { return mBases; }

	const Position &getCenter() const { return mCenter; }
	int getClearance() const { return mClearance; }

	bool isConnected(std::tr1::shared_ptr<RegionClass> region) const;

	void setSize(int size) { mSize = size; }
	int getSize() const { return mSize; }

	void draw(BWAPI::Color color);

	void removeChokepoint(WeakChokepoint chokepoint);

private:
	Position mCenter;
	int mClearance;
	int mSize;

	std::set<WeakChokepoint> mChokepoints;
	std::map<WeakChokepoint, TilePosition> mChokePointToTiles;
	std::set<TilePosition> mChokePointTiles;

	std::set<Base> mBases;
};

typedef std::tr1::shared_ptr<RegionClass> Region;
typedef std::tr1::weak_ptr<RegionClass> WeakRegion;