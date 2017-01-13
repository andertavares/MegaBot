#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Unit.h"
#include "UnitGroup.h"

using namespace SkynetBot;

class PlayerTrackerClass
{
public:
	PlayerTrackerClass(){}

	void onBegin();
	void update();

	void onPlayerLeft(Player player);
	void onDiscover(Unit unit);
	void onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType);

	void updatePlayers();

	bool isEnemyRace(BWAPI::Race race);

	std::set<Player> getEnemies(Player player = BWAPI::Broodwar->self()) const;
	std::set<Player> getAllies(Player player = BWAPI::Broodwar->self()) const;

	bool playerHasReseached(Player player, BWAPI::TechType tech);
	bool enemyHasReseached(BWAPI::TechType tech);

	int playerUnitCount(BWAPI::UnitType unit, Player player = BWAPI::Broodwar->self()) const;
	int enemyUnitCount(BWAPI::UnitType unit, Player player = BWAPI::Broodwar->self()) const;

	const std::vector<UnitGroup> &getEnemyClusters() { return mEnemyClusters; }

private:
	std::set<Player> mEnemies;
	std::set<Player> mAllies;

	std::map<BWAPI::Race, bool> mEnemyRaceInfo;

	std::map<Player, bool> mPlayerHasOffensiveUnitsNeedDetection;

	std::map<Player, std::set<BWAPI::TechType>> mResearched;

	std::vector<UnitGroup> mEnemyClusters;

	bool mShowDebugInfo;
};

typedef Singleton<PlayerTrackerClass> PlayerTracker;