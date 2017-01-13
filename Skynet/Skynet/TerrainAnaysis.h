#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "RectangleArray.h"
#include "Chokepoint.h"
#include "Region.h"

class TerrainAnaysisClass
{
public:
	TerrainAnaysisClass();

	void analyseBasesAndTerrain();
	void finaliseConnectivity();

	inline const std::set<Region> &getRegions() const { return mRegions; }
	inline const std::set<Chokepoint> &getChokepoints() const { return mChokepoints; }

	inline const Region &getRegion(const TilePosition &tile) const { return getRegion(tile.x*4 ,tile.y*4); }
	inline const Region &getRegion(const Position &pos) const { return getRegion(pos.x/8, pos.y/8); }
	inline const Region &getRegion(const WalkPosition &tile) const { return getRegion(tile.x, tile.y); }
	const Region &getRegion(int x, int y) const
	{
		if(x >= mMapWidth) x = mMapWidth - 1;
		else if(x < 0) x = 0;

		if(y >= mMapHeight) y = mMapHeight - 1;
		else if(y < 0) y = 0;

		return mTileToRegion[x][y];
	}

	inline int getWalkClearance(WalkPosition currentTile) { return getWalkClearance(currentTile.x, currentTile.y); }
	int getWalkClearance(int x, int y)
	{
		if(x >= mMapWidth) x = mMapWidth - 1;
		else if(x < 0) x = 0;

		if(y >= mMapHeight) y = mMapHeight - 1;
		else if(y < 0) y = 0;

		return mTileClearance[x][y];
	}

	inline int getWalkConnectivity(WalkPosition currentTile) { return getWalkConnectivity(currentTile.x, currentTile.y); }
	int getWalkConnectivity(int x, int y)
	{
		if(x >= mMapWidth) x = mMapWidth - 1;
		else if(x < 0) x = 0;

		if(y >= mMapHeight) y = mMapHeight - 1;
		else if(y < 0) y = 0;

		return mTileConnectivity[x][y];
	}

private:
	int mMapWidth;
	int mMapHeight;

	std::set<Chokepoint> mChokepoints;
	std::set<Region> mRegions;
	RectangleArray<Region> mTileToRegion;

	RectangleArray<int> mTileConnectivity;
	std::set<int> mSmallObstacles;

	RectangleArray<int> mTileClearance;
	RectangleArray<WalkPosition> mTileClosestObstacle;

	void calculateConnectivity();
	void calculateWalkTileClearance();
	void createRegions();
	void createBases();

	std::pair<WalkPosition, WalkPosition> findChokePoint(WalkPosition center);

	void removeChokepoint(Chokepoint chokeToRemove);
};

typedef Singleton<TerrainAnaysisClass> TerrainAnaysis;