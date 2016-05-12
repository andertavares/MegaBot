#include "BuildingPlacer.h"

#include <boost/bind.hpp>

#include "BaseTracker.h"
#include "UnitHelper.h"
#include "MapHelper.h"
#include "UnitTracker.h"
#include "TerrainAnaysis.h"
#include "DrawBuffer.h"
#include "WallTracker.h"
#include "BorderTracker.h"
#include "ScoutManager.h"
#include "PylonPowerTracker.h"

BuildingPlacerClass::BuildingPlacerClass()
{
}

void BuildingPlacerClass::calculateReservations()
{
	const BWAPI::UnitType &centerType = BWAPI::Broodwar->self()->getRace().getCenter();
	const BWAPI::UnitType &refineryType = BWAPI::Broodwar->self()->getRace().getRefinery();

	// first reserve all wall positions
	mPermanentReserved = WallTracker::Instance().getWallTiles();

	// for each base that has resources
	for each(Base base in BaseTracker::Instance().getResourceBases())
	{
		// reserve the base location
		for(int x = base->getCenterBuildLocation().x(); x < base->getCenterBuildLocation().x()+centerType.tileWidth(); ++x)
			for(int y = base->getCenterBuildLocation().y(); y < base->getCenterBuildLocation().y()+centerType.tileHeight(); ++y)
				mPermanentReserved[TilePosition(x, y)] = centerType;

		// if its zerg, also reserve the area the larva hang out
		if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg)
		{
			for(int x = base->getCenterBuildLocation().x() - 1; x < base->getCenterBuildLocation().x()+centerType.tileWidth()+1; ++x)
				for(int y = base->getCenterBuildLocation().y() - 1; y < base->getCenterBuildLocation().y()+centerType.tileHeight()+1; ++y)
					mPermanentReserved[TilePosition(x, y)] = centerType;
		}

		// and reserve the geyser space just so it doesn't get included in the resource reserved space
		for each(Unit geyser in base->getGeysers())
		{
			for(int x = geyser->getTilePosition().x(); x < geyser->getTilePosition().x()+geyser->getType().tileWidth(); ++x)
				for(int y = geyser->getTilePosition().y(); y < geyser->getTilePosition().y()+geyser->getType().tileHeight(); ++y)
					mPermanentReserved[TilePosition(x, y)] = refineryType;
		}

		reserveResourceSpace(base->getMinerals(), base);
		reserveResourceSpace(base->getGeysers(), base);
	}
}

void BuildingPlacerClass::finaliseReservations()
{
	// Reserve the tiles around the choke for general use
	// TODO: perhaps only in a path between the two sides
	for each(Chokepoint choke in TerrainAnaysis::Instance().getChokepoints())
	{
		int minX = std::min(choke->getBuildTiles().first.x(), choke->getBuildTiles().second.x());
		int maxX = std::max(choke->getBuildTiles().first.x(), choke->getBuildTiles().second.x());

		int minY = std::min(choke->getBuildTiles().first.y(), choke->getBuildTiles().second.y());
		int maxY = std::max(choke->getBuildTiles().first.y(), choke->getBuildTiles().second.y());

		for(int x = minX; x <= maxX; ++x)
			for(int y = minY; y <= maxY; ++y)
				if(mPermanentReserved.count(TilePosition(x, y)) == 0)
					mPermanentReserved[TilePosition(x, y)] = BWAPI::UnitTypes::None;
	}
}

