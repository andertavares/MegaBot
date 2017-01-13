#pragma once

#include "Interface.h"

#include <boost/scoped_ptr.hpp>

#include "OperatorType.h"
#include "TypeSafeEnum.h"

struct ConditionTestDef
{
	enum type
	{
		None,
		enemyUnitCountGreaterEqualThan,
		enemyUnitCountLessThan,
		enemyHasResearched,
		enemyHasUpgraded,
		enemyHasExpanded,
		isResearching,
		isResearched,
		isUpgrading,
		isUpgraded,
		myPlannedUnitTotalGreaterEqualThan,
		myPlannedUnitTotalLessThan,
		myUnitTotalBuildCountGreaterEqualThan,
		myUnitTotalBuildCountLessThan,
		myUnitCountGreaterEqualThan,
		myUnitCountLessThan,
		isEnemyZerg,
		isEnemyProtoss,
		isEnemyTerran,
		isEnemyUnknown,
		isContained,
		canForgeExpand,
		mapHasIsland,
		mapHasMineralOnlyNatural,
		distanceBetweenBotAndEnemy,
		minDistanceBetweenMainsLessThan,
		minDistanceBetweenMainsGreaterThan,
		mapSize,
		numberOfEnemies,
		enemyMainSize,
		randomChance,
		mapIs
	};
};
typedef SafeEnum<ConditionTestDef> ConditionTest;

struct Condition
{
public:
	Condition() : mType(ConditionTest::None), mOperatorType(OperatorType::None), mNeededValue(false) {}
	Condition(ConditionTest type, bool expectedValue = true);
	Condition(ConditionTest type, int extraValue);
	Condition(ConditionTest type, bool expectedValue, int extraValue);
	Condition(ConditionTest type, double extraValue);
	Condition(ConditionTest type, bool expectedValue, double extraValue);
	Condition(ConditionTest type, BWAPI::UnitType unitType, int count = 1);
	Condition(ConditionTest type, BWAPI::TechType tech);
	Condition(ConditionTest type, BWAPI::UpgradeType upgrade, int level = 1);
	Condition(ConditionTest type, std::string string);

	Condition(const Condition &other)
		: mType(other.mType)
		, mOperatorType(other.mOperatorType)
		, mLCondition(other.mLCondition ? new Condition(*other.mLCondition) : NULL)
		, mRCondition(other.mRCondition ? new Condition(*other.mRCondition) : NULL)
		, mNeededValue(other.mNeededValue)
		, mExtraInt(other.mExtraInt)
		, mExtraDouble(other.mExtraDouble)
		, mUnitType(other.mUnitType)
		, mTech(other.mTech)
		, mUpgrade(other.mUpgrade)
		, mString(other.mString)
	{
	}

	Condition &Condition::operator=(const Condition &other)
	{
		mType = other.mType;
		mOperatorType = other.mOperatorType;
		mNeededValue = other.mNeededValue;
		mExtraInt = other.mExtraInt;
		mExtraDouble = other.mExtraDouble;
		mUnitType = other.mUnitType;
		mTech = other.mTech;
		mUpgrade = other.mUpgrade;
		mString = other.mString;

		mLCondition.reset(other.mLCondition ? new Condition(*other.mLCondition) : NULL);
		mRCondition.reset(other.mRCondition ? new Condition(*other.mRCondition) : NULL);

		return *this;
	}

	bool evauluate() const
	{
		if(mOperatorType == OperatorType::None)
			return passesValue();
		else if(mOperatorType == OperatorType::And)
			return mLCondition->evauluate() && mRCondition->evauluate();
		else if(mOperatorType == OperatorType::Or)
			return mLCondition->evauluate() || mRCondition->evauluate();

		return false;
	}

	Condition operator&&(const Condition &other) const
	{
		return Condition(*this, OperatorType::And, other);
	}

	Condition operator||(const Condition &other) const
	{
		return Condition(*this, OperatorType::Or, other);
	}

private:
	ConditionTest mType;

	OperatorType mOperatorType;
	boost::scoped_ptr<Condition> mLCondition;
	boost::scoped_ptr<Condition> mRCondition;

	bool mNeededValue;
	int mExtraInt;
	double mExtraDouble;

	BWAPI::UnitType mUnitType;
	BWAPI::TechType mTech;
	BWAPI::UpgradeType mUpgrade;
	std::string mString;

	bool passesValue() const;

	Condition(const Condition &left, OperatorType::type opType, const Condition &right)
		: mType(ConditionTest::None)
		, mOperatorType(opType)
		, mLCondition(new Condition(left))
		, mRCondition(new Condition(right))
		, mNeededValue(false)
		, mExtraInt(0)
		, mExtraDouble(0)
		, mUnitType(BWAPI::UnitTypes::None)
		, mTech(BWAPI::TechTypes::None)
		, mUpgrade(BWAPI::UpgradeTypes::None)
		, mString()
	{}
};