#include "PathFinder.h"
#include "Heap.h"
#include "BuildingPlacer.h"
#include "TerrainAnaysis.h"
#include "DrawBuffer.h"
#include "Logger.h"

BuildTilePath PathFinderClass::CreateTilePath(TilePosition start, TilePosition target, std::tr1::function<bool (TilePosition)> tileTest, std::tr1::function<int (TilePosition, TilePosition, int)> gFunction, std::tr1::function<int (TilePosition, TilePosition)> hFunction, int maxGValue, bool diaganol)
{
	BuildTilePath path;

	Heap<TilePosition, int> openTiles(true);
	std::map<TilePosition, int> gmap;
	std::map<TilePosition, TilePosition> parent;
	std::set<TilePosition> closedTiles;

	openTiles.push(std::make_pair(start, 0));
	gmap[start] = 0;
	parent[start] = start;

	while(!openTiles.empty())
	{
		TilePosition p = openTiles.top().first;
		int gvalue = gmap[p];

		if(p == target || (maxGValue != 0 && gvalue >= maxGValue))
			break;

		int fvalue = openTiles.top().second;
		
		openTiles.pop();
		closedTiles.insert(p);

		int minx = std::max(p.x() - 1, 0);
		int maxx = std::min(p.x() + 1, BWAPI::Broodwar->mapWidth());
		int miny = std::max(p.y() - 1, 0);
		int maxy = std::min(p.y() + 1, BWAPI::Broodwar->mapHeight());

		for(int x = minx; x <= maxx; x++)
		{
			for(int y = miny; y <= maxy; y++)
			{
				if (x != p.x() && y != p.y() && !diaganol)
					continue;

				TilePosition t(x, y);

				if (closedTiles.find(t) != closedTiles.end())
					continue;

				if(!tileTest(t))
					continue;

				int g = gFunction(t, p, gvalue);

				int f = g + hFunction(t, target);
				if (gmap.find(t) == gmap.end() || g < gmap.find(t)->second)
				{
					gmap[t] = g;
					openTiles.set(t, f);
					parent[t] = p;
				}
			}
		}
	}

	if(!openTiles.empty())
	{
		TilePosition p = openTiles.top().first;
		while(p != parent[p])
		{
			path.addNode(p);
			p = parent[p];
		}
		path.addNode(start);
		path.isComplete = true;
	}

	return path;
}

class FleeGValue
{
public:
	int operator()(TilePosition currentTile, TilePosition previousTile, int gTotal)
	{
		return gTotal + 1;
	}
};

class FleeHValue
{
public:
	FleeHValue(const UnitGroup &enemies)
		: mEnemies(enemies)
	{
		mMmaxHValue = std::max(BWAPI::Broodwar->mapHeight(), BWAPI::Broodwar->mapWidth());
	}

	int operator()(TilePosition position, TilePosition target)
	{
		int h = 0;

		for each(Unit enemy in mEnemies)
		{
			h += mMmaxHValue - (int)position.getDistance(enemy->getTilePosition());
		}

		h /= mEnemies.size();

		return h;
	}

	const UnitGroup &mEnemies;
	int mMmaxHValue;
};

class FleeTileTest
{
public:
	FleeTileTest(bool stickToRegion, TilePosition start)
		: mStickToRegion(stickToRegion)
		, mStart(start)
	{}

	bool operator()(TilePosition position)
	{
		if(!BuildingPlacer::Instance().isTileWalkable(position))
			return false;

		return !mStickToRegion || TerrainAnaysis::Instance().getRegion(position) == TerrainAnaysis::Instance().getRegion(mStart);
	}

	bool mStickToRegion;
	TilePosition mStart;
};

BuildTilePath PathFinderClass::CreateAdvancedFleePath(TilePosition start, const UnitGroup &enemies, bool stickToRegion)
{
	return CreateTilePath(start, BWAPI::TilePositions::None, FleeTileTest(stickToRegion, start), FleeGValue(), FleeHValue(enemies), 20, false);
}