void BuildingPlacerClass::reserveResourceSpace(const UnitGroup &resources, Base base)
{
	for each(Unit resource in resources)
	{
		int distanceBetween = (resource->getPosition().getApproxDistance(base->getCenterLocation()) / 32) + 1;
		for(int x = resource->getTilePosition().x() - int(distanceBetween); x < resource->getTilePosition().x() + int(distanceBetween) + 4; ++x)
		{
			for(int y = resource->getTilePosition().y() - int(distanceBetween); y < resource->getTilePosition().y() + int(distanceBetween) + 3; ++y)
			{
				int distanceToResource = Position(x*32+16, y*32+16).getApproxDistance(resource->getPosition()) / 32;
				int distanceToBase = Position(x*32+16, y*32+16).getApproxDistance(base->getCenterLocation()) / 32;

				if(distanceToResource + distanceToBase < distanceBetween && mPermanentReserved.count(TilePosition(x, y)) == 0)
				{
					if(resource->getType().isMineralField())
						mResources[resource].insert(TilePosition(x, y));

					++mResourceReserved[TilePosition(x, y)];
				}
			}
		}
	}
}

void BuildingPlacerClass::update()
{
	for(std::set<ReservedLocation>::iterator it = mReservedLocations.begin(); it != mReservedLocations.end();)
	{
		if((*it)->update())
			mReservedLocations.erase(it++);
		else
			++it;
	}

// 	for(std::map<TilePosition, BWAPI::UnitType>::iterator it = mPermanentReserved.begin(); it != mPermanentReserved.end(); ++it)
// 	{
// 		BWAPI::Broodwar->drawBox(BWAPI::CoordinateType::Map, it->first.x() * 32, it->first.y() * 32, it->first.x() * 32 + 32, it->first.y() * 32 + 32, BWAPI::Colors::Red);
// 	}
// 	for(std::map<TilePosition, int>::iterator it = mResourceReserved.begin(); it != mResourceReserved.end(); ++it)
// 	{
// 		BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map, it->first.x()*32+16, it->first.y()*32+16, 16, BWAPI::Colors::Blue);
// 	}
// 	for(std::map<TilePosition, ReservedLocation>::iterator it = mReservedTiles.begin(); it != mReservedTiles.end(); ++it)
// 	{
// 		BWAPI::Broodwar->drawBox(BWAPI::CoordinateType::Map, it->first.x() * 32, it->first.y() * 32, it->first.x() * 32 + 32, it->first.y() * 32 + 32, BWAPI::Colors::Red);
// 	}
}

void BuildingPlacerClass::onDestroy(Unit unit)
{
	if(mResources.count(unit) != 0)
	{
		for each(TilePosition tile in mResources[unit])
		{
			--mResourceReserved[tile];
			if(mResourceReserved[tile] == 0)
				mResourceReserved.erase(tile);
		}

		mResources.erase(unit);
	}
}

void BuildingPlacerClass::reserve(ReservedLocation location)
{
	int startx = location->getTilePosition().x();
	int starty = location->getTilePosition().y();

	int endx = startx + location->getUnitType().tileWidth();
	int endy = starty + location->getUnitType().tileHeight();

	if(UnitHelper::hasAddon(location->getUnitType()))
		endx += 2;

	for(int x = startx; x < endx; ++x)
		for(int y = starty; y < endy; ++y)
			mReservedTiles[TilePosition(x, y)] = location;
}

void BuildingPlacerClass::free(ReservedLocation location)
{
	int startx = location->getTilePosition().x();
	int starty = location->getTilePosition().y();

	int endx = startx + location->getUnitType().tileWidth();
	int endy = starty + location->getUnitType().tileHeight();

	if(UnitHelper::hasAddon(location->getUnitType()))
		endx += 2;

	for(int x = startx; x < endx; ++x)
		for(int y = starty; y < endy; ++y)
			mReservedTiles.erase(TilePosition(x, y));
}

ReservedLocation BuildingPlacerClass::getLandingReservation(Unit unit)
{
	ReservedLocation location(new ReservedLocationClass(unit));
	mReservedLocations.insert(location);
	return location;
}

ReservedLocation BuildingPlacerClass::getBuildingReservation(BuildingLocation position, BWAPI::UnitType type)
{
	ReservedLocation location(new ReservedLocationClass(type, position));
	mReservedLocations.insert(location);
	return location;
}

