#include "Base.h"

#include <boost/bind.hpp>

#include "DrawBuffer.h"
#include "UnitTracker.h"
#include "Chokepoint.h"
#include "MapHelper.h"
#include "UnitHelper.h"

BaseClass::BaseClass(Region region, std::set<TilePosition> tiles, bool startLocation)
	: mCenterTilePosition(BWAPI::TilePositions::None)
	, mCenterPosition(region->getCenter())
	, mMinedOut(true)
	, mRegion(region)
	, mIsStartLocation(startLocation)
	, mPlayer(NULL)
	, mBuildings()
	, mTiles(tiles)
	, mIsUnderAttack(false)
	, mIsContested(false)
	, mTechBuildings(0)
{
}

BaseClass::BaseClass(TilePosition position, const UnitGroup &resources, Region region, std::set<TilePosition> tiles, bool startLocation)
	: mCenterTilePosition(position)
	, mCenterPosition(Position(position.x()*32+64, position.y()*32+48))
	, mMinedOut(false)
	, mRegion(region)
	, mIsStartLocation(startLocation)
	, mPlayer(NULL)
	, mBuildings()
	, mTiles(tiles)
	, mIsUnderAttack(false)
	, mIsContested(false)
	, mTechBuildings(0)
{
	for each(Unit resource in resources)
	{
		if(resource->getType() == BWAPI::UnitTypes::Resource_Mineral_Field)
			mMinerals.insert(resource);
		else
			mGeysers.insert(resource);
	}
}

void BaseClass::update()
{
	if(!mMinedOut)
	{
		bool noGas = true;
		for each(Unit geyser in mGeysers)
		{
			if(geyser->getResources() > 0)
				noGas = false;
		}
		for each(Unit geyser in mRefineries)
		{
			if(geyser->getResources() > 0)
				noGas = false;
		}

		if(mMinerals.empty() && noGas)
			mMinedOut = true;
	}

	for(UnitGroup::iterator it = mBuildings.begin(); it != mBuildings.end();)
	{
		if((*it)->isLifted() || (*it)->accessibility() == AccessType::Lost)
		{
			Unit unit = *it;
			++it;
			removeUnit(unit, unit->getPlayer(), unit->getType());
		}
		else
			++it;
	}

	// TODO: Move this to the base manager and make a generic add building and remove building
	for each(Unit unit in mLiftedBuildings)
	{
		if(!unit->isLifted() && unit->exists())
			addUnit(unit);
	}

	mLiftedBuildings.clear();
	for each(Unit unit in UnitTracker::Instance().getAllUnits())
	{
		if(unit->getType().isBuilding() && unit->isLifted())
			mLiftedBuildings.insert(unit);
	}

	mAllThreats.clear();
	mThreatTypes.clear();
	mAllDefenders.clear();
	int workerCount = 0;

	if(mPlayer && mPlayer != BWAPI::Broodwar->neutral())
	{
		for each(Unit defender in UnitTracker::Instance().selectAllUnits(mPlayer))
		{
			if(!defender->isCompleted())
				continue;

			const BWAPI::UnitType &type = defender->getType();
			if(!UnitHelper::isArmyUnit(type) && !type.isWorker())
				continue;

			if(mTiles.count(defender->getTilePosition()) == 0)
				continue;

			if(type.isWorker())
			{
				if(workerCount > 14)//Only count 14 as thats the only amount it will pull from the mineral line
					continue;
				else
					++workerCount;
			}

			mAllDefenders.insert(defender);
		}

		for each(Unit enemy in UnitTracker::Instance().selectAllEnemy(mPlayer))
		{
			if(enemy->getType().isBuilding() && !enemy->isLifted())
				continue;

			if(enemy->accessibility() == AccessType::Lost)
				continue;

			if(mTiles.count(enemy->getTilePosition()) != 0)
			{
				mAllThreats.insert(enemy);
				mThreatTypes[UnitThreatType::Scout].insert(enemy);
			}

			if(enemy->getType().isWorker())
				continue;

			const int maxRange = std::max(48, 24 + std::max(enemy->getGroundWeaponMaxRange(), enemy->getAirWeaponMaxRange()));

			for each(Unit building in mBuildings)
			{
				if(enemy->getDistance(building) <= maxRange)
				{
					mAllThreats.insert(enemy);

					if(enemy->getType().isFlyer() || enemy->isLifted())
					{
						if(enemy->canAttackGround())
							mThreatTypes[UnitThreatType::AirToGround].insert(enemy);
						if(enemy->canAttackAir())
							mThreatTypes[UnitThreatType::AirToAir].insert(enemy);
					}
					else
					{
						if(enemy->canAttackGround())
							mThreatTypes[UnitThreatType::GroundToGround].insert(enemy);
						if(enemy->canAttackAir())
							mThreatTypes[UnitThreatType::GroundToAir].insert(enemy);
					}

					break;
				}
			}
		}
	}

	mActive = false;
	mActiveInFuture = false;
	mActivateTime = std::numeric_limits<int>::max();
	mIsUnderAttack = !mThreatTypes[UnitThreatType::AirToGround].empty() || !mThreatTypes[UnitThreatType::GroundToGround].empty();
	if(!mResourceDepots.empty())
	{
		if(!mMinedOut)
		{
			if(!mIsUnderAttack && mResourceDepots[0]->isCompleted())
			{
				mActive = true;
				mActiveInFuture = true;
				mActivateTime = 0;
			}
			else if(!mIsUnderAttack || (mAllDefenders.size() > 6 && mAllDefenders.ratingDifference(mThreatTypes[UnitThreatType::AirToGround] + mThreatTypes[UnitThreatType::GroundToGround]) >= 150))
			{
				mActiveInFuture = true;
				mActivateTime = mResourceDepots[0]->getCompletedTime();
			}
		}
	}

	//drawDebugInfo();
}