RegionPath PathFinderClass::CreateRegionPath(Region start, Region target)
{
	RegionPath path;

	Heap<Region, int> openRegions(true);
	std::map<Region, int> gmap;
	std::map<Region, Region> parent;
	std::set<Region> closedRegions;

	openRegions.push(std::make_pair(start, 0));
	gmap[start] = 0;
	parent[start] = start;

	while(!openRegions.empty())
	{
		Region region = openRegions.top().first;

		if(region == target)
		{
			while(region != parent[region])
			{
				path.addNode(region);
				region = parent[region];
			}
			path.addNode(start);
			path.isComplete = true;
			return path;
		}

		int gvalue = gmap[region];
		int fvalue = openRegions.top().second;

		openRegions.pop();
		closedRegions.insert(region);

		for each(Chokepoint choke in region->getChokepoints())
		{
			if(!choke)
				BWAPI::Broodwar->printf("Chokepoint choke in PathFinderClass::CreateRegionPath null");
			if(!choke->getRegions().first)
				BWAPI::Broodwar->printf("choke->getRegions().first in PathFinderClass::CreateRegionPath null");
			if(!choke->getRegions().second)
				BWAPI::Broodwar->printf("choke->getRegions().second in PathFinderClass::CreateRegionPath null");

			Region other = choke->getRegions().first;
			if(other == region)
				other = choke->getRegions().second;

			int g = gvalue + region->getCenter().getApproxDistance(other->getCenter());
			int h = other->getCenter().getApproxDistance(target->getCenter());

			int f = g + h;
			if(gmap.count(other) == 0 || g < gmap.find(other)->second)
			{
				gmap[other] = g;
				openRegions.set(other, f);
				parent[other] = region;
			}
		}
	}

	return path;
}

PositionPath PathFinderClass::CreateCheapWalkPath(Position start, Position target)
{
	if(!BWAPI::Broodwar->hasPath(start, target))
		return PositionPath();

	Region startRegion = TerrainAnaysis::Instance().getRegion(TilePosition(start));//TODO: explore why there may be a null region returned from this
	Region endRegion = TerrainAnaysis::Instance().getRegion(TilePosition(target));

	if(!startRegion || !endRegion)
		return PositionPath();

	PositionPath path;

	if(startRegion == endRegion)
	{
		path.addNode(target);
		path.addNode(start);
		path.isComplete = true;

		return path;
	}

	RegionPath regionPath = CreateRegionPath(startRegion, endRegion);

	if(!regionPath.isComplete)
		return path;

	path.path.push_back(start);

	Region previousRegion;
	for each(Region region in regionPath.path)
	{
		if(previousRegion)
		{
			for each(Chokepoint choke in region->getChokepoints())
			{
				if(choke->getRegions().first == previousRegion || choke->getRegions().second == previousRegion)
				{
					path.path.push_back(choke->getCenter());
					break;
				}
			}
		}

		previousRegion = region;
	}

	path.path.push_back(target);
	path.isComplete = true;

	return path;
}

WalkPositionPath PathFinderClass::CreateWalkPath(WalkPosition start, WalkPosition target, std::tr1::function<bool (WalkPosition)> tileTest, std::tr1::function<int (WalkPosition, WalkPosition, int)> gFunction, std::tr1::function<int (WalkPosition, WalkPosition)> hFunction, int maxGValue, bool diaganol)
{
	WalkPositionPath path;

	Heap<WalkPosition, int> openTiles(false);
	std::map<WalkPosition, int> gmap;
	std::map<WalkPosition, WalkPosition> parent;
	std::set<WalkPosition> closedTiles;

	int mapWidth = BWAPI::Broodwar->mapWidth() * 4;
	int mapHeight = BWAPI::Broodwar->mapHeight() * 4;

	openTiles.push(std::make_pair(start, 0));
	gmap[start] = 0;
	parent[start] = start;

	while(!openTiles.empty())
	{
		WalkPosition p = openTiles.top().first;
		int gvalue = gmap[p];

		if(p == target || (maxGValue != 0 && gvalue >= maxGValue))
			break;

		int fvalue = openTiles.top().second;

		openTiles.pop();
		closedTiles.insert(p);

		int minx = std::max(p.x - 1, 0);
		int maxx = std::min(p.x + 1, mapWidth);
		int miny = std::max(p.y - 1, 0);
		int maxy = std::min(p.y + 1, mapHeight);

		for(int x = minx; x <= maxx; x++)
		{
			for(int y = miny; y <= maxy; y++)
			{
				if (x != p.x && y != p.y && !diaganol)
					continue;

				WalkPosition t(x, y);

				if(closedTiles.find(t) != closedTiles.end())
					continue;

				if(!tileTest(t))
					continue;

				int g = gFunction(t, p, gvalue);

				int f = g + hFunction(t, target);
				if (gmap.find(t) == gmap.end() || g < gmap.find(t)->second)
				{
					gmap[t] = g;
					openTiles.set(t, f);
					parent[t] = p;
				}
			}
		}
	}

	if(!openTiles.empty())
	{
		WalkPosition p = openTiles.top().first;
		while(p != parent[p])
		{
			path.addNode(p);
			p = parent[p];
		}
		path.addNode(start);
		path.isComplete = true;
	}

	return path;
}