bool BuildingPlacerClass::isReserved(TilePosition position) const
{
	return mReservedTiles.find(position) != mReservedTiles.end();
}

bool BuildingPlacerClass::isReserved(int x, int y) const
{
	return isReserved(TilePosition(x, y));
}

bool BuildingPlacerClass::isResourceReserved(TilePosition position) const
{
	return mResourceReserved.find(position) != mResourceReserved.end();
}

bool BuildingPlacerClass::isResourceReserved(int x, int y) const
{
	return isResourceReserved(TilePosition(x, y));
}

std::pair<TilePosition, Base> BuildingPlacerClass::buildingLocationToTile(BuildingLocation position, BWAPI::UnitType type) const
{
	if(type.isRefinery())
		return getRefineryLocation();

	if(type.requiresPsi())
	{
		bool atleastone = false;
		for each(Unit pylon in UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_Pylon))
		{
			if(pylon->isCompleted())
				atleastone = true;
		}
		if(!atleastone)
			return std::make_pair(BWAPI::TilePositions::None, Base());
	}

	switch(position.underlying())
	{
	case BuildingLocation::BaseChoke:
		{
			TilePosition pos = WallTracker::Instance().getWallPosition(type);
			if(pos != BWAPI::TilePositions::None)
				return std::make_pair(pos, BaseTracker::Instance().getBase(pos));
		}
	case BuildingLocation::BaseParimeter:
	case BuildingLocation::Base:
		{
			for each(Base base in baseToBuildAtOrder(type))
			{
				TilePosition pos = getBuildLocation(base, type); // TODO : flags of some sort so certain units can be build in minerals etc

				if(pos != BWAPI::TilePositions::None)
					return std::make_pair(pos, base);
			}
			break;
		}
	case BuildingLocation::Expansion:
			return getExpandLocation();
	case BuildingLocation::ExpansionGas:
			return getExpandLocation(true);
	case BuildingLocation::Proxy:
			//TODO: find a location that isn't on the path between any base to any other base start locations closest to potential enemy locations
	case BuildingLocation::Manner:
			//TODO: build in a location that harms the enemy, engineering bay in expand location, pylon in mineral line, gas steal etc
		break;
	}

	return std::make_pair(BWAPI::TilePositions::None, Base());
}

std::pair<TilePosition, Base> BuildingPlacerClass::getlandingLocation(Unit unit) const
{
	// TODO : NYI
	return std::make_pair(BWAPI::TilePositions::None, Base());
	//return BuildingPlacer::Instance().getBuildLocation(unit->getTilePosition() + TilePosition(0, 1));
}

class BuildLocationCompare
{
public:
	BuildLocationCompare(Base base, BWAPI::UnitType type) : mBase(base), mType(type) {}

	bool operator()(TilePosition location) const
	{
		if(BaseTracker::Instance().getBase(location) != mBase)
			return false;

		return BuildingPlacer::Instance().isLocationNonBlocking(location, mType);
	}

private:
	Base mBase;
	BWAPI::UnitType mType;
};

TilePosition BuildingPlacerClass::getBuildLocation(Base base, BWAPI::UnitType type) const
{
	return MapHelper::spiralSearch(base->getCenterBuildLocation(), BuildLocationCompare(base, type));
}

class RegionChokeCompare
{
public:
	RegionChokeCompare(bool ignoreReservations, Region region)
		: mIgnoreReservations(ignoreReservations)
		, mRegion(region)
	{}

	bool operator()(TilePosition &location)
	{
		return BuildingPlacer::Instance().isTileWalkable(location, mIgnoreReservations) && TerrainAnaysis::Instance().getRegion(location) == mRegion;
	}

private:
	bool mIgnoreReservations;
	Region mRegion;
};

