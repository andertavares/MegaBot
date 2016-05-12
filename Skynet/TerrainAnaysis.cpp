#include "TerrainAnaysis.h"

#include "UnitHelper.h"
#include "MapHelper.h"
#include "BaseTracker.h"
#include "DrawBuffer.h"
#include "UnitGroup.h"
#include "UnitTracker.h"
#include "MapHelper.h"
#include "BuildingPlacer.h"
#include "PathFinder.h"
#include "Logger.h"
#include "Heap.h"

TerrainAnaysisClass::TerrainAnaysisClass()
{
}

void TerrainAnaysisClass::analyseBasesAndTerrain()
{
	calculateConnectivity();
	calculateWalkTileClearance();
	createRegions();

	createBases();

	BuildingPlacer::Instance().calculateReservations();
	finaliseConnectivity();
	BuildingPlacer::Instance().finaliseReservations();
}

class WalkInRegionComp
{
public:
	WalkInRegionComp(Region region) : mRegion(region) {}

	bool operator()(TilePosition location) const
	{
		for(int x = location.x()-1; x <= location.x()+1; ++x)
		{
			for(int y = location.y()-1; y <= location.y()+1; ++y)
			{
				TilePosition tile(x, y);
				if(!tile.isValid() || TerrainAnaysis::Instance().getRegion(tile) != mRegion || !BuildingPlacer::Instance().isTileWalkable(tile))
					return false;
			}
		}

		return true;
	}

private:
	Region mRegion;
};

void TerrainAnaysisClass::finaliseConnectivity()
{
	for each(Region region in mRegions)
	{
		WalkInRegionComp goCompare(region);

		for each(Chokepoint chokepoint in mChokepoints)
		{
			TilePosition pos = MapHelper::spiralSearch(TilePosition(chokepoint->getCenter()), goCompare, 24);

			region->addChokepointTile(chokepoint, pos);
			chokepoint->addChokepointTile(region, pos);
		}
	}
}

void TerrainAnaysisClass::calculateWalkTileClearance()
{
	mTileClearance.resize(mMapWidth, mMapHeight, -1);
	mTileClosestObstacle.resize(mMapWidth, mMapHeight);

	Heap<WalkPosition, int> unvisitedTiles(true);
	for(int x = 0; x < mMapWidth; ++x)
	{
		for(int y = 0; y < mMapHeight; ++y)
		{
			if(!BWAPI::Broodwar->isWalkable(x, y))
			{
				mTileClearance[x][y] = 0;
				mTileClosestObstacle[x][y] = WalkPosition(x, y);
				if(mSmallObstacles.count(mTileConnectivity[x][y]) == 0)
					unvisitedTiles.set(WalkPosition(x, y), 0);
			}
			else if(x == 0 || y == 0 || x == mMapWidth-1 || y == mMapHeight-1)
			{
				mTileClearance[x][y] = 10;
				mTileClosestObstacle[x][y] = WalkPosition((x == 0 ? -1 : (x == mMapWidth-1 ? mMapWidth : x)), (y == 0 ? -1 : (y == mMapHeight-1 ? mMapHeight : y)));
				unvisitedTiles.set(WalkPosition(x, y), 10);
			}
		}
	}

	while(!unvisitedTiles.empty())
	{
		WalkPosition tile = unvisitedTiles.top().first;
		const int distance = unvisitedTiles.top().second + 10;
		const int diagDistance = distance + 4;
		unvisitedTiles.pop();

		const int west = tile.x - 1;
		const int north = tile.y - 1;
		const int east = tile.x + 1;
		const int south = tile.y + 1;

		const bool canGoWest = west >= 0;
		const bool canGoNorth = north >= 0;
		const bool canGoEast = east < mMapWidth;
		const bool canGoSouth = south < mMapHeight;

		const WalkPosition &currentParent = mTileClosestObstacle[tile.x][tile.y];

		if(canGoWest && (mTileClearance[west][tile.y] == -1 || distance < mTileClearance[west][tile.y]))
		{
			mTileClearance[west][tile.y] = distance;
			mTileClosestObstacle[west][tile.y] = currentParent;
			unvisitedTiles.set(WalkPosition(west, tile.y), distance);
		}

		if(canGoNorth && (mTileClearance[tile.x][north] == -1 || distance < mTileClearance[tile.x][north]))
		{
			mTileClearance[tile.x][north] = distance;
			mTileClosestObstacle[tile.x][north] = currentParent;
			unvisitedTiles.set(WalkPosition(tile.x, north), distance);
		}

		if(canGoEast && (mTileClearance[east][tile.y] == -1 || distance < mTileClearance[east][tile.y]))
		{
			mTileClearance[east][tile.y] = distance;
			mTileClosestObstacle[east][tile.y] = currentParent;
			unvisitedTiles.set(WalkPosition(east, tile.y), distance);
		}

		if(canGoSouth && (mTileClearance[tile.x][south] == -1 || distance < mTileClearance[tile.x][south]))
		{
			mTileClearance[tile.x][south] = distance;
			mTileClosestObstacle[tile.x][south] = currentParent;
			unvisitedTiles.set(WalkPosition(tile.x, south), distance);
		}

		if(canGoWest && canGoNorth && (mTileClearance[west][north] == -1 || diagDistance < mTileClearance[west][north]))
		{
			mTileClearance[west][north] = diagDistance;
			mTileClosestObstacle[west][north] = currentParent;
			unvisitedTiles.set(WalkPosition(west, north), diagDistance);
		}

		if(canGoEast && canGoSouth && (mTileClearance[east][south] == -1 || diagDistance < mTileClearance[east][south]))
		{
			mTileClearance[east][south] = diagDistance;
			mTileClosestObstacle[east][south] = currentParent;
			unvisitedTiles.set(WalkPosition(east, south), diagDistance);
		}

		if(canGoEast && canGoNorth && (mTileClearance[east][north] == -1 || diagDistance < mTileClearance[east][north]))
		{
			mTileClearance[east][north] = diagDistance;
			mTileClosestObstacle[east][north] = currentParent;
			unvisitedTiles.set(WalkPosition(east, north), diagDistance);
		}

		if(canGoWest && canGoSouth && (mTileClearance[west][south] == -1 || diagDistance < mTileClearance[west][south]))
		{
			mTileClearance[west][south] = diagDistance;
			mTileClosestObstacle[west][south] = currentParent;
			unvisitedTiles.set(WalkPosition(west, south), diagDistance);
		}
	}
}

