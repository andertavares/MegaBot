#pragma once

#include "Interface.h"

#include <boost/scoped_ptr.hpp>

#include "TypeSafeEnum.h"

#include "OperatorType.h"

namespace UnitFilterFlags
{
	enum type
	{
		None = 0,
		IsComplete = 1,
		HasAddon = (1 << 1),
		IsLifted = (1 << 2),
		IsWorker = (1 << 3),
		IsArmyUnit = (1 << 4),
		CanAttackGround = (1 << 5),
		CanAttackAir = (1 << 6),
		IsSpellCaster = (1 << 7),
		All = -1
	};
}

namespace UnitPositionFlags
{
	enum type
	{
		None = 0,
		CanTravelSafely = 1,
		All = -1
	};
}

struct UnitFilter
{
public:
	UnitFilter();
	UnitFilter(BWAPI::UnitType type);
	UnitFilter(UnitFilterFlags::type flags);
	UnitFilter(UnitPositionFlags::type flags, const Position &pos);

	UnitFilter(const UnitFilter &other)
		: mType(other.mType)
		, mOperatorType(other.mOperatorType)
		, mLFilter(other.mLFilter ? new UnitFilter(*other.mLFilter) : NULL)
		, mRFilter(other.mRFilter ? new UnitFilter(*other.mRFilter) : NULL)
		, mExpectedValue(other.mExpectedValue)
		, mUnitType(other.mUnitType)
		, mUnitFlags(other.mUnitFlags)
		, mPositionFlags(other.mPositionFlags)
		, mPosition(other.mPosition)
	{}

	UnitFilter &UnitFilter::operator=(const UnitFilter &other)
	{
		mType = other.mType;
		mOperatorType = other.mOperatorType;

		mExpectedValue = other.mExpectedValue;

		mUnitType = other.mUnitType;
		mUnitFlags = other.mUnitFlags;
		mPositionFlags = other.mPositionFlags;
		mPosition = other.mPosition;

		mLFilter.reset(other.mLFilter ? new UnitFilter(*other.mLFilter) : NULL);
		mRFilter.reset(other.mRFilter ? new UnitFilter(*other.mRFilter) : NULL);

		return *this;
	}

	UnitFilter operator||(const UnitFilter& other) const
	{
		return UnitFilter(*this, OperatorType::Or, other);
	}
	UnitFilter operator&&(const UnitFilter& other) const
	{
		return UnitFilter(*this, OperatorType::And, other);
	}

	bool passesFilter(const Unit &unit) const
	{
		if(mOperatorType == OperatorType::None)
			return filter(unit);
		else if(mOperatorType == OperatorType::And)
			return mLFilter->passesFilter(unit) == mExpectedValue && mRFilter->passesFilter(unit) == mExpectedValue;
		else if(mOperatorType == OperatorType::Or)
			return mLFilter->passesFilter(unit) == mExpectedValue || mRFilter->passesFilter(unit) == mExpectedValue;

		return false;
	}

	UnitFilter &operator!();
	bool operator!=(const UnitFilter& other) const;
	bool operator==(const UnitFilter& other) const;
	bool operator<(const UnitFilter& other) const;

private:
	struct UnitFilterTypeDef
	{
		enum type
		{
			None,
			UnitOfType,
			PassesFilterFlags,
			PassesPositionFlags
		};
	};
	typedef SafeEnum<UnitFilterTypeDef> UnitFilterType;

	UnitFilterType mType;

	OperatorType mOperatorType;
	boost::scoped_ptr<UnitFilter> mLFilter;
	boost::scoped_ptr<UnitFilter> mRFilter;

	bool mExpectedValue;

	BWAPI::UnitType mUnitType;
	UnitFilterFlags::type mUnitFlags;

	UnitPositionFlags::type mPositionFlags;
	Position mPosition;

	bool filter(const Unit &unit) const;

	UnitFilter(const UnitFilter &left, OperatorType::type opType, const UnitFilter &right)
		: mType(UnitFilterType::None)
		, mOperatorType(opType)
		, mLFilter(new UnitFilter(left))
		, mRFilter(new UnitFilter(right))
		, mExpectedValue(true)
		, mUnitType(BWAPI::UnitTypes::None)
		, mUnitFlags(UnitFilterFlags::None)
		, mPositionFlags(UnitPositionFlags::None)
		, mPosition(BWAPI::Positions::None)
	{}
};