bool BuildingPlacerClass::isLocationNonBlocking(TilePosition position, BWAPI::UnitType type, bool ignoreReservations) const
{
	if(!isLocationBuildable(position, type, ignoreReservations))
		return false;

	std::set<TilePosition> targets = getSurroundingTiles(position, type, ignoreReservations);

	/* If there are no surrounding tiles, it's probably impossible to reach the build site */
	if(targets.empty())
		return false;

	/* Don't build on choke tiles, they are vital for the calculation */
	Region region = TerrainAnaysis::Instance().getRegion(position);
	const std::set<TilePosition> &chokeTiles = region->getChokepointTiles();
	for each(TilePosition tile in chokeTiles)
	{
		if(tile.x() >= position.x() && tile.x() < position.x() + type.tileWidth() && tile.y() >= position.y() && tile.y() < position.y() + type.tileHeight())
			return false;

		targets.insert(tile);
	}

	/* Start from one of the chokes */
	TilePosition startTile = *chokeTiles.begin();

	/* Ignore the tiles we are building on as we cant path though this building */
	std::set<TilePosition> ignoreTiles;
	for(int x = position.x(); x < position.x() + type.tileWidth(); ++x)
		for(int y = position.y(); y < position.y() + type.tileHeight(); ++y)
			ignoreTiles.insert(TilePosition(x, y));

	/* Don't block other buildings */
	for each(Unit unit in UnitTracker::Instance().getAllUnits())
	{
		if(unit->exists() && !unit->getType().canProduce())
			continue;

		/* Refineries are ok as they will be protected by resource reservation */
		if(unit->getType().isRefinery())
			continue;

		if(UnitHelper::unitProducesGround(unit->getType()))
		{
			/* 2 is no location found yet */
			int isBlocked = 2;
			for each(TilePosition tile in getSurroundingTiles(unit->getTilePosition(), unit->getType(), ignoreReservations))
			{
				if(TerrainAnaysis::Instance().getRegion(tile) == region)
				{
					/* If this location is touching this building, ensure we don't block all exits */
					if(ignoreTiles.find(tile) != ignoreTiles.end())
						isBlocked = (isBlocked==0?0:1);
					else
					{
						/* This tile is not blocked, we can touch this building here */
						targets.insert(tile);
						isBlocked = 0;
					}
				}
			}

			/* Building here will stop units being able to exit this building */
			if(isBlocked == 1)
				return false;
		}
	}

	/* Check which tiles can be reached */
	targets = MapHelper::floodFill(startTile, RegionChokeCompare(ignoreReservations, region), targets, ignoreTiles);

	/* If there are no remaining targets, there are all reachable */
	if(targets.empty())
		return true;

	/* Check if we could reach these positions before */
	std::set<TilePosition> newTargets = MapHelper::floodFill(startTile, RegionChokeCompare(ignoreReservations, region), targets);

	/* If they are the same, they were never reachable */
	if(targets == newTargets)
	{
		//if(m_bShowDebugInfo)
// 		{
// 			DrawBuffer::Instance().clearBuffer(BufferedCategory::BuildingPlacer);
// 			for each(TilePosition tile in targets)
// 				DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Map, tile.x()*32+16, tile.y()*32+16, 16, 240, BWAPI::Colors::Red, false, BufferedCategory::BuildingPlacer);
// 
// 			for(int x = position.x(); x < position.x() + type.tileWidth(); ++x)
// 				for(int y = position.y(); y < position.y() + type.tileHeight(); ++y)
// 					DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Map, x*32+16, y*32+16, 16, 240, BWAPI::Colors::White, false, BufferedCategory::BuildingPlacer);
// 		}

		return true;
	}

	//if(m_bShowDebugInfo)
// 	{
// 		DrawBuffer::Instance().clearBuffer(BufferedCategory::BuildingPlacer);
// 		for each(TilePosition tile in targets)
// 			DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Map, tile.x()*32+16, tile.y()*32+16, 16, 240, BWAPI::Colors::Red, false, BufferedCategory::BuildingPlacer);
// 
// 		for each(TilePosition tile in newTargets)
// 			DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Map, tile.x()*32+16, tile.y()*32+16, 16, 240, BWAPI::Colors::Yellow, false, BufferedCategory::BuildingPlacer);
// 
// 		for(int x = position.x(); x < position.x() + type.tileWidth(); ++x)
// 			for(int y = position.y(); y < position.y() + type.tileHeight(); ++y)
// 				DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Map, x*32+16, y*32+16, 16, 240, BWAPI::Colors::White, false, BufferedCategory::BuildingPlacer);
// 
// 		DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Map, startTile.x()*32+16, startTile.y()*32+16, 16, 240, BWAPI::Colors::Green, false, BufferedCategory::BuildingPlacer);
// 	}

	return false;
}