std::pair<WalkPosition, WalkPosition> TerrainAnaysisClass::findChokePoint(WalkPosition center)
{
	WalkPosition side1 = mTileClosestObstacle[center.x][center.y];

	if(side1 == center)
		return std::make_pair(side1, side1);

	Vector side1Direction(side1.x - center.x, side1.y - center.y);
	side1Direction.normalise();

	int mapWidth = BWAPI::Broodwar->mapWidth() * 4;
	int mapHeight = BWAPI::Broodwar->mapHeight() * 4;

	int x0 = side1.x;
	int y0 = side1.y;

	int x1 = center.x;
	int y1 = center.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	x0 = x1;
	y0 = y1;

	int error = dx - dy;

	for(;;)
	{
		if(x0 < 0 || y0 < 0 || x0 >= mapWidth || y0 >= mapHeight || !BWAPI::Broodwar->isWalkable(x0, y0))
			return std::make_pair(side1, WalkPosition(x0, y0));

		WalkPosition side2 = mTileClosestObstacle[x0][y0];

		Vector side2Direction(side2.x - center.x, side2.y - center.y);
		side2Direction.normalise();

		float dot = side2Direction.dotProduct(side1Direction);
		float angle = acos(dot);
		if(angle > 2.0f)
			return std::make_pair(side1, side2);

		int e2 = error*2;
		if(e2 > -dy)
		{
			error -= dy;
			x0 += sx;
		}
		if(e2 < dx)
		{
			error += dx;
			y0 += sy;
		}
	}
}

