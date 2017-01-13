#include "MorphTask.h"

#include "ResourceTracker.h"

using namespace SkynetBot;

MorphTask::MorphTask(TaskType priority, BWAPI::UnitType type)
	: Task(priority)
	, mType(type)
	, mReservedResources(false)
	, mRequiredSatisfyTime(BWAPI::Broodwar->getFrameCount())
{
}

int MorphTask::getEndTime() const
{
	if(mUnit && (mUnit->isMorphing() || !isCompleted()))
		return mUnit->getCompletedTime();

	return mRequiredSatisfyTime + mType.buildTime();
}

int MorphTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position MorphTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position MorphTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool MorphTask::preUpdate()
{
	if(mUnit && mUnit->isCompleted() && mUnit->getType() == mType)
	{
		complete();
		return true;
	}

	return false;
}

bool MorphTask::update()
{
	if(mUnit && mUnit->getType() != mType)
		mUnit->morph(mType);

	return hasEnded() && !mUnit;
}

bool MorphTask::waitingForUnit(Unit unit) const
{
	return false;
}

void MorphTask::giveUnit(Unit unit)
{
	if(unit->getType() == mType.whatBuilds().first)
	{
		mUnit = unit;
		reserveResources();
	}
}

void MorphTask::returnUnit(Unit unit)
{
	if(mUnit == unit)
	{
		reserveResources();
		mUnit = StaticUnits::nullunit;
	}
	else
		assert(false);
}

bool MorphTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	if(unit->getType() == mType || unit->getBuildType() == mType)
	{
		freeResources();
		return false;
	}

	return true;
}

UnitGroup MorphTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	if(mUnit)
		returnUnits.insert(mUnit);

	return returnUnits;
}

int MorphTask::getPriority(Unit unit) const
{
	return 40;
}

void MorphTask::updateRequirements()
{
	clearRequirements();

	if(!mUnit)
	{
		freeResources();

		RequirementGroup requiredConditions;
		requiredConditions.addUnitFilterRequirement(40, mType.buildTime(), UnitFilter(mType.whatBuilds().first) && UnitFilter(UnitFilterFlags::IsComplete));

		requiredConditions.addTechForTypeRequirement(mType);

		if(mType.mineralPrice() > 0) requiredConditions.addMineralRequirement(mType.mineralPrice());
		if(mType.gasPrice() > 0) requiredConditions.addGasRequirement(mType.gasPrice());
		if(mType.supplyRequired() > 0) requiredConditions.addSupplyRequirement(mType.supplyRequired());

		addRequirement(requiredConditions);
	}
}

void MorphTask::reserveResources()
{
	if(!mReservedResources && !hasEnded())
	{
		ResourceTracker::Instance().reserveCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().reserveCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		ResourceTracker::Instance().reserveCurrentSupply(mRequiredSatisfyTime, mType.supplyRequired());
		mReservedResources = true;
	}
}

void MorphTask::freeResources()
{
	if(mReservedResources)
	{
		ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		ResourceTracker::Instance().releaseCurrentSupply(mRequiredSatisfyTime, mType.supplyRequired());
		mReservedResources = false;
	}
}