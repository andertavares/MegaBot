#include "TrainTask.h"

#include "Logger.h"
#include "ResourceTracker.h"

using namespace SkynetBot;

TrainTask::TrainTask(TaskType priority, BWAPI::UnitType type)
	: Task(priority)
	, mType(type)
	, mReservedResources(false)
	, mRequiredSatisfyTime(BWAPI::Broodwar->getFrameCount())
	, mOrderTime(0)
{
}

int TrainTask::getEndTime() const
{
	if(mProducedUnit && mProductionBuilding)
		return BWAPI::Broodwar->getFrameCount() + mProductionBuilding->getRemainingTrainTime();

	if(mProductionBuilding)
		return BWAPI::Broodwar->getFrameCount() + mType.buildTime();

	if(mRequiredSatisfyTime == Requirement::maxTime)
		return mRequiredSatisfyTime;
	
	return std::max(mRequiredSatisfyTime, BWAPI::Broodwar->getFrameCount()) + mType.buildTime();
}

int TrainTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position TrainTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position TrainTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool TrainTask::preUpdate()
{
	if(mProducedUnit && mProducedUnit->isCompleted())
	{
		complete();
		return true;
	}

	if(mProductionBuilding && shouldReturnUnit())
		return true;

	return false;
}

bool TrainTask::update()
{
	if(mProductionBuilding)
	{
		if(!mProducedUnit)
		{
			if(!mProductionBuilding->isTraining())
				mProductionBuilding->train(mType);
		}
		else if(mProducedUnit->getType() != mType)
			mProductionBuilding->cancel();
	}

	return hasEnded() && !mProductionBuilding && !mProducedUnit;
}

bool TrainTask::waitingForUnit(Unit unit) const
{
	if(!shouldReturnUnit() && mProductionBuilding && mProductionBuilding->getBuildUnit() == unit)
		return true;

	return false;
}

void TrainTask::giveUnit(Unit unit)
{
	if(unit->getType() == mType.whatBuilds().first)
	{
		reserveResources();
		mProductionBuilding = unit;
	}
	else
	{
		freeResources();
		mProducedUnit = unit;
	}
}

void TrainTask::returnUnit(Unit unit)
{
	if(mProductionBuilding == unit)
	{
		freeResources();
		mProductionBuilding = StaticUnits::nullunit;
	}
	else if(mProducedUnit == unit)
	{
		freeResources();
		mProducedUnit = StaticUnits::nullunit;
	}
	else
		assert(false);
}

bool TrainTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup TrainTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	if(mProductionBuilding)
		returnUnits.insert(mProductionBuilding);
	if(mProducedUnit)
		returnUnits.insert(mProducedUnit);

	return returnUnits;
}

int TrainTask::getPriority(Unit unit) const
{
	return 40;
}

void TrainTask::updateRequirements()
{
	clearRequirements();

	if(!mProductionBuilding)
	{
		RequirementGroup requiredConditions;

		freeResources();

		requiredConditions.addUnitFilterRequirement(40, mType.buildTime(), UnitFilter(mType.whatBuilds().first) && UnitFilter(UnitFilterFlags::IsComplete) && !UnitFilter(UnitFilterFlags::IsLifted));

		requiredConditions.addTechForTypeRequirement(mType);

		if(mType.mineralPrice() > 0) requiredConditions.addMineralRequirement(mType.mineralPrice());
		if(mType.gasPrice() > 0) requiredConditions.addGasRequirement(mType.gasPrice());
		if(mType.supplyRequired() > 0) requiredConditions.addSupplyRequirement(mType.supplyRequired());

		addRequirement(requiredConditions);
	}
}

void TrainTask::reserveResources()
{
	if(!mReservedResources && !hasEnded())
	{
		ResourceTracker::Instance().reserveCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().reserveCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		ResourceTracker::Instance().reserveCurrentSupply(mRequiredSatisfyTime, mType.supplyRequired());
		mReservedResources = true;
	}
}

void TrainTask::freeResources()
{
	if(mReservedResources)
	{
		ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		ResourceTracker::Instance().releaseCurrentSupply(mRequiredSatisfyTime, mType.supplyRequired());
		mReservedResources = false;
	}
}