bool BuildingPlacerClass::isLocationBuildable(TilePosition position, BWAPI::UnitType type, bool ignoreReservations) const
{
	if(type.isRefinery())
	{
		for each(Unit geyser in UnitTracker::Instance().getGeysers())
		{
			if(geyser->getTilePosition() == position)
			{
				if(!ignoreReservations)
				{
					for(int x = position.x(); x < position.x() + type.tileWidth(); ++x)
					{
						for(int y = position.y(); y < position.y() + type.tileHeight(); ++y)
						{
							if(isReserved(x, y))
								return false;
						}
					}
				}

				return true;
			}
		}

		return false;
	}

	for(int x = position.x(); x < position.x() + type.tileWidth(); ++x)
	{
		for(int y = position.y(); y < position.y() + type.tileHeight(); ++y)
		{
			if(!isTileBuildable(TilePosition(x, y), type, ignoreReservations))
				return false;
		}
	}

	if(type.isResourceDepot())
	{
		for each(Unit mineral in UnitTracker::Instance().getMinerals())
		{
			if(mineral->accessibility() != AccessType::Dead)
			{
				if (mineral->getTilePosition().x() > position.x() - 5 &&
					mineral->getTilePosition().y() > position.y() - 4 &&
					mineral->getTilePosition().x() < position.x() + 7 &&
					mineral->getTilePosition().y() < position.y() + 6)
				{
					return false;
				}
			}
		}
		for each(Unit geyser in UnitTracker::Instance().getGeysers())
		{
			if(geyser->accessibility() != AccessType::Dead)
			{
				if (geyser->getTilePosition().x() > position.x() - 7 &&
					geyser->getTilePosition().y() > position.y() - 5 &&
					geyser->getTilePosition().x() < position.x() + 7 &&
					geyser->getTilePosition().y() < position.y() + 6)
				{
					return false;
				}
			}
		}
	}

	if(type.requiresPsi())
	{
		if(!PylonPowerTracker::Instance().hasPower(position, type))
			return false;
	}

	/* If this has an addon, make sure there is space to build it */
	if(UnitHelper::hasAddon(type) && !isAddonBuildable(position))
		return false;

	/* Put some extra space between the first 6 pylons*/
	/*if(type == BWAPI::UnitTypes::Protoss_Pylon)
	{
		UnitGroup ug = SelectAll(BWAPI::UnitTypes::Protoss_Pylon);
		if(ug.size() < 6)
		{
			ug = ug.inRadius(8 * 32, position);

			if(ug.size() != 0)
				return false;
		}
	}*/

	return true;
}

