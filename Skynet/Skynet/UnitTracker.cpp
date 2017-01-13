#include "UnitTracker.h"
#include "BaseTracker.h"
#include "UnitTracker.h"
#include "TaskManager.h"
#include "BuildingPlacer.h"
#include "ResourceManager.h"
#include "PlayerTracker.h"
#include "UnitPredictor.h"
#include "PylonPowerTracker.h"
#include "Logger.h"

using namespace SkynetBot;

UnitTrackerClass::UnitTrackerClass()
	: mOnBeginCalled(false)
{
}

void UnitTrackerClass::pumpUnitEvents()
{
	mOnBeginCalled = true;
	for(std::map<BWAPI::Unit*, Unit>::iterator it = mUnits.begin(); it != mUnits.end(); ++it)
		onDiscover(it->second);
}

void UnitTrackerClass::onUnitDiscover(BWAPI::Unit* unit)
{
	std::map<BWAPI::Unit*, Unit>::iterator it = mUnits.find(unit);
	if(it != mUnits.end())
	{
		checkUnit(it->second);
		return;
	}

	Unit prediction = UnitPredictor::Instance().onNewUnit(unit);
	if(prediction)
	{
		mUnits[unit] = prediction;
		return;
	}

	Unit newUnit(new UnitClass(unit));
	mUnits[unit] = newUnit;

	onDiscover(newUnit);
}

void UnitTrackerClass::onUnitDestroy(BWAPI::Unit* unit)
{
	std::map<BWAPI::Unit*, Unit>::iterator it = mUnits.find(unit);
	if(it != mUnits.end())
	{
		onDestroy(it->second);
		mUnits.erase(it);
	}
}

void UnitTrackerClass::onDiscover(Unit unit)
{
	unit->update();

	mUnitToType[unit] = unit->getType();
	mUnitToPlayer[unit] = unit->getPlayer();
	mPlayerToTypeToUnits[unit->getPlayer()][unit->getType()].insert(unit);
	mPlayerToUnits[unit->getPlayer()].insert(unit);
	mAllUnits.insert(unit);

	if(unit->getType().supplyProvided() > 0 && unit->getPlayer() == BWAPI::Broodwar->self())
		mMySupplyProviders.insert(unit);

	if(!mOnBeginCalled)
		return;

	LOGMESSAGE(String_Builder() << "onDiscover() : " << unit->getType().getName() << ", ID : " << unit->getID() << ", Player : " << unit->getPlayer()->getName());

	PylonPowerTracker::Instance().onDiscover(unit);
	PlayerTracker::Instance().onDiscover(unit);
	BaseTracker::Instance().onDiscover(unit);
	TaskManager::Instance().onDiscover(unit);
}

void UnitTrackerClass::onMorphRenegade(Unit unit)
{
	Player lastPlayer = mUnitToPlayer[unit];
	BWAPI::UnitType lastType = mUnitToType[unit];

	if(lastPlayer != unit->getPlayer() || lastType != unit->getType())
	{
		Player lastPlayerForFunc = lastPlayer == unit->getPlayer() ? NULL : lastPlayer;
		BWAPI::UnitType lastTypeForFunc = lastType == unit->getType() ? BWAPI::UnitTypes::None : lastType;
		//TODO: unit has morphed, if its morphing into something known set the completed time to the build time

		if(mOnBeginCalled)
		{
			LOGMESSAGE(String_Builder() << "onMorphRenegade() : " << unit->getType().getName() << ", ID : " << unit->getID() << ", Player : " << unit->getPlayer()->getName());
			if(lastTypeForFunc != BWAPI::UnitTypes::None)
				LOGMESSAGE(String_Builder() << "    Previous Type : " << lastTypeForFunc.getName());
			if(lastPlayerForFunc != NULL)
				LOGMESSAGE(String_Builder() << "    Previous Player : " << lastPlayerForFunc->getName());

			PylonPowerTracker::Instance().onMorphRenegade(unit, lastPlayerForFunc, lastTypeForFunc);
			PlayerTracker::Instance().onMorphRenegade(unit, lastPlayerForFunc, lastTypeForFunc);
			BaseTracker::Instance().onMorphRenegade(unit, lastPlayerForFunc, lastTypeForFunc);
			TaskManager::Instance().onMorphRenegade(unit, lastPlayerForFunc, lastTypeForFunc);

			mPlayerToUnits[lastPlayer].erase(unit);
			mPlayerToTypeToUnits[lastPlayer][lastType].erase(unit);
			mUnitToType[unit] = unit->getType();
			mUnitToPlayer[unit] = unit->getPlayer();
			mPlayerToUnits[unit->getPlayer()].insert(unit);
			mPlayerToTypeToUnits[unit->getPlayer()][unit->getType()].insert(unit);
		}

		mMySupplyProviders.erase(unit);
		if(unit->getType().supplyProvided() > 0 && unit->getPlayer() == BWAPI::Broodwar->self())
			mMySupplyProviders.insert(unit);
	}
}