void TerrainAnaysisClass::calculateConnectivity()
{
	mMapWidth = BWAPI::Broodwar->mapWidth() * 4;
	mMapHeight = BWAPI::Broodwar->mapHeight() * 4;

	int currentRegion = 1;
	mTileConnectivity.resize(mMapWidth, mMapHeight, 0);
	for(int x = 0; x < mMapWidth; ++x)
	{
		for(int y = 0; y < mMapHeight; ++y)
		{
			if(mTileConnectivity[x][y] != 0)
				continue;

			bool walkable = BWAPI::Broodwar->isWalkable(x, y);
			int tileCount = 0;

			std::set<WalkPosition> unvisitedTiles;
			unvisitedTiles.insert(WalkPosition(x, y));

			while(!unvisitedTiles.empty())
			{
				const std::set<WalkPosition>::iterator &tile = unvisitedTiles.begin();

				++tileCount;
				mTileConnectivity[tile->x][tile->y] = currentRegion;

				if(tile->x > 0 && BWAPI::Broodwar->isWalkable(tile->x-1, tile->y) == walkable && mTileConnectivity[tile->x-1][tile->y] == 0)
					unvisitedTiles.insert(WalkPosition(tile->x-1, tile->y));
				if(tile->y > 0 && BWAPI::Broodwar->isWalkable(tile->x, tile->y-1) == walkable && mTileConnectivity[tile->x][tile->y-1] == 0)
					unvisitedTiles.insert(WalkPosition(tile->x, tile->y-1));

				if(tile->x < mMapWidth-1 && BWAPI::Broodwar->isWalkable(tile->x+1, tile->y) == walkable && mTileConnectivity[tile->x+1][tile->y] == 0)
					unvisitedTiles.insert(WalkPosition(tile->x+1, tile->y));
				if(tile->y < mMapHeight-1 && BWAPI::Broodwar->isWalkable(tile->x, tile->y+1) == walkable && mTileConnectivity[tile->x][tile->y+1] == 0)
					unvisitedTiles.insert(WalkPosition(tile->x, tile->y+1));

				unvisitedTiles.erase(tile);
			}

			if(!walkable && tileCount < 200)
				mSmallObstacles.insert(currentRegion);

			++currentRegion;
		}
	}
}

