#include "MapHelper.h"

#include <regex>

#include "UnitTracker.h"
#include "Heap.h"
#include "BaseTracker.h"
#include "PathFinder.h"

TilePosition MapHelper::spiralSearch(TilePosition startLocation, std::tr1::function<bool (TilePosition)> testFunction, int maxRadius)
{
	int x = startLocation.x();
	int y = startLocation.y();

	int length = 1;
	int j      = 0;
	bool first = true;
	int dx     = 0;
	int dy     = 1;

	if(maxRadius == 0)
		maxRadius = std::max(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());

	while(length < maxRadius)
	{
		if(x >= 0 && x < BWAPI::Broodwar->mapWidth() && y >= 0 && y < BWAPI::Broodwar->mapHeight())
		{
			TilePosition location(x, y);
			if(testFunction(location))
				return location;
		}

		x = x + dx;
		y = y + dy;

		//count how many steps we take in this direction
		++j;
		if(j == length) //if we've reached the end, its time to turn
		{
			//reset step counter
			j = 0;

			//Spiral out. Keep going.
			if(!first)
				++length; //increment step counter if needed

			//first=true for every other turn so we spiral out at the right rate
			first = !first;

			//turn counter clockwise 90 degrees:
			if (dx == 0)
			{
				dx = dy;
				dy = 0;
			}
			else
			{
				dy = -dx;
				dx = 0;
			}
		}
		//Spiral out. Keep going.
	}

	return BWAPI::TilePositions::None;
}

std::set<TilePosition> MapHelper::floodFill(TilePosition start, std::tr1::function<bool (TilePosition)> testFunction, std::set<TilePosition> targets, std::set<TilePosition> ignoreTiles)
{
	bool noTargets = targets.empty();

	std::set<TilePosition> remainingTiles;
	remainingTiles.insert(start);

	while(!remainingTiles.empty())
	{
		if(!noTargets && targets.empty())
			return targets;

		TilePosition tile = *remainingTiles.begin();

		ignoreTiles.insert(tile);
		remainingTiles.erase(tile);

		if(!tile.isValid())
			continue;

		if(!testFunction(tile))
			continue;

		if(noTargets)
			targets.insert(tile);
		else
			targets.erase(tile);

		if(ignoreTiles.count(TilePosition(tile.x() + 1, tile.y())) == 0)
			remainingTiles.insert(TilePosition(tile.x() + 1, tile.y()));
		if(ignoreTiles.count(TilePosition(tile.x() - 1, tile.y())) == 0)
			remainingTiles.insert(TilePosition(tile.x() - 1, tile.y()));
		if(ignoreTiles.count(TilePosition(tile.x(), tile.y() + 1)) == 0)
			remainingTiles.insert(TilePosition(tile.x(), tile.y() + 1));
		if(ignoreTiles.count(TilePosition(tile.x(), tile.y() - 1)) == 0)
			remainingTiles.insert(TilePosition(tile.x(), tile.y() - 1));
	}

	return targets;
}

std::map<WalkPosition, int> MapHelper::walkSearch(WalkPosition start, std::tr1::function<bool (WalkPosition)> testFunction, std::set<WalkPosition> targets, std::set<WalkPosition> ignoreTiles)
{
	std::map<WalkPosition, int> returnDistances;

	int mapWidth = BWAPI::Broodwar->mapWidth() * 4;
	int mapHeight = BWAPI::Broodwar->mapHeight() * 4;

	Heap<WalkPosition, int> openTiles(true);
	std::map<WalkPosition, int> gmap;
	std::set<WalkPosition> closedTiles;

	openTiles.push(std::make_pair(start, 0));
	gmap[start] = 0;

	int maxhvalue = std::max(mapHeight, mapWidth);

	while(!openTiles.empty())
	{
		WalkPosition p = openTiles.top().first;

		int fvalue = openTiles.top().second;
		int gvalue = gmap[p];

		openTiles.pop();
		closedTiles.insert(p);
		returnDistances[p] = gvalue;

		for(int i = 0; i < 4; ++i)
		{
			int x = i == 0 ? 1 : i == 1 ? -1 : 0;
			int y = i == 2 ? 1 : i == 3 ? -1 : 0;
			WalkPosition tile(p.x + x, p.y + y);

			if(tile.x < 0 || tile.y < 0 || tile.x >= mapWidth || tile.y >= mapHeight)
				continue;

			if(closedTiles.find(tile) != closedTiles.end())
				continue;

			if(!testFunction(tile))
				continue;

			int g = gvalue + 1;

			int f = g;
			if(gmap.find(tile) == gmap.end() || gmap[tile] > g)
			{
				gmap[tile] = g;
				openTiles.set(tile, f);
			}
		}
	}

	return returnDistances;
}

bool MapHelper::isAnyVisible(TilePosition location, BWAPI::UnitType type)
{
	for(int x = location.x(); x < location.x() + type.tileWidth(); ++x)
	{
		for(int y = location.y(); y < location.y() + type.tileHeight(); ++y)
		{
			if(BWAPI::Broodwar->isVisible(x, y))
				return true;
		}
	}

	return false;
}

bool MapHelper::isAllVisible(TilePosition location, BWAPI::UnitType type)
{
	for(int x = location.x(); x < location.x() + type.tileWidth(); ++x)
	{
		for(int y = location.y(); y < location.y() + type.tileHeight(); ++y)
		{
			if(!BWAPI::Broodwar->isVisible(x, y))
				return false;
		}
	}

	return true;
}

bool MapHelper::isTileWalkable(TilePosition location)
{
	return isTileWalkable(location.x(), location.y());
}

bool MapHelper::isTileWalkable(int x, int y)
{
	for(int nx = x * 4; nx < x * 4 + 4; ++nx)
	{
		for(int ny = y * 4; ny < y * 4 + 4; ++ny)
		{
			if(!BWAPI::Broodwar->isWalkable(nx, ny))
				return false;
		}
	}

	return true;
}

bool MapHelper::mapIs(std::string name)
{
	std::string mapName = BWAPI::Broodwar->mapName();
	std::transform(mapName.begin(), mapName.end(), mapName.begin(), tolower);

	std::string mapFileName = BWAPI::Broodwar->mapFileName();
	std::transform(mapFileName.begin(), mapFileName.end(), mapFileName.begin(), tolower);

	std::transform(name.begin(), name.end(), name.begin(), tolower);

	return std::tr1::regex_search(mapName.begin(), mapName.end(), std::tr1::regex(name)) || std::tr1::regex_search(mapFileName.begin(), mapFileName.end(), std::tr1::regex(name));
}

int MapHelper::getGroundDistance(Position start, Position end)
{
	if(!BWAPI::Broodwar->hasPath(start, end))
		return std::numeric_limits<int>::max();

	PositionPath path = PathFinder::Instance().CreateCheapWalkPath(start, end);
	if(!path.isComplete)
		return std::numeric_limits<int>::max();

	int groundDistance = path.getLength();
	int linearDistance = start.getApproxDistance(end);

	if(groundDistance < linearDistance)
		return linearDistance;

	return groundDistance;
}