bool BuildingPlacerClass::isTileBlocked(TilePosition tile, BWAPI::UnitType type, bool ignoreReservations) const
{
	if(!ignoreReservations)
	{
		if(isReserved(tile) || (!UnitHelper::isStaticDefense(type) && isResourceReserved(tile)))
			return true;

		std::map<TilePosition, BWAPI::UnitType>::const_iterator it = mPermanentReserved.find(tile);
		if(it != mPermanentReserved.end())
			if(it->second != type)
				return true;
	}

	for each(Unit unit in UnitTracker::Instance().getUnitsOnTile(tile.x(), tile.y()))
	{
		if(!unit->exists())
			continue;

		if(unit->getType().isBuilding() && !unit->isLifted())
			return true;

		if(!type.isRefinery() && unit->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser)
			return true;

		if(!unit->getType().canMove())
			return true;
	}

	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran && !type.isAddon())
	{
		if(tile.x()-1 >= 0)
			if(isAddonBuilder(tile.x()-1, tile.y()))
				return true;
		if(tile.x()-2 >= 0)
			if(isAddonBuilder(tile.x()-2, tile.y()))
				return true;
	}

	return false;
}

bool BuildingPlacerClass::isTileBuildable(TilePosition tile, BWAPI::UnitType type, bool ignoreReservations) const
{
	if(!BWAPI::Broodwar->isBuildable(tile))
		return false;

	if(type.getRace() == BWAPI::Races::Zerg)
	{
		if(!type.isResourceDepot() && !BWAPI::Broodwar->hasCreep(tile))
			return false;
	}
	else if(BWAPI::Broodwar->hasCreep(tile))
		return false;

	return !isTileBlocked(tile, type, ignoreReservations);
}

bool BuildingPlacerClass::isTileWalkable(TilePosition tile, bool ignoreReservations) const
{
	if(!MapHelper::isTileWalkable(tile))
		return false;

	return !isTileBlocked(tile, BWAPI::UnitTypes::None, ignoreReservations);
}

std::pair<TilePosition, Base> BuildingPlacerClass::getRefineryLocation() const
{
	for each(Base base in baseToBuildAtOrder())
	{
		for each(Unit geyser in base->getGeysers())
		{
			if(!isReserved(geyser->getTilePosition()))
				return std::make_pair(geyser->getTilePosition(), base);
		}
	}

	return std::make_pair(BWAPI::TilePositions::None, Base());
}

bool baseCompare(const Base &i, const Base &j)
{
	if(BWAPI::Broodwar->getFrameCount() < 12000)
	{
		if(i->isStartLocation())
		{
			if(!j->isStartLocation())
				return true;
		}
		else if(j->isStartLocation())
			return false;
	}

	return (i->getNumberOfTechBuildings() > j->getNumberOfTechBuildings());
}

bool baseComparePylon(const Base &i, const Base &j)
{
	// Prioritise a base without a depot the lowest
	if(i->getResourceDepot())
	{
		if(!j->getResourceDepot())
			return true;
	}
	else if(!j->getResourceDepot())
		return false;

	// Next prioritise the base that doesn't have a pylon
	int iCount = 0;
	for each(Unit building in i->getBuildings())
	{
		if(building->getType() == BWAPI::UnitTypes::Protoss_Pylon && building->getPlayer() == BWAPI::Broodwar->self())
			++iCount;
	}

	int jCount = 0;
	for each(Unit building in j->getBuildings())
	{
		if(building->getType() == BWAPI::UnitTypes::Protoss_Pylon && building->getPlayer() == BWAPI::Broodwar->self())
			++jCount;
	}

	if(iCount == 0)
	{
		if(jCount != 0)
			return true;
	}
	else if(jCount == 0)
		return false;

	// Finally prioritise using the default sort
	return baseCompare(i, j);
}

bool baseCompareDefense(const Base &i, const Base &j, BWAPI::UnitType type)
{
	int iCount = 0;
	for each(Unit building in i->getBuildings())
	{
		if(building->getType() == type && building->getPlayer() == BWAPI::Broodwar->self())
			++iCount;
	}

	int jCount = 0;
	for each(Unit building in j->getBuildings())
	{
		if(building->getType() == type && building->getPlayer() == BWAPI::Broodwar->self())
			++jCount;
	}

	return jCount > iCount;
}

