#include "UpgradeTask.h"

#include "ResourceTracker.h"

using namespace SkynetBot;

UpgradeTask::UpgradeTask(TaskType priority, BWAPI::UpgradeType type, int level)
	: Task(priority)
	, mType(type)
	, mLevel(level)
	, mReservedResources(false)
	, mRequiredSatisfyTime(BWAPI::Broodwar->getFrameCount())
{
}

int UpgradeTask::getEndTime() const
{
	if(mUnit)
		return BWAPI::Broodwar->getFrameCount() + mUnit->getRemainingUpgradeTime();

	if(mRequiredSatisfyTime == Requirement::maxTime)
		return mRequiredSatisfyTime;

	return mRequiredSatisfyTime + mType.upgradeTime(mLevel);
}

int UpgradeTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position UpgradeTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position UpgradeTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool UpgradeTask::preUpdate()
{
	if(BWAPI::Broodwar->self()->getUpgradeLevel(mType) >= mLevel)
		complete();

	if((isStopped() || isCanceled() || isCompleted()) && mUnit && !mUnit->isUpgrading())
		return true;
	
	updateRequirements();

	return false;
}

bool UpgradeTask::update()
{
	if(mUnit)
	{
		if(mUnit->isUpgrading())
		{
			if(mUnit->getUpgrade() != mType || isStopped() || isPaused() || isCanceled())
				mUnit->cancel();
			else if(mReservedResources)
			{
				ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice(mLevel));
				ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice(mLevel));
				mReservedResources = false;
			}
		}
		else if(!shouldReturnUnit())
			mUnit->upgrade(mType);
	}

	return hasEnded() && !mUnit;
}

bool UpgradeTask::waitingForUnit(Unit unit) const
{
	return false;
}

void UpgradeTask::giveUnit(Unit unit)
{
	if(unit->getType() == mType.whatUpgrades())
	{
		mUnit = unit;
		assert(mReservedResources == false);
		ResourceTracker::Instance().reserveCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice(mLevel));
		ResourceTracker::Instance().reserveCurrentGas(mRequiredSatisfyTime, mType.gasPrice(mLevel));
		mReservedResources = true;
	}
}

void UpgradeTask::returnUnit(Unit unit)
{
	if(mUnit == unit)
	{
		mUnit = StaticUnits::nullunit;

		if(mReservedResources)
		{
			ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice(mLevel));
			ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice(mLevel));
			mReservedResources = false;
		}
	}
	else
		assert(false);
}

bool UpgradeTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup UpgradeTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	if(mUnit)
		returnUnits.insert(mUnit);

	return returnUnits;
}

int UpgradeTask::getPriority(Unit unit) const
{
	return 40;
}

void UpgradeTask::updateRequirements()
{
	clearRequirements();

	if(!mUnit && !shouldReturnUnit())
	{
		RequirementGroup requiredConditions;

		assert(mReservedResources == false);
		requiredConditions.addTechForTypeRequirement(mType, mLevel);
		requiredConditions.addUnitFilterRequirement(40, mType.upgradeTime(mLevel), UnitFilter(mType.whatUpgrades()) && UnitFilter(UnitFilterFlags::IsComplete));
		requiredConditions.addMineralRequirement(mType.mineralPrice(mLevel));
		requiredConditions.addGasRequirement(mType.gasPrice(mLevel));

		addRequirement(requiredConditions);
	}
}