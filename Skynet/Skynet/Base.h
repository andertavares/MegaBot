#pragma once

#include "Interface.h"
#include <deque>

#include "UnitGroup.h"
#include "Region.h"

using namespace SkynetBot;

struct UnitThreatTypeDef
{
	enum type
	{
		Scout,
		AirToAir,
		AirToGround,
		GroundToAir,
		GroundToGround
	};
};
typedef SafeEnum<UnitThreatTypeDef> UnitThreatType;

class BaseClass
{
public:
	BaseClass(Region region, std::set<TilePosition> tiles, bool startLocation);
	BaseClass(TilePosition position, const UnitGroup &resources, Region region, std::set<TilePosition> tiles, bool startLocation);

	void update();

	TilePosition getCenterBuildLocation() const { return mCenterTilePosition; }
	Position getCenterLocation() const { return mCenterPosition; }

	const UnitGroup &getMinerals() const { return mMinerals; }
	const UnitGroup &getGeysers() const { return mGeysers; }
	const UnitGroup &getRefineries() const { return mRefineries; }

	Unit getResourceDepot() const { return mResourceDepots.empty() ? StaticUnits::nullunit : mResourceDepots[0]; }
	bool isActive(bool activeInFuture = false) const { return (mActive || (activeInFuture && mActiveInFuture)); }

	bool isMinedOut() const { return mMinedOut; }
	bool isUnderAttack() const { return mIsUnderAttack; }
	bool isContested() const { return mIsContested; }

	bool isStartLocation() const { return mIsStartLocation; }

	const UnitGroup &getBuildings() const { return mBuildings; }
	unsigned int getNumberOfTechBuildings() const { return mTechBuildings; }

	Player getPlayer() const { return mPlayer; }

	void onDiscover(Unit unit);
	void onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType);
	void onDestroy(Unit unit);

	void drawDebugInfo() const;

	Region getRegion() const { return mRegion.lock(); }

	const std::set<TilePosition> &getTiles() const { return mTiles; }

	bool isEnemyBase() const { return BWAPI::Broodwar->self()->isEnemy(mPlayer); }
	bool isMyBase() const { return mPlayer == BWAPI::Broodwar->self(); }
	bool isAllyBase() const { return BWAPI::Broodwar->self()->isAlly(mPlayer); }

	Unit getClosestEnemyBuilding(Position pos);

	const UnitGroup &getEnemyThreats() const { return mAllThreats; }
	const UnitGroup getDefenders() const { return mAllDefenders; }

	bool depotCompare(const Unit &depotOne, const Unit &depotTwo);

	unsigned int getActivateTime() const { return mActivateTime; }

private:
	Position mCenterPosition;
	TilePosition mCenterTilePosition;

	UnitGroup mMinerals;
	UnitGroup mGeysers;
	UnitGroup mRefineries;

	bool mActive;
	bool mActiveInFuture;
	unsigned int mActivateTime;

	bool mMinedOut;
	bool mIsUnderAttack;
	bool mIsContested;
	unsigned int mTechBuildings;

	bool mIsStartLocation;

	Player mPlayer;

	UnitGroup mBuildings;

	std::deque<Unit> mResourceDepots;

	WeakRegion mRegion;
	std::set<TilePosition> mTiles;

	std::map<Player, int> mPlayerBuildingNumbers;
	UnitGroup mAllThreats;
	std::map<UnitThreatType, UnitGroup> mThreatTypes;

	UnitGroup mAllDefenders;

	UnitGroup mLiftedBuildings;

	void updatePlayer();

	void addUnit(Unit unit);
	void removeUnit(Unit unit, Player playerToRemove, BWAPI::UnitType typeToRemove);
};

typedef std::tr1::shared_ptr<BaseClass> Base;