#include "UnitFilter.h"

#include "UnitHelper.h"
#include "PathFinder.h"
#include "BorderTracker.h"
#include "TerrainAnaysis.h"

UnitFilter::UnitFilter()
	: mType(UnitFilterType::None)
	, mOperatorType(OperatorType::None)
	, mLFilter()
	, mRFilter()
	, mExpectedValue(true)
	, mUnitType(BWAPI::UnitTypes::None)
	, mUnitFlags(UnitFilterFlags::None)
	, mPositionFlags(UnitPositionFlags::None)
	, mPosition(BWAPI::Positions::None)
{
}

UnitFilter::UnitFilter(BWAPI::UnitType type)
	: mType(UnitFilterType::UnitOfType)
	, mOperatorType(OperatorType::None)
	, mLFilter()
	, mRFilter()
	, mExpectedValue(true)
	, mUnitType(type)
	, mUnitFlags(UnitFilterFlags::None)
	, mPositionFlags(UnitPositionFlags::None)
	, mPosition(BWAPI::Positions::None)
{
}

UnitFilter::UnitFilter(UnitFilterFlags::type flags)
	: mType(UnitFilterType::PassesFilterFlags)
	, mOperatorType(OperatorType::None)
	, mLFilter()
	, mRFilter()
	, mExpectedValue(true)
	, mUnitType(BWAPI::UnitTypes::None)
	, mUnitFlags(flags)
	, mPositionFlags(UnitPositionFlags::None)
	, mPosition(BWAPI::Positions::None)
{
}

UnitFilter::UnitFilter(UnitPositionFlags::type flags, const Position &pos)
	: mType(UnitFilterType::PassesPositionFlags)
	, mOperatorType(OperatorType::None)
	, mLFilter()
	, mRFilter()
	, mExpectedValue(true)
	, mUnitType(BWAPI::UnitTypes::None)
	, mUnitFlags(UnitFilterFlags::None)
	, mPositionFlags(flags)
	, mPosition(pos)
{
}

UnitFilter &UnitFilter::operator!()
{
	mExpectedValue = !mExpectedValue;
	return *this;
}

bool UnitFilter::operator!=(const UnitFilter& other) const
{
	return !operator==(other);
}

bool UnitFilter::operator==(const UnitFilter& other) const
{
	if(mType != other.mType)
		return false;
	else if(mOperatorType != other.mOperatorType)
		return false;
	else if(*mLFilter != *other.mLFilter)
		return false;
	else if(*mRFilter != *other.mRFilter)
		return false;
	else if(mExpectedValue != other.mExpectedValue)
		return false;
	else if(mUnitType != other.mUnitType)
		return false;
	else if(mUnitFlags != other.mUnitFlags)
		return false;
	else if(mPositionFlags != other.mPositionFlags)
		return false;
	else if(mPosition != other.mPosition)
		return false;
	else
		return true;
}

bool UnitFilter::operator<(const UnitFilter& other) const
{
	if(mType < other.mType)
		return true;
	else if(mType != other.mType)
		return false;

	if(mOperatorType < other.mOperatorType)
		return true;
	else if(mOperatorType != other.mOperatorType)
		return false;

	if(*mLFilter < *other.mLFilter)
		return true;
	else if(*mLFilter != *other.mLFilter)
		return false;

	if(*mRFilter < *other.mRFilter)
		return true;
	else if(*mRFilter != *other.mRFilter)
		return false;

	if(mExpectedValue < other.mExpectedValue)
		return true;
	else if(mExpectedValue != other.mExpectedValue)
		return false;

	if(mUnitType < other.mUnitType)
		return true;
	else if(mUnitType != other.mUnitType)
		return false;

	if(mUnitFlags < other.mUnitFlags)
		return true;
	else if(mUnitFlags != other.mUnitFlags)
		return false;

	if(mPositionFlags < other.mPositionFlags)
		return true;
	else if(mPositionFlags != other.mPositionFlags)
		return false;

	if(mPosition < other.mPosition)
		return true;
	else if(mPosition != other.mPosition)
		return false;

	return false;
}

bool UnitFilter::filter(const Unit &unit) const
{
	switch(mType.underlying())
	{
	case UnitFilterType::None:
		return mExpectedValue;
	case UnitFilterType::UnitOfType:
		return (unit->getType() == mUnitType) == mExpectedValue;
	case UnitFilterType::PassesFilterFlags:
		{
			if((mUnitFlags & UnitFilterFlags::IsComplete) != 0 && unit->isCompleted() != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::HasAddon) != 0 && unit->hasAddon() != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::IsLifted) != 0 && unit->isLifted() != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::IsWorker) != 0 && unit->getType().isWorker() != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::IsArmyUnit) != 0 && UnitHelper::isArmyUnit(unit->getType()) != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::CanAttackGround) != 0 && unit->canAttackGround() != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::CanAttackAir) != 0 && unit->canAttackAir() != mExpectedValue)
				return false;
			if((mUnitFlags & UnitFilterFlags::IsSpellCaster) != 0 && unit->getType().isSpellcaster() != mExpectedValue)
				return false;

			return true;
		}
	case UnitFilterType::PassesPositionFlags:
		{
			RegionPath regionPath = PathFinder::Instance().CreateRegionPath(TerrainAnaysis::Instance().getRegion(unit->getPosition()), TerrainAnaysis::Instance().getRegion(mPosition));

			if(!regionPath.isComplete && mExpectedValue)
				return false;

			for each(Region region in regionPath.path)
			{
				if((mUnitFlags & UnitPositionFlags::CanTravelSafely) != 0 && BorderTracker::Instance().isRegionSafe(region) != mExpectedValue)
					return false;
			}

			return true;
		}
	}

	return false;
}