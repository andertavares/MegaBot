#include "UnitFilter.h"

#include "UnitHelper.h"

UnitFilter::UnitFilter()
	: mType(UnitFilterType::None)
	, mOperatorType(OperatorType::None)
	, mLFilter()
	, mRFilter()
	, mExpectedValue(true)
	, mUnitType(BWAPI::UnitTypes::None)
	, mUnitFlags(UnitFilterFlags::None)
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
{
}

UnitFilter::UnitFilter(UnitFilterFlags::type flags)
	: mType(UnitFilterType::PassesFlags)
	, mOperatorType(OperatorType::None)
	, mLFilter()
	, mRFilter()
	, mExpectedValue(true)
	, mUnitType(BWAPI::UnitTypes::None)
	, mUnitFlags(flags)
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
	case UnitFilterType::PassesFlags:
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
	}

	return false;
}