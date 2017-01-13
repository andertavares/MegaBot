#include "PlayerTracker.h"

#include <sstream>

#include "UnitTracker.h"
#include "BaseTracker.h"
#include "UnitHelper.h"

using namespace SkynetBot;

void PlayerTrackerClass::onBegin()
{
	updatePlayers();
}

void PlayerTrackerClass::update()
{
	if(mShowDebugInfo)
	{
		std::stringstream ss;
		ss << "There are ";

		for each(BWAPI::Race race in BWAPI::Races::allRaces())
		{
			if(isEnemyRace(race))
				ss << "[" << race.getName() << "] ";
		}

		ss << "Enemy Races";

		BWAPI::Broodwar->drawText(BWAPI::CoordinateType::Screen, 15, 5, "%s", ss.str().c_str());

		int y = 20;
		if(!getEnemies().empty())
		{
			BWAPI::Broodwar->drawTextScreen(5, y, "Enemies:");
			for each(Player player in getEnemies())
			{
				std::stringstream ss2;
				ss2 << "[" << player->getRace().getName() << "] " << player->getName();
				BWAPI::Broodwar->drawTextScreen(15, y += 10, "%s", ss2.str().c_str());
			}
		}

		if(!getAllies().empty())
		{
			y += 10;
			BWAPI::Broodwar->drawTextScreen(5, y, "Allies:");
			for each(Player player in getAllies())
			{
				y+=10;
				BWAPI::Broodwar->drawTextScreen(15, y, "%s", player->getName().c_str());
			}
		}
	}

	if(BWAPI::Broodwar->getFrameCount() % 240 == 0)
		updatePlayers();

	UnitGroup allEnemyUnits;
	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		if(unit->accessibility() == AccessType::Lost)
			continue;

		const BWAPI::UnitType &type = unit->getType();
		if(type.isBuilding() && !UnitHelper::isStaticDefense(type))
			continue;

		if(type.isWorker() || type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine || type == BWAPI::UnitTypes::Zerg_Overlord || type == BWAPI::UnitTypes::Zerg_Larva || type == BWAPI::UnitTypes::Zerg_Egg)
			continue;

		allEnemyUnits.insert(unit);
	}

	mEnemyClusters = UnitHelper::getClusters(allEnemyUnits, 384, 1);
}

void PlayerTrackerClass::onPlayerLeft(Player player)
{
	updatePlayers();
}

void PlayerTrackerClass::onDiscover(Unit unit)
{
	if(unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
		mResearched[unit->getPlayer()].insert(BWAPI::TechTypes::Tank_Siege_Mode);
	else if(unit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		mResearched[unit->getPlayer()].insert(BWAPI::TechTypes::Spider_Mines);
}

void PlayerTrackerClass::onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType)
{
	onDiscover(unit);
}

void PlayerTrackerClass::updatePlayers()
{
	mEnemies.clear();
	mAllies.clear();
	mEnemyRaceInfo.clear();

	for each(Player player in BWAPI::Broodwar->getPlayers())
	{
		if(player->leftGame())
			continue;

		if(player == BWAPI::Broodwar->self())
			continue;

		if(player == BWAPI::Broodwar->neutral())
			continue;

		if(BWAPI::Broodwar->self()->isEnemy(player))
		{
			mEnemies.insert(player);
			mEnemyRaceInfo[player->getRace()] = true;
		}
		else if(BWAPI::Broodwar->self()->isAlly(player))
			mAllies.insert(player);
	}
}

bool PlayerTrackerClass::isEnemyRace(BWAPI::Race race)
{
	return mEnemyRaceInfo[race];
}

bool PlayerTrackerClass::playerHasReseached(Player player, BWAPI::TechType tech)
{
	return mResearched[player].find(tech) != mResearched[player].end();
}

bool PlayerTrackerClass::enemyHasReseached(BWAPI::TechType tech)
{
	for each(Player enemy in getEnemies())
	{
		if(playerHasReseached(enemy, tech))
			return true;
	}

	return false;
}

std::set<Player> PlayerTrackerClass::getEnemies(Player player) const
{
	if(player == BWAPI::Broodwar->self())
		return mEnemies;

	std::set<Player> enemies;
	for each(Player enemy in BWAPI::Broodwar->getPlayers())
	{
		if(player->isEnemy(enemy))
			enemies.insert(enemy);
	}

	return enemies;
}

std::set<Player> PlayerTrackerClass::getAllies(Player player) const
{
	if(player == BWAPI::Broodwar->self())
		return mAllies;

	std::set<Player> allies;
	for each(Player ally in BWAPI::Broodwar->getPlayers())
	{
		if(player->isAlly(ally))
			allies.insert(ally);
	}

	return allies;
}

int PlayerTrackerClass::playerUnitCount(BWAPI::UnitType unit, Player player) const
{
	return UnitTracker::Instance().selectAllUnits(unit, player).size();
}

int PlayerTrackerClass::enemyUnitCount(BWAPI::UnitType unit, Player player) const
{
	int count = 0;
	for each(Player enemy in getEnemies(player))
	{
		count += playerUnitCount(unit, enemy);
	}

	return count;
}