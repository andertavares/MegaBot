#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Base.h"

using namespace SkynetBot;

class BaseTrackerClass
{
public:
	BaseTrackerClass();

	Base createBase(std::set<TilePosition> tiles, Region region, bool startLocation, TilePosition position = BWAPI::TilePositions::None, const UnitGroup &resources = UnitGroup());

	void update();
	void onDiscover(Unit unit);
	void onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType);
	void onDestroy(Unit unit);

	const std::set<Base> &getAllBases() const { return mBases; };
	std::set<Base> getResourceBases();
	std::set<Base> getActiveBases(bool activeInFuture = false, Player player = BWAPI::Broodwar->self());
	std::set<Base> getEnemyActiveBases(bool activeInFuture = false);
	std::set<Base> getEnemyBases();
	std::set<Base> getAllyBases();
	std::set<Base> getPlayerBases(Player player = BWAPI::Broodwar->self());
	std::set<Base> getImportantBases(Player player = BWAPI::Broodwar->self());
	std::set<Base> getEnemyImportantBases();

	Base getBase(TilePosition location) { return mTileToBase[location]; }

private:
	std::set<Base> mBases;
	std::map<TilePosition, Base> mTileToBase;
};

typedef Singleton<BaseTrackerClass> BaseTracker;