std::vector<Base> BuildingPlacerClass::baseToBuildAtOrder(BWAPI::UnitType type) const
{
	std::vector<Base> basesOrder = baseToBuildAtOrder();

	if(type == BWAPI::UnitTypes::Protoss_Pylon)
	{
		std::sort(basesOrder.begin(), basesOrder.end(), baseComparePylon);
	}
	else if(type == BWAPI::UnitTypes::Protoss_Photon_Cannon || type == BWAPI::UnitTypes::Terran_Missile_Turret)
	{
		std::vector<Base> newBasesOrder;
		for each(Base base in basesOrder)
		{
			if(base->getMinerals().empty())
				continue;

			bool hasPylon = type.requiresPsi() ? false : true;

			if(!hasPylon)
			{
				std::set<Unit> buildings = base->getBuildings();
				for each(Unit building in buildings)
				{
					if(building->getType() == BWAPI::UnitTypes::Protoss_Pylon && building->getPlayer() == BWAPI::Broodwar->self() && building->isCompleted())
						hasPylon = true;
				}
			}

			if(hasPylon)
				newBasesOrder.push_back(base);
		}

		std::sort(newBasesOrder.begin(), newBasesOrder.end(), boost::bind<bool>(&baseCompareDefense, _1, _2, type));

		return newBasesOrder;
	}

	return basesOrder;
}

std::vector<Base> BuildingPlacerClass::baseToBuildAtOrder() const
{
	std::vector<Base> returnValue;

	for each(Base base in BaseTracker::Instance().getPlayerBases())
	{
		if(!base->isUnderAttack())
			returnValue.push_back(base);
	}

	std::sort(returnValue.begin(), returnValue.end(), baseCompare);

	return returnValue;
}
bool BuildingPlacerClass::isAddonBuilder(int x, int y) const
{
	std::map<TilePosition, BWAPI::UnitType>::const_iterator it = mPermanentReserved.find(TilePosition(x, y));
	if(it != mPermanentReserved.end())
	{
		if(UnitHelper::hasAddon(it->second))
			return true;
	}

	for each(Unit unit in UnitTracker::Instance().getUnitsOnTile(x, y))
	{
		if(!unit->isLifted() && UnitHelper::hasAddon(unit->getType()))
			return true;
	}

	return false;
}

bool BuildingPlacerClass::isAddonBuildable(TilePosition position) const
{
	for(int x = position.x() + 4; x < position.x() + 6; ++x)
		for(int y = position.y() + 1; y < position.y() + 3; ++y)
			if(!isTileBuildable(TilePosition(x, y), BWAPI::UnitTypes::Terran_Machine_Shop))
				return false;

	return true;
}

std::pair<TilePosition, Base> BuildingPlacerClass::getExpandLocation(bool gas) const
{
	std::set<Base> enemyBases = BaseTracker::Instance().getEnemyBases();

	Base basePos;
	int closestDistance = std::numeric_limits<int>::max();
	int furthestDistance = std::numeric_limits<int>::min();
	ExpandType type = ExpandType::expNone;

	std::set<BorderPosition> myRegions = BorderTracker::Instance().getBorderPositions(PositionType::BotControlRegion);
	std::set<BorderPosition> enemyRegions = BorderTracker::Instance().getBorderPositions(PositionType::EnemyControlRegion);

	for(std::set<BorderPosition>::const_iterator it = myRegions.begin(); it != myRegions.end(); ++it)
	{
		for each(Base base in it->mRegion->getBases())
		{
			rememberBestExpandLocation(base, basePos, closestDistance, furthestDistance, type, gas, enemyBases);
		}
	}

	if(type == ExpandType::expNone)
	{
		for each(Base base in BaseTracker::Instance().getAllBases())
		{
			if(enemyRegions.count(BorderPosition(PositionType::EnemyControlRegion, base->getRegion())) != 0)
				continue;

			rememberBestExpandLocation(base, basePos, closestDistance, furthestDistance, type, gas, enemyBases);
		}
	}

	if(type == ExpandType::expNone)
	{
		for each(Base base in BaseTracker::Instance().getAllBases())
		{
			rememberBestExpandLocation(base, basePos, closestDistance, furthestDistance, type, gas, enemyBases);
		}
	}

	if(type != ExpandType::expNone)
		return std::make_pair(basePos->getCenterBuildLocation(), basePos);
	else
		return std::make_pair(BWAPI::TilePositions::None, Base());
}

