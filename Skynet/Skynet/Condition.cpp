#include "Condition.h"

#include <cassert>
#include <ctime>

#include "MapHelper.h"
#include "WallTracker.h"
#include "PlayerTracker.h"
#include "MacroManager.h"
#include "Logger.h"

#include <boost/random.hpp>

Condition::Condition(ConditionTest type, bool expectedValue)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(expectedValue)
	, mExtraInt(0)
	, mExtraDouble(0)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
}

Condition::Condition(ConditionTest type, int extraValue)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(false)
	, mExtraInt(extraValue)
	, mExtraDouble(0)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
}

Condition::Condition(ConditionTest type, bool expectedValue, int extraValue)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(expectedValue)
	, mExtraInt(extraValue)
	, mExtraDouble(0)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
}

Condition::Condition(ConditionTest type, double extraValue)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(false)
	, mExtraInt(0)
	, mExtraDouble(extraValue)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
	if(type == ConditionTest::randomChance)
	{
		boost::mt19937 rng(static_cast<unsigned int>(std::time(0)));
		boost::uniform_real<> dist(0.0, 1.0);
		boost::variate_generator<boost::mt19937, boost::uniform_real<>> randValue(rng, dist);

		mNeededValue = (extraValue >= randValue());
	}
}

Condition::Condition(ConditionTest type, bool expectedValue, double extraValue)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(expectedValue)
	, mExtraInt(0)
	, mExtraDouble(extraValue)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
}

Condition::Condition(ConditionTest type, BWAPI::UnitType unitType, int count)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(false)
	, mExtraInt(count)
	, mExtraDouble(0)
	, mUnitType(unitType)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
}

Condition::Condition(ConditionTest type, BWAPI::TechType tech)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(false)
	, mExtraInt(0)
	, mExtraDouble(0)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(tech)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString()
{
}

Condition::Condition(ConditionTest type, BWAPI::UpgradeType upgrade, int level)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(false)
	, mExtraInt(level)
	, mExtraDouble(0)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(upgrade)
	, mString()
{
}

Condition::Condition(ConditionTest type, std::string string)
	: mType(type)
	, mOperatorType(OperatorType::None)
	, mLCondition()
	, mRCondition()
	, mNeededValue(false)
	, mExtraInt(0)
	, mExtraDouble(0)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTech(BWAPI::TechTypes::None)
	, mUpgrade(BWAPI::UpgradeTypes::None)
	, mString(string)
{
}

bool Condition::passesValue() const
{
	switch(mType.underlying())
	{
	case ConditionTest::myUnitTotalBuildCountGreaterEqualThan:
		return BWAPI::Broodwar->self()->deadUnitCount(mUnitType) + MacroManager::Instance().getPlannedTotal(mUnitType) >= mExtraInt;
	case ConditionTest::myUnitTotalBuildCountLessThan:
		return BWAPI::Broodwar->self()->deadUnitCount(mUnitType) + MacroManager::Instance().getPlannedTotal(mUnitType) < mExtraInt;
	case ConditionTest::enemyUnitCountGreaterEqualThan:
		return PlayerTracker::Instance().enemyUnitCount(mUnitType) >= mExtraInt;
	case ConditionTest::enemyUnitCountLessThan:
		return PlayerTracker::Instance().enemyUnitCount(mUnitType) < mExtraInt;
	case ConditionTest::myUnitCountGreaterEqualThan:
		return PlayerTracker::Instance().playerUnitCount(mUnitType) >= mExtraInt;
	case ConditionTest::myUnitCountLessThan:
		return PlayerTracker::Instance().playerUnitCount(mUnitType) < mExtraInt;
	//case ConditionTest::minDistanceBetweenMainsLessThan:
		//return MapInformation::Instance().minMainDistance() < mExtraFloat;
	//case ConditionTest::minDistanceBetweenMainsGreaterThan:
		//return MapInformation::Instance().minMainDistance() > mExtraFloat;
	case ConditionTest::None:
		return mNeededValue;
	case ConditionTest::isEnemyProtoss:
		return PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Protoss) == mNeededValue;
	case ConditionTest::isEnemyZerg:
		return PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Zerg) == mNeededValue;
	case ConditionTest::isEnemyTerran:
		return PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Terran) == mNeededValue;
	case ConditionTest::isEnemyUnknown:
		return PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Unknown) == mNeededValue;
	case ConditionTest::enemyHasResearched:
		return PlayerTracker::Instance().enemyHasReseached(mTech);
	case ConditionTest::mapSize:
		return BWAPI::Broodwar->getStartLocations().size() == mExtraInt;
	case ConditionTest::numberOfEnemies:
		return PlayerTracker::Instance().getEnemies().size() == mExtraInt;
	case ConditionTest::canForgeExpand:
		return WallTracker::Instance().canForgeExpand();
	case ConditionTest::myPlannedUnitTotalGreaterEqualThan:
		return MacroManager::Instance().getPlannedTotal(mUnitType) >= mExtraInt;
	case ConditionTest::myPlannedUnitTotalLessThan:
		return MacroManager::Instance().getPlannedTotal(mUnitType) < mExtraInt;
	case ConditionTest::mapIs:
		return MapHelper::mapIs(mString);
	case ConditionTest::isResearching:
		return BWAPI::Broodwar->self()->isResearching(mTech) || BWAPI::Broodwar->self()->hasResearched(mTech);
	case ConditionTest::isResearched:
		return BWAPI::Broodwar->self()->hasResearched(mTech);
	case ConditionTest::isUpgrading:
		return BWAPI::Broodwar->self()->isUpgrading(mUpgrade) && (mExtraInt == 1 || BWAPI::Broodwar->self()->getUpgradeLevel(mUpgrade) == mExtraInt-1);
	case ConditionTest::isUpgraded:
		return BWAPI::Broodwar->self()->getUpgradeLevel(mUpgrade) == mExtraInt;
	case ConditionTest::randomChance:
		return mNeededValue;
	}
	return false;
}