void TerrainAnaysisClass::createRegions()
{
	mMapWidth = BWAPI::Broodwar->mapWidth() * 4;
	mMapHeight = BWAPI::Broodwar->mapHeight() * 4;

	mTileToRegion.resize(mMapWidth, mMapHeight);
	std::map<WalkPosition, Chokepoint> chokeTiles;

	for(;;)
	{
		int currentRegionClearance = 0;
		WalkPosition currentRegionTile;

		for(int x = 0; x < mMapWidth; ++x)
		{
			for(int y = 0; y < mMapHeight; ++y)
			{
				if(mTileToRegion[x][y])
					continue;

				if(chokeTiles.count(WalkPosition(x, y)) != 0)
					continue;

				const int localMaximaValue = mTileClearance[x][y];
				if(localMaximaValue > currentRegionClearance)
				{
					currentRegionClearance = localMaximaValue;
					currentRegionTile.x = x;
					currentRegionTile.y = y;
				}
			}
		}

		if(currentRegionClearance == 0)
			break;

		Region currentRegion(new RegionClass(currentRegionTile.getCenterPosition(), currentRegionClearance));
		mRegions.insert(currentRegion);

		std::map<WalkPosition, WalkPosition> tileToLastMinima;
		std::map<WalkPosition, std::vector<WalkPosition>> tileToChildren;

		Heap<WalkPosition, int> unvisitedTiles(false);

		unvisitedTiles.set(currentRegionTile, currentRegionClearance);
		tileToLastMinima[currentRegionTile] = currentRegionTile;

		while(!unvisitedTiles.empty())
		{
			WalkPosition currentTile = unvisitedTiles.top().first;
			int currentTileClearance = unvisitedTiles.top().second;
			unvisitedTiles.pop();

			if(chokeTiles.count(currentTile) != 0)
			{
				if(chokeTiles[currentTile]->getRegions().second && chokeTiles[currentTile]->getRegions().second != currentRegion)
				{
					DrawBuffer::Instance().drawBufferedBox(BWAPI::CoordinateType::Map, currentTile.x * 8, currentTile.y * 8, currentTile.x * 8 + 8, currentTile.y * 8 + 8, 999999, BWAPI::Colors::Red);
					LOGMESSAGEWARNING("Touched a choke saved to anouther region");
				}
				else if(chokeTiles[currentTile]->getRegions().first != currentRegion)
				{
					currentRegion->addChokepoint(chokeTiles[currentTile]);
					chokeTiles[currentTile]->setRegion2(currentRegion);
				}

				continue;
			}

			if(mTileToRegion[currentTile.x][currentTile.y])
			{
				LOGMESSAGEWARNING("2 regions possibly connected without a choke");
				continue;
			}

			WalkPosition lastMinima = tileToLastMinima[currentTile];
			const int chokeSize = mTileClearance[lastMinima.x][lastMinima.y];

			bool foundChokepoint = false;
			if(chokeSize < int(float(currentRegionClearance)*0.90f) && chokeSize < int(float(currentTileClearance)*0.80f))
				foundChokepoint = true;

			if(foundChokepoint)
			{
				const int minDistance = 32;
				if((abs(currentRegionTile.x - lastMinima.x) + abs(currentRegionTile.y - lastMinima.y)) < minDistance)
					foundChokepoint = false;
				else if((abs(currentTile.x - lastMinima.x) + abs(currentTile.y - lastMinima.y)) < minDistance)
					foundChokepoint = false;
				else if(currentTileClearance < 120)
					foundChokepoint = false;
			}

			if(foundChokepoint)
			{
				const std::pair<WalkPosition, WalkPosition> &chokeSides = findChokePoint(lastMinima);

				Chokepoint currentChokepoint(new ChokepointClass(chokeSides.first.getCenterPosition(), chokeSides.second.getCenterPosition(), chokeSize));
				mChokepoints.insert(currentChokepoint);

				currentChokepoint->setRegion1(currentRegion);
				currentRegion->addChokepoint(currentChokepoint);

				int x0 = chokeSides.second.x;
				int y0 = chokeSides.second.y;

				int x1 = chokeSides.first.x;
				int y1 = chokeSides.first.y;

				int dx = abs(x1 - x0);
				int dy = abs(y1 - y0);

				int sx = x0 < x1 ? 1 : -1;
				int sy = y0 < y1 ? 1 : -1;

				int error = dx - dy;

				std::set<WalkPosition> chokeChildren;

				for(;;)
				{
					if(x0 >= 0 && y0 >= 0 && x0 < mMapWidth && y0 < mMapHeight && mTileClearance[x0][y0] != 0 && !mTileToRegion[x0][y0])
					{
						const WalkPosition thisChokeTile(x0, y0);

						tileToChildren[currentRegionTile].push_back(thisChokeTile);
						chokeTiles[thisChokeTile] = currentChokepoint;
						chokeChildren.insert(thisChokeTile);
					}

					if(x0 == x1 && y0 == y1)
						break;

					int e2 = error*2;
					if(e2 > -dy)
					{
						error -= dy;
						x0 += sx;
					}
					if(e2 < dx)
					{
						error += dx;
						y0 += sy;
					}
				}

				while(!chokeChildren.empty())
				{
					std::set<WalkPosition>::iterator currentTile = chokeChildren.begin();

					tileToLastMinima.erase(*currentTile);
					unvisitedTiles.erase(*currentTile);

					for each(WalkPosition nextTile in tileToChildren[*currentTile])
					{
						chokeChildren.insert(nextTile);
					}
					tileToChildren.erase(*currentTile);

					chokeChildren.erase(currentTile);
				}
			}
			else
			{
				if(mTileClearance[currentTile.x][currentTile.y] < chokeSize)
					lastMinima = currentTile;

				for(int i = 0; i < 4; ++i)
				{
					int x = (i == 0 ? currentTile.x-1 : (i == 1 ? currentTile.x+1 : currentTile.x));
					int y = (i == 2 ? currentTile.y-1 : (i == 3 ? currentTile.y+1 : currentTile.y));

					if(x < 0 || y < 0 || x >= mMapWidth || y >= mMapHeight)
						continue;

					if(mTileClearance[x][y] == 0)
						continue;

					const WalkPosition nextTile(x, y);
					if(tileToLastMinima.count(nextTile) == 0)
					{
						tileToLastMinima[nextTile] = lastMinima;
						tileToChildren[currentTile].push_back(nextTile);

						unvisitedTiles.set(nextTile, mTileClearance[x][y]);
					}
				}
			}
		}

		std::set<WalkPosition> tileSteps;
		tileSteps.insert(currentRegionTile);
		int regionSize = 1;

		while(!tileSteps.empty())
		{
			std::set<WalkPosition>::iterator currentTile = tileSteps.begin();
			++regionSize;

			for each(WalkPosition nextTile in tileToChildren[*currentTile])
			{
				tileSteps.insert(nextTile);
			}

			mTileToRegion[currentTile->x][currentTile->y] = currentRegion;

			tileSteps.erase(currentTile);
		}

		currentRegion->setSize(regionSize);
	}

	for(std::set<Chokepoint>::iterator it = mChokepoints.begin(); it != mChokepoints.end();)
	{
		if(!(*it)->getRegions().first || !(*it)->getRegions().second || (*it)->getRegions().first == (*it)->getRegions().second)
			removeChokepoint(*(it++));
		else
			++it;
	}
}