bool BaseClass::depotCompare(const Unit &depotOne, const Unit &depotTwo)
{
	int thisDistance = depotOne->getPosition().getApproxDistance(mCenterPosition);
	int currentDistance = depotTwo->getPosition().getApproxDistance(mCenterPosition);

	return thisDistance < currentDistance;
}

void BaseClass::onDiscover(Unit unit)
{
	addUnit(unit);
}

void BaseClass::onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType)
{
	removeUnit(unit, previousPlayer != NULL ? previousPlayer : unit->getPlayer(), previousType != BWAPI::UnitTypes::None ? previousType : unit->getType());
	addUnit(unit);
}

void BaseClass::onDestroy(Unit unit)
{
	removeUnit(unit, unit->getPlayer(), unit->getType());
}

void BaseClass::drawDebugInfo() const
{
	if(mPlayer != NULL)
		BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map, mCenterPosition.x(), mCenterPosition.y(), 78, mPlayer->getColor(), false);
	else
		BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map, mCenterPosition.x(), mCenterPosition.y(), 78, BWAPI::Broodwar->neutral()->getColor(), false);

	bool showtitle = false;
	int ypos = 30;

	if(mIsStartLocation)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Start Location");
		ypos -= 10;
		showtitle = true;
	}

	if(!mBuildings.empty())
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Buildings: %u", mBuildings.size());
		ypos -= 10;
		showtitle = true;
	}

	if(mTechBuildings > 0)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Number of Tech Buildings: %d", mTechBuildings);
		ypos -= 10;
		showtitle = true;
	}

	if(mPlayer != NULL)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Owner: %s", mPlayer->getName().c_str());
		ypos -= 10;
		showtitle = true;
	}

	if(isEnemyBase())
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Is Enemy");
		ypos -= 10;
		showtitle = true;
	}

	if(isAllyBase())
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Is Ally");
		ypos -= 10;
		showtitle = true;
	}

	if(isMyBase())
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Is Mine");
		ypos -= 10;
		showtitle = true;
	}

	if(mIsContested)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Base is Contested");
		ypos -= 10;
		showtitle = true;
	}

	if(mIsUnderAttack)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Base is Under Attack");
		ypos -= 10;
		showtitle = true;
	}

	if(mActive)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Base is Active");
		ypos -= 10;
		showtitle = true;
	}
	else if(mActiveInFuture)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Base is Active In Future");
		ypos -= 10;
		showtitle = true;
	}

	if(mMinedOut)
	{
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - ypos, "Base is mined out");
		ypos -= 10;
		showtitle = true;
	}

	if(showtitle)
		BWAPI::Broodwar->drawTextMap(mCenterPosition.x() + 60, mCenterPosition.y() - 40, "Base Info:");

	for each(Unit unit in mAllThreats)
	{
		BWAPI::Broodwar->drawLineMap(unit->getPosition().x(), unit->getPosition().y(), mCenterPosition.x(), mCenterPosition.y(), BWAPI::Colors::Red);
	}
	for each(Unit building in mBuildings)
	{
		BWAPI::Broodwar->drawLine(BWAPI::CoordinateType::Map, building->getPosition().x(), building->getPosition().y(), mCenterPosition.x(), mCenterPosition.y(), building->getPlayer()->getColor());
	}

	for each(Unit mineral in mMinerals)
	{
		BWAPI::Broodwar->drawCircleMap(mineral->getPosition().x(), mineral->getPosition().y(), 32, BWAPI::Colors::Blue);
		BWAPI::Broodwar->drawLineMap(mineral->getPosition().x(), mineral->getPosition().y(), mCenterPosition.x(), mCenterPosition.y(), BWAPI::Colors::Blue);
	}
	for each(Unit geyser in mGeysers)
	{
		BWAPI::Broodwar->drawCircleMap(geyser->getPosition().x(), geyser->getPosition().y(), 32, BWAPI::Colors::Green);
		BWAPI::Broodwar->drawLineMap(geyser->getPosition().x(), geyser->getPosition().y(), mCenterPosition.x(), mCenterPosition.y(), BWAPI::Colors::Green);
	}
	for each(Unit geyser in mRefineries)
	{
		BWAPI::Broodwar->drawCircleMap(geyser->getPosition().x(), geyser->getPosition().y(), 32, BWAPI::Colors::Orange);
		BWAPI::Broodwar->drawLineMap(geyser->getPosition().x(), geyser->getPosition().y(), mCenterPosition.x(), mCenterPosition.y(), BWAPI::Colors::Green);
	}
}

Unit BaseClass::getClosestEnemyBuilding(Position pos)
{
	int minDist = std::numeric_limits<int>::max();
	Unit bestBuilding;

	for each(Unit unit in mBuildings)
	{
		if(!BWAPI::Broodwar->self()->isEnemy(unit->getPlayer()))
			continue;

		int thisDist = pos.getApproxDistance(unit->getPosition());
		if(thisDist < minDist)
		{
			minDist = thisDist;
			bestBuilding = unit;
		}
	}

	return bestBuilding;
}

void BaseClass::updatePlayer()
{
	mIsContested = false;
	if(mPlayerBuildingNumbers.empty())
		mPlayer = NULL;
	else if(mPlayerBuildingNumbers.size() == 1)
		mPlayer = mPlayerBuildingNumbers.begin()->first;
	else
	{
		mIsContested = true;
		if(!mResourceDepots.empty())
			mPlayer = mResourceDepots[0]->getPlayer();
		else
		{
			mPlayer = NULL;
			int number = 0;
			for each(std::pair<Player, int> pair in mPlayerBuildingNumbers)
			{
				if(pair.second > number)
				{
					mPlayer = pair.first;
					number = pair.second;
				}
			}
		}
	}
}

void BaseClass::addUnit(Unit unit)
{
	if(mTiles.count(unit->getTilePosition()) == 0)
		return;

	const BWAPI::UnitType &type = unit->getType();

	if(type == BWAPI::UnitTypes::Resource_Vespene_Geyser)
		mGeysers.insert(unit);

	if(type.isBuilding() && unit->getPlayer() != BWAPI::Broodwar->neutral() && !unit->isLifted())
	{
		mBuildings.insert(unit);
		++mPlayerBuildingNumbers[unit->getPlayer()];

		if(type.isResourceDepot())
		{
			mResourceDepots.push_front(unit);
			if(mResourceDepots.size() > 1)
				std::sort(mResourceDepots.begin(), mResourceDepots.end(), boost::bind<bool>(&BaseClass::depotCompare, this, _1, _2));
		}

		if(!type.isResourceDepot() && type.supplyProvided() == 0 && !UnitHelper::isStaticDefense(type) && !type.isRefinery())
			++mTechBuildings;

		updatePlayer();
	}

	if(type.isRefinery())
		mRefineries.insert(unit);
}

void BaseClass::removeUnit(Unit unit, Player playerToRemove, BWAPI::UnitType typeToRemove)
{
	mRefineries.erase(unit);
	mGeysers.erase(unit);
	mMinerals.erase(unit);

	std::set<Unit>::iterator it = mBuildings.find(unit);
	if(it != mBuildings.end())
	{
		mBuildings.erase(it);
		--mPlayerBuildingNumbers[playerToRemove];
		if(mPlayerBuildingNumbers[playerToRemove] == 0)
			mPlayerBuildingNumbers.erase(playerToRemove);

		mResourceDepots.erase(std::remove(mResourceDepots.begin(), mResourceDepots.end(), unit), mResourceDepots.end());

		if(!typeToRemove.isResourceDepot() && typeToRemove.supplyProvided() == 0 && !UnitHelper::isStaticDefense(typeToRemove) && !typeToRemove.isRefinery())
			--mTechBuildings;

		updatePlayer();
	}
}