std::set<TilePosition> BuildingPlacerClass::getSurroundingTiles(TilePosition position, BWAPI::UnitType type, bool ignoreReservations) const
{
	std::set<TilePosition> tiles;
	int y1 = position.y() + type.tileHeight();
	int x1 = position.x() - 1;
	int y2 = position.y() + type.tileHeight() - 1;
	int x2 = position.x() - 1;

	while(true)
	{
		TilePosition t1(x1, y1);
		if(isTileWalkable(t1, ignoreReservations))
			tiles.insert(t1);

		TilePosition t2(x2, y2);
		if(isTileWalkable(t2, ignoreReservations))
			tiles.insert(t2);

		if(x1 < position.x() + type.tileWidth())
			x1++;
		else
			y1--;

		if(y1 == y2 && x1 == x2)
			break;

		if(y2 > position.y() - 1)
			y2--;
		else
			x2++;
	}

	return tiles;
}

void BuildingPlacerClass::rememberBestExpandLocation(Base base, Base& bestBase, int& closestDistance, int& furthestDistance, ExpandType& type, bool gas, std::set<Base> enemyBases) const
{
	if(base->getPlayer() == NULL || (!base->isActive() && base->getPlayer() == BWAPI::Broodwar->self()))
	{
		if(base->getCenterBuildLocation() == BWAPI::TilePositions::None)
			return;

		if(gas && base->getGeysers().empty() && base->getRefineries().empty())
			return;

		if(base->isMinedOut())
			return;

		if(base->isMyBase() && base->isUnderAttack())
			return;

		ScoutType lastScoutType = ScoutManager::Instance().getLastScoutType(base);
		int lastScoutTime = ScoutManager::Instance().getLastScoutTime(base);
		if(BWAPI::Broodwar->getFrameCount() > 6200 && (lastScoutType == ScoutType::FailedWithGroundLow || lastScoutType == ScoutType::FailedWithGroundHigh || lastScoutType == ScoutType::FailedWithAir))
			return;

		//TODO: when we have transport code, remove this if we have a transport
		bool isConnected = false;
		for each(Unit unit in UnitTracker::Instance().selectAllUnits(BWAPI::Broodwar->self()->getRace().getWorker()))
		{
			if(unit->hasPath(Position(base->getCenterBuildLocation())))
			{
				isConnected = true;
				break;
			}
		}

		if(!isConnected)
			return;

		if(isLocationBuildable(base->getCenterBuildLocation(), BWAPI::Broodwar->self()->getRace().getCenter()))
		{
			for each(Base myBase in BaseTracker::Instance().getPlayerBases())
			{
				if(myBase->getCenterBuildLocation() == BWAPI::TilePositions::None)
					continue;

				int tDistance = MapHelper::getGroundDistance(base->getCenterLocation(), myBase->getCenterLocation());
				if(tDistance >= 1450 && type != ExpandType::expCloseToMe && !enemyBases.empty())
				{
					for each(Base enemyBase in enemyBases)
					{
						int enemyDistance = MapHelper::getGroundDistance(base->getCenterLocation(), enemyBase->getCenterLocation());
						if(enemyDistance > furthestDistance)
						{
							furthestDistance = enemyDistance;
							bestBase = base;
							type = ExpandType::expFurthestFromEnemy;
						}
					}
				}
				else if(tDistance < closestDistance)
				{
					closestDistance = tDistance;
					bestBase = base;
					type = ExpandType::expCloseToMe;
				}
			}
		}
	}
}