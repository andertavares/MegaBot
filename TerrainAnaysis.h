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

	const std::set<Region> &getRegions() const { return mRegions; }
	const std::set<Chokepoint> &getChokepoints() const { return mChokepoints; }

	const Region &getRegion(TilePosition tile) const { return mTileToRegion[tile.x()*4][tile.y()*4]; }
	const Region &getRegion(Position pos) const { return mTileToRegion[pos.x()/8][pos.y()/8]; }
	const Region &getRegion(WalkPosition tile) const { return mTileToRegion[tile.x][tile.y]; }
	const Region &getRegion(int x, int y) const { return mTileToRegion[x][y]; }

	int getWalkClearance(WalkPosition currentTile) { return mTileClearance[currentTile.x][currentTile.y]; }
	int getWalkClearance(int x, int y) { return mTileClearance[x][y]; }

	int getWalkConnectivity(WalkPosition currentTile) { return mTileConnectivity[currentTile.x][currentTile.y]; }
	int getWalkConnectivity(int x, int y) { return mTileConnectivity[x][y]; }

private:
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