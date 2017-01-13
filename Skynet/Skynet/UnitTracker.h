#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Unit.h"
#include "UnitGroup.h"

using namespace SkynetBot;

class UnitTrackerClass
{
public:
	UnitTrackerClass();

	void pumpUnitEvents();
	void update();

	void onUnitDiscover(BWAPI::Unit* unit);
	void onUnitDestroy(BWAPI::Unit* unit);

	Unit getUnit(BWAPI::Unit* unit);
	UnitGroup getUnitGroup(std::set<BWAPI::Unit*> units);

	const UnitGroup &getGeysers() { return mPlayerToTypeToUnits[BWAPI::Broodwar->neutral()][BWAPI::UnitTypes::Resource_Vespene_Geyser]; }
	const UnitGroup &getMinerals() { return mPlayerToTypeToUnits[BWAPI::Broodwar->neutral()][BWAPI::UnitTypes::Resource_Mineral_Field]; }

	const UnitGroup &getSupplyProviders() const { return mMySupplyProviders; }

	UnitGroup getUnitsOnTile(int x, int y);

	const UnitGroup &getAllUnits() { return mAllUnits; }
	const UnitGroup &selectAllUnits(BWAPI::UnitType type, Player player = BWAPI::Broodwar->self()) { return mPlayerToTypeToUnits[player][type]; }
	const UnitGroup &selectAllUnits(Player player = BWAPI::Broodwar->self()) { return mPlayerToUnits[player]; }
	UnitGroup selectAllEnemy(Player player = BWAPI::Broodwar->self());
	UnitGroup selectAllEnemy(BWAPI::UnitType type, Player player = BWAPI::Broodwar->self());

	void onDiscover(Unit unit);
	void onMorphRenegade(Unit unit);
	void onDestroy(Unit unit);

private:
	std::map<BWAPI::Unit*, Unit> mUnits;

	std::map<Unit, Player> mUnitToPlayer;
	std::map<Unit, BWAPI::UnitType> mUnitToType;

	std::map<Player, std::map<BWAPI::UnitType, UnitGroup>> mPlayerToTypeToUnits;
	std::map<Player, UnitGroup> mPlayerToUnits;
	
	UnitGroup mAllUnits;

	UnitGroup mMySupplyProviders;

	bool mOnBeginCalled;

	void checkUnit(Unit unit);
};

typedef Singleton<UnitTrackerClass> UnitTracker;