//Functor that remembers the closest location tested to the resources
class PossibleLocationCompare
{
public:
	PossibleLocationCompare(UnitGroup *resources, TilePosition *bestLocation)
		: mBestLocation(bestLocation)
		, mResources(resources)
		, mBestRating(std::numeric_limits<unsigned int>::max())
	{}

	bool operator()(TilePosition &location)
	{
		for(int x = location.x(); x < location.x() + BWAPI::UnitTypes::Protoss_Nexus.tileWidth(); ++x)
		{
			for(int y = location.y(); y < location.y() + BWAPI::UnitTypes::Protoss_Nexus.tileHeight(); ++y)
			{
				if(x < 0 && y < 0 && x >= BWAPI::Broodwar->mapWidth() && y >= BWAPI::Broodwar->mapHeight())
					return false;

				if(!BWAPI::Broodwar->isBuildable(x, y))
					return false;
			}
		}

		unsigned int distanceToResources = 0;
		for each(const Unit &resource in *mResources)
		{
			const BWAPI::UnitType &resourceType = resource->getType();
			const TilePosition &resourceTilePosition = resource->getTilePosition();

			if (resourceTilePosition.x() > location.x() - (resourceType == BWAPI::UnitTypes::Resource_Mineral_Field ? 5 : 7) &&
				resourceTilePosition.y() > location.y() - (resourceType == BWAPI::UnitTypes::Resource_Mineral_Field ? 4 : 5) &&
				resourceTilePosition.x() < location.x() + 7 &&
				resourceTilePosition.y() < location.y() + 6)
			{
				return false;
			}

			const Position &resourcePosition = resource->getPosition();

			int tx = location.x()*32+64;
			int ty = location.y()*32+48;

			int uLeft       = resourcePosition.x() - resourceType.dimensionLeft();
			int uTop        = resourcePosition.y() - resourceType.dimensionUp();
			int uRight      = resourcePosition.x() + resourceType.dimensionRight() + 1;
			int uBottom     = resourcePosition.y() + resourceType.dimensionDown() + 1;

			int targLeft    = tx - BWAPI::Broodwar->self()->getRace().getCenter().dimensionLeft();
			int targTop     = ty - BWAPI::Broodwar->self()->getRace().getCenter().dimensionUp();
			int targRight   = tx + BWAPI::Broodwar->self()->getRace().getCenter().dimensionRight() + 1;
			int targBottom  = ty + BWAPI::Broodwar->self()->getRace().getCenter().dimensionDown() + 1;

			int xDist = uLeft - targRight;
			if(xDist < 0)
			{
				xDist = targLeft - uRight;
				if(xDist < 0)
					xDist = 0;
			}

			int yDist = uTop - targBottom;
			if(yDist < 0)
			{
				yDist = targTop - uBottom;
				if(yDist < 0)
					yDist = 0;
			}

			distanceToResources += Position(0,0).getApproxDistance(Position(xDist, yDist));
		}

		if(distanceToResources < mBestRating)
		{
			mBestRating = distanceToResources;
			*mBestLocation = location;
		}

		return false;
	}

private:
	unsigned int mBestRating;
	TilePosition *mBestLocation;
	UnitGroup *mResources;
};