void UnitTrackerClass::onDestroy(Unit unit)
{
	mAllUnits.erase(unit);
	mPlayerToUnits[mUnitToPlayer[unit]].erase(unit);
	mPlayerToTypeToUnits[mUnitToPlayer[unit]][mUnitToType[unit]].erase(unit);
	mUnitToType.erase(unit);
	mUnitToPlayer.erase(unit);

	mMySupplyProviders.erase(unit);

	if(!mOnBeginCalled)
		return;

	LOGMESSAGE(String_Builder() << "onDestroy() : " << unit->getType().getName() << ", ID : " << unit->getID() << ", Player : " << unit->getPlayer()->getName());

	unit->onDestroy();
	PylonPowerTracker::Instance().onDestroy(unit);
	ResourceManager::Instance().onDestroy(unit);
	BaseTracker::Instance().onDestroy(unit);
	TaskManager::Instance().onDestroy(unit);
	BuildingPlacer::Instance().onDestroy(unit);
}

void UnitTrackerClass::update()
{
	for(std::set<Unit>::iterator it = mAllUnits.begin(); it != mAllUnits.end();)
		checkUnit(*(it++));

	for(std::map<BWAPI::Unit*, Unit>::iterator it = mUnits.begin(); it != mUnits.end();)
	{
		if(it->second->accessibility() == AccessType::Dead)
			mUnits.erase(it++);
		else
			++it;
	}
}

Unit UnitTrackerClass::getUnit(BWAPI::Unit* unit)
{
	std::map<BWAPI::Unit*, Unit>::iterator it = mUnits.find(unit);
	if(it != mUnits.end())
		 return it->second;

	return StaticUnits::nullunit;
}

UnitGroup UnitTrackerClass::getUnitGroup(std::set<BWAPI::Unit*> units)
{
	UnitGroup returnUnits;

	for each(BWAPI::Unit* unit in units)
	{
		std::map<BWAPI::Unit*, Unit>::iterator it = mUnits.find(unit);
		if(it != mUnits.end())
			returnUnits.insert(it->second);
	}

	return returnUnits;
}

UnitGroup UnitTrackerClass::getUnitsOnTile(int x, int y)
{
	return getUnitGroup(BWAPI::Broodwar->getUnitsOnTile(x, y));
}

void UnitTrackerClass::checkUnit(Unit unit)
{
	unit->update();

#ifdef SKYNET_DRAW_DEBUG
	unit->drawUnitPosition();
#endif

	if(mUnitToPlayer[unit] != unit->getPlayer() || mUnitToType[unit] != unit->getType())
		onMorphRenegade(unit);

	AccessType thisAccess = unit->accessibility();
	if(thisAccess == AccessType::Dead)
		onDestroy(unit);
}

UnitGroup UnitTrackerClass::selectAllEnemy(Player player)
{
	UnitGroup enemies;
	for each(Player player in PlayerTracker::Instance().getEnemies(player))
	{
		enemies += mPlayerToUnits[player];
	}

	return enemies;
}

UnitGroup UnitTrackerClass::selectAllEnemy(BWAPI::UnitType type, Player player)
{
	UnitGroup enemies;
	for each(Player player in PlayerTracker::Instance().getEnemies(player))
	{
		enemies += mPlayerToTypeToUnits[player][type];
	}

	return enemies;
}