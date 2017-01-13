#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "BaseTracker.h"
#include "ReservedLocation.h"
#include "TypeSafeEnum.h"

using namespace SkynetBot;

struct ExpandTypeDef
{
	enum type
	{
		expNone,
		expFurthestFromEnemy,
		expCloseToMe
	};
};
typedef SafeEnum<ExpandTypeDef> ExpandType;

class BuildingPlacerClass
{
public:
	BuildingPlacerClass();

	void reserveResourceSpace(const UnitGroup &resources, Base base);

	void calculateReservations();
	void finaliseReservations();
	void update();
	void onDestroy(Unit unit);

	ReservedLocation getLandingReservation(Unit unit);
	ReservedLocation getBuildingReservation(BuildingLocation position, BWAPI::UnitType type);

	bool isReserved(TilePosition position) const;
	bool isReserved(int x, int y) const;

	bool isResourceReserved(TilePosition position) const;
	bool isResourceReserved(int x, int y) const;

	std::vector<Base> baseToBuildAtOrder(BWAPI::UnitType type) const;
	std::vector<Base> baseToBuildAtOrder() const;

	std::pair<TilePosition, Base> buildingLocationToTile(BuildingLocation position, BWAPI::UnitType type) const;

	std::pair<TilePosition, Base> getExpandLocation(bool gas = false) const;
	std::pair<TilePosition, Base> getRefineryLocation() const;
	std::pair<TilePosition, Base> getlandingLocation(Unit unit) const;

	TilePosition getBuildLocation(Base base, BWAPI::UnitType type) const;

	bool isLocationNonBlocking(TilePosition position, BWAPI::UnitType type, bool ignoreReservations = false) const;
	bool isLocationBuildable(TilePosition position, BWAPI::UnitType type, bool ignoreReservations = false) const;
	bool isTileBlocked(TilePosition tile, BWAPI::UnitType type, bool ignoreReservations = false) const;
	bool isTileBuildable(TilePosition tile, BWAPI::UnitType type, bool ignoreReservations = false) const;
	bool isTileWalkable(TilePosition tile, bool ignoreReservations = false) const;
	bool isAddonBuildable(TilePosition position) const;

	std::set<TilePosition> getSurroundingTiles(TilePosition position, BWAPI::UnitType type, bool ignoreReservations = false) const;

	bool isAddonBuilder(int x, int y) const;

	void reserve(ReservedLocation location);
	void free(ReservedLocation location);

private:
	void rememberBestExpandLocation(Base base, Base& bestBase, int& closestDistance, int& furthestDistance, ExpandType& type, bool gas, std::set<Base> enemyBases) const;

	std::map<TilePosition, BWAPI::UnitType> mPermanentReserved;
	std::map<TilePosition, int> mResourceReserved;
	std::map<Unit, std::set<TilePosition>> mResources;

	std::set<ReservedLocation> mReservedLocations;
	std::map<TilePosition, ReservedLocation> mReservedTiles;
};

typedef Singleton<BuildingPlacerClass> BuildingPlacer;