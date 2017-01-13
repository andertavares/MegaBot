#include "BaseTracker.h"

#include <limits>
#include <cassert>

using namespace SkynetBot;

BaseTrackerClass::BaseTrackerClass()
{
}

Base BaseTrackerClass::createBase(std::set<TilePosition> tiles, Region region, bool startLocation, TilePosition position, const UnitGroup &resources)
{
	Base base;
	if(position != BWAPI::TilePositions::None)
		base = Base(new BaseClass(position, resources, region, tiles, startLocation));
	else
		base = Base(new BaseClass(region, tiles, startLocation));

	for each(TilePosition tile in tiles)
		mTileToBase[tile] = base;
	
	mBases.insert(base);

	return base;
}

void BaseTrackerClass::update()
{
	for each(Base base in mBases)
	{
		//TODO: track flying buildings, they can move between bases
		base->update();

		//base->drawDebugInfo();
	}
}

void BaseTrackerClass::onDiscover(Unit unit)
{
	for each(Base base in mBases)
	{
		base->onDiscover(unit);
	}
}

void BaseTrackerClass::onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType)
{
	for each(Base base in mBases)
	{
		base->onMorphRenegade(unit, previousPlayer, previousType);
	}
}

void BaseTrackerClass::onDestroy(Unit unit)
{
	for each(Base base in mBases)
	{
		base->onDestroy(unit);
	}
}

std::set<Base> BaseTrackerClass::getResourceBases()
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(!base->isMinedOut())
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getActiveBases(bool activeInFuture, Player player)
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(base->isActive(activeInFuture) && base->getPlayer() == player)
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getEnemyActiveBases(bool activeInFuture)
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(base->isActive(activeInFuture) && BWAPI::Broodwar->self()->isEnemy(base->getPlayer()))
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getEnemyBases()
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(BWAPI::Broodwar->self()->isEnemy(base->getPlayer()))
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getAllyBases()
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(BWAPI::Broodwar->self()->isAlly(base->getPlayer()))
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getPlayerBases(Player player)
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(base->getPlayer() == player)
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getImportantBases(Player player)
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(base->getPlayer() == player && (base->isActive(true) || base->getNumberOfTechBuildings() >= 2))
			bases.insert(base);
	}

	return bases;
}

std::set<Base> BaseTrackerClass::getEnemyImportantBases()
{
	std::set<Base> bases;

	for each(Base base in mBases)
	{
		if(BWAPI::Broodwar->self()->isEnemy(base->getPlayer()) && (base->isActive(true) || base->getNumberOfTechBuildings() > 0))
			bases.insert(base);
	}

	return bases;
}