void TerrainAnaysisClass::createBases()
{
	//Create a set of all Geysers and all mineral patches with more than 200 minerals
	UnitGroup resources = UnitTracker::Instance().getGeysers();
	for each(Unit mineral in UnitTracker::Instance().getMinerals())
	{
		if(mineral->getResources() > 200)
			resources.insert(mineral);
	}

	//Group them into clusters
	std::vector<UnitGroup> resourceClusters = UnitHelper::getClusters(resources, 260, 3);

	//TODO: create the bases on the fly rather then collect information then create
	std::set<TilePosition> basesToCreate;
	std::map<TilePosition, UnitGroup> baseToCreateToMineral;
	std::map<TilePosition, bool> baseIsRegionBase;
	std::map<TilePosition, bool> baseToCreateIsStartLocation;
	std::map<TilePosition, Region> baseToRegion;
	std::map<Region, std::set<TilePosition>> baseToCreateFromRegion;

	for each(UnitGroup resourceCluster in resourceClusters)
	{
		TilePosition baseLocation;
		PossibleLocationCompare locationCompare(&resourceCluster, &baseLocation);
		MapHelper::spiralSearch(TilePosition(resourceCluster.getCenter()), locationCompare, 18);

		bool addedToOther = false;
		for(std::map<TilePosition, UnitGroup>::iterator it = baseToCreateToMineral.begin(); it != baseToCreateToMineral.end(); ++it)
		{
			int dx = abs(baseLocation.x() - it->first.x());
			int dy = abs(baseLocation.y() - it->first.y());

			if(dx <= 4 && dy <= 3)
			{
				it->second.insert(resourceCluster.begin(), resourceCluster.end());
				addedToOther = true;
				break;
			}
		}

		if(!addedToOther)
		{
			basesToCreate.insert(baseLocation);
			baseToCreateToMineral[baseLocation] = resourceCluster;
		}
	}

	for each(TilePosition base in basesToCreate)
	{
		Region region = mTileToRegion[base.x() * 4][base.y() * 4];
		if(region)
		{
			baseToCreateFromRegion[region].insert(base);
			baseToRegion[base] = region;
		}
	}

	RectangleArray<TilePosition> tileToBase(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
	std::map<TilePosition, std::set<TilePosition>> baseToTiles;

	for(int x = 0; x < BWAPI::Broodwar->mapWidth(); ++x)
	{
		for(int y = 0; y < BWAPI::Broodwar->mapHeight(); ++y)
		{
			Region region = mTileToRegion[x*4][y*4];

			//This isn't a region, so find the closest Region
			if(!region || region->getClearance() < 80)
			{
				int distance = std::numeric_limits<int>::max();
				for each(Region reg in mRegions)
				{
					if(reg->getClearance() < 80)
						continue;

					int thisDistance = reg->getCenter().getApproxDistance(Position(x*32, y*32));
					if(thisDistance < distance)
					{
						distance = thisDistance;
						region = reg;
					}
				}
			}

			// Still hasn't got a region? something must be wrong with the analysis
			if(!region)
				return;
			
			// Dont already have a base for this region
			if(baseToCreateFromRegion.count(region) == 0)
			{
				TilePosition newBase = TilePosition(region->getCenter());

				basesToCreate.insert(newBase);
				baseToCreateFromRegion[region].insert(newBase);
				baseToRegion[newBase] = region;

				tileToBase[x][y] = newBase;
				baseToTiles[newBase].insert(TilePosition(x, y));
				baseIsRegionBase[newBase] = true;
			}
			// Have 1 base exactly, use it
			else if(baseToCreateFromRegion[region].size() == 1)
			{
				TilePosition base = *baseToCreateFromRegion[region].begin();
				tileToBase[x][y] = base;
				baseToTiles[base].insert(TilePosition(x, y));
			}
			// Find the closest from the possibilities
			else
			{
				TilePosition baseToUse;
				int distance = std::numeric_limits<int>::max();

				for each(TilePosition base in baseToCreateFromRegion[region])
				{
					int thisDistance = Position(base).getApproxDistance(Position(x*32, y*32));
					if(thisDistance < distance)
					{
						distance = thisDistance;
						baseToUse = base;
					}
				}

				tileToBase[x][y] = baseToUse;
				baseToTiles[baseToUse].insert(TilePosition(x, y));
			}
		}
	}

	for each(TilePosition startLocation in BWAPI::Broodwar->getStartLocations())
	{
		baseToCreateIsStartLocation[tileToBase[startLocation.x()][startLocation.y()]] = true;
	}

	for each(TilePosition base in basesToCreate)
	{
		Base newBase;
		if(baseIsRegionBase[base])
			newBase = BaseTracker::Instance().createBase(baseToTiles[base], baseToRegion[base], baseToCreateIsStartLocation[base]);
		else
			newBase = BaseTracker::Instance().createBase(baseToTiles[base], baseToRegion[base], baseToCreateIsStartLocation[base], base, baseToCreateToMineral[base]);

		newBase->getRegion()->addBase(newBase);
	}
}

void TerrainAnaysisClass::removeChokepoint(Chokepoint chokeToRemove)
{
	//Removes a chokepoint and if its a real one will merge the 2 regions

	Region reg1 = chokeToRemove->getRegions().first;
	Region reg2 = chokeToRemove->getRegions().second;

	if(reg1)
		reg1->removeChokepoint(chokeToRemove);

	if(reg2 && reg2 != reg1)
		reg2->removeChokepoint(chokeToRemove);

	// choke is real, need to merge the regions
	if(reg1 && reg2 && reg1 != reg2)
	{
		LOGMESSAGEWARNING("Tried to Merge 2 regions but is not implemented");
	}
}