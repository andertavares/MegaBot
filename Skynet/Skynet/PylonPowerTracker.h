#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Task.h"

using namespace SkynetBot;

class PylonPowerTrackerClass
{
public:
	PylonPowerTrackerClass(){}

	void update();

	void onDiscover(Unit unit);
	void onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType);
	void onDestroy(Unit unit);

	bool hasPower(TilePosition tile, BWAPI::UnitType unit);

private:
	UnitGroup mPylons;

	UnitGroup mLiftedBuildings;

	std::map<Unit, std::set<TilePosition>> mPylonSmallPowerSites;
	std::map<Unit, std::set<TilePosition>> mPylonMediumPowerSites;
	std::map<Unit, std::set<TilePosition>> mPylonLargePowerSites;

	std::map<TilePosition, int> mSmallPowerSites;
	std::map<TilePosition, int> mMediumPowerSites;
	std::map<TilePosition, int> mLargePowerSites;

	void onAddPylon(Unit unit);
	void onRemovePylon(Unit unit);

	void addToPowerField(Unit unit);
	void removeFromPowerField(Unit unit);
};

typedef Singleton<PylonPowerTrackerClass> PylonPowerTracker;