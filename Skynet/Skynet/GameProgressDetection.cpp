#include "GameProgressDetection.h"

#include "UnitTracker.h"
#include "ResourceTracker.h"
#include "PlayerTracker.h"
#include "MacroManager.h"

void GameProgressDetectionClass::update()
{
	mState = StateType::TechNormal;
	mShouldGG = false;
	mShouldAttack = true;

	int enemyKnownSupply = 0;
	int enemyGuessSupply = 0;
	int mySupply = BWAPI::Broodwar->self()->supplyUsed();

	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		int supply = unit->getType().supplyRequired();

		if(unit->accessibility() != AccessType::Lost)
			enemyKnownSupply += supply;

		enemyGuessSupply += supply;
	}

	if(PlayerTracker::Instance().getEnemies().size() < 2)
	{
		int buildingCount = 0;
		for each(Unit unit in UnitTracker::Instance().selectAllUnits())
		{
			if(unit->getType().isBuilding() && !unit->getType().isAddon())
				++buildingCount;
		}

		if(buildingCount != 0)
		{
			if(enemyKnownSupply > mySupply*2 && ResourceTracker::Instance().getMineralRate() < 5.0)
				mShouldGG = true;

			if(enemyGuessSupply > mySupply*3 && ResourceTracker::Instance().getMineralRate() < 5.0)
				mShouldGG = true;

			if(enemyGuessSupply > mySupply*4 || enemyKnownSupply > mySupply*3)
				mShouldGG = true;
		}
	}
	
	// if bases available to attack are up small chokes, dont attack without recall or a much bigger army
	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
	{
		if(UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_Observer).size() == 0)
		{
			if(BWAPI::Broodwar->getFrameCount() > 24*60*10)
				mState = StateType::TechHigh;
			else if((UnitTracker::Instance().selectAllEnemy(BWAPI::UnitTypes::Zerg_Lurker).size() + UnitTracker::Instance().selectAllEnemy(BWAPI::UnitTypes::Protoss_Dark_Templar).size()) > 0)
				mState = StateType::TechHigh;
		}
		if(BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Leg_Enhancements) == 0 && !BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Leg_Enhancements) && PlayerTracker::Instance().enemyHasReseached(BWAPI::TechTypes::Tank_Siege_Mode))
			mState = StateType::TechHigh;

		if(PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Protoss) && !MacroManager::Instance().hasRequirements(BWAPI::UnitTypes::Protoss_High_Templar) && UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_Reaver).empty())
		{
			if(BWAPI::Broodwar->getFrameCount() > 24*60*12 || (UnitTracker::Instance().selectAllEnemy(BWAPI::UnitTypes::Protoss_High_Templar).size() + UnitTracker::Instance().selectAllEnemy(BWAPI::UnitTypes::Protoss_Reaver).size()) > 0)
				mState = StateType::TechHigh;
		}
	}

	if(mState != StateType::TechHigh)
	{
		if(enemyGuessSupply > mySupply)//TODO: also if attacking successfully
			mState = StateType::BuildArmy;
		else
		{
			if(!mShouldAttack || mySupply > 380)
				mState = StateType::TechHigh;
		}
	}
}