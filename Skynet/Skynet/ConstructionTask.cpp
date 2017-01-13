#include "ConstructionTask.h"
#include "Logger.h"
#include "ResourceTracker.h"
#include "MapHelper.h"
#include "DrawBuffer.h"

using namespace SkynetBot;

ConstructionTask::ConstructionTask(TaskType priority, BuildingLocation position, BWAPI::UnitType type)
	: Task(priority)
	, mType(type)
	, mBuildingLocation(position)
	, mReservedResources(false)
	, mRequiredSatisfyTime(BWAPI::Broodwar->getFrameCount())
	, mRequiredDelayTime(BWAPI::Broodwar->getFrameCount())
{
}

int ConstructionTask::getEndTime() const
{
	if(mProducedUnit)
		return mProducedUnit->getCompletedTime();

	if(mBuilder)
		return BWAPI::Broodwar->getFrameCount() + mType.buildTime();

	if(mRequiredSatisfyTime == Requirement::maxTime)
		return mRequiredSatisfyTime;

	return std::max(mRequiredSatisfyTime, BWAPI::Broodwar->getFrameCount()) + mType.buildTime();
}

int ConstructionTask::getEndTime(Unit unit) const
{
	if(unit->getType() == mType || mType.getRace() == BWAPI::Races::Terran)
		return getEndTime();

	if(mType.getRace() == BWAPI::Races::Protoss)
	{
		if(mRequiredSatisfyTime == Requirement::maxTime)
			return mRequiredSatisfyTime;

		return std::max(BWAPI::Broodwar->getFrameCount(), mRequiredSatisfyTime) + BWAPI::Broodwar->getLatencyFrames();
	}
	
	return Requirement::maxTime;
}

Position ConstructionTask::getStartLocation(Unit unit) const
{
	if(mReservedLocation)
		return mReservedLocation->getPosition();

	return unit->getPosition();
}

Position ConstructionTask::getEndLocation(Unit unit) const
{
	if(mReservedLocation)
		return mReservedLocation->getPosition();

	return unit->getPosition();
}

bool ConstructionTask::preUpdate()
{
	if(mProducedUnit)
	{
		if(mProducedUnit->exists())
		{
			freeResources();
			freeLocation();
		}

		if(!isStopped() && !isCanceled() && mProducedUnit->isCompleted())
		{
			complete();
			return true;
		}
	}

	if(mReservedLocation)
	{
		if(hasEnded())
			freeLocation();
		else if(isPaused() || isStopped())
		{
			freeResources();

			mRequiredSatisfyTime = Requirement::maxTime;
			mRequiredDelayTime = Requirement::maxTime;
			mReservedLocation->setStartTime(Requirement::maxTime);
		}
	}

	if(!shouldReturnUnit())
		updateRequirements();

	if(mBuilder && finishedWithBuilder() && (mType.getRace() == BWAPI::Races::Protoss || !mBuilder->isConstructing()))
		return true;

	return false;
}

bool ConstructionTask::update()
{
	if(mBuilder)
	{
		if(shouldReturnUnit())
		{
			if(!mBuilder->cancel())
				mBuilder->stop();
		}
		else
		{
			if(!mProducedUnit || !mProducedUnit->exists())
			{
				if(mReservedLocation && mReservedLocation->isCalculated())
					mBuilder->build(mReservedLocation->getTilePosition(), mType);
			}
			else if(mType.getRace() == BWAPI::Races::Terran && (!mBuilder->isConstructing() || !mProducedUnit->isBeingConstructed()))
				mBuilder->build(mProducedUnit);
		}
	}

	if((isStopped() || isCanceled()) && mProducedUnit && mProducedUnit->exists())
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Attempting to cancel build");
		mProducedUnit->cancel();
	}

	return hasEnded() && !mBuilder && !mProducedUnit;
}

bool ConstructionTask::waitingForUnit(Unit unit) const
{
	if(mReservedLocation && mReservedLocation->isCalculated())
	{
		if(mReservedLocation->getUnitPrediction() == unit)
			return true;

		if(mType.isRefinery() && unit->getTilePosition() == mReservedLocation->getTilePosition())
			return true;
	}

	return false;
}

void ConstructionTask::giveUnit(Unit unit)
{
	if(unit->getType() == mType.whatBuilds().first)
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Given Builder");
		assert(!mBuilder);
		mBuilder = unit;
		reserveResources();
	}
	else if(unit == mReservedLocation->getUnitPrediction() || unit->getTilePosition() == mReservedLocation->getTilePosition())
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Given Produced Unit");

		assert(!mProducedUnit || !mProducedUnit->exists());

		mProducedUnit = unit;
		if(mProducedUnit->exists())
		{
			freeResources();
			freeLocation();
		}
	}
	else
		assert(false);
}

void ConstructionTask::returnUnit(Unit unit)
{
	if(unit == mBuilder)
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Returning Builder");
		freeResources();
		mBuilder = StaticUnits::nullunit;
	}
	else if(unit == mProducedUnit)
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Returning Produced Unit");
		freeResources();
		mProducedUnit = StaticUnits::nullunit;
	}
	else
		assert(!unit->exists());//If it doesn't exist it was a prediction that has been replaced
}

bool ConstructionTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	if(unit == mBuilder && unit->getType() == mType)
	{
		freeLocation();
		freeResources();

		mProducedUnit = unit;
		mBuilder = StaticUnits::nullunit;

		return false;
	}

	return true;
}

UnitGroup ConstructionTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	if(mBuilder && finishedWithBuilder())
		returnUnits.insert(mBuilder);

	if(mProducedUnit && mProducedUnit->exists() && isCompleted())
		returnUnits.insert(mProducedUnit);

	return returnUnits;
}

int ConstructionTask::getPriority(Unit unit) const
{
	if(unit == mProducedUnit || (mReservedLocation && unit == mReservedLocation->getUnitPrediction()))
		return 100;

	return 40;
}

void ConstructionTask::updateRequirements()
{
	clearRequirements();

	if(!hasEnded())
	{
		reserveLocation();

		if(!finishedWithBuilder() && !mBuilder && (mProducedUnit || (mReservedLocation && mReservedLocation->isCalculated())))
		{
			Position location = mProducedUnit ? mProducedUnit->getPosition() : mReservedLocation->getPosition();
			int neededDuration = mType.getRace() == BWAPI::Races::Protoss ? BWAPI::Broodwar->getLatencyFrames() : mType.getRace() == BWAPI::Races::Zerg ? Requirement::maxTime : mType.buildTime();

			RequirementGroup requiredConditions;
			requiredConditions.addUnitFilterRequirement(40, neededDuration, UnitFilter(mType.whatBuilds().first) && UnitFilter(UnitFilterFlags::IsComplete), location);

			if(!mReservedResources && (!mProducedUnit || !mProducedUnit->exists()))
			{
				requiredConditions.addTechForTypeRequirement(mType);

				if(mType.mineralPrice() > 0) requiredConditions.addMineralRequirement(mType.mineralPrice());
				if(mType.gasPrice() > 0) requiredConditions.addGasRequirement(mType.gasPrice());
				if(mType.supplyRequired() > 0) requiredConditions.addSupplyRequirement(mType.supplyRequired());
			}

			addRequirement(requiredConditions);
		}
	}
}

void ConstructionTask::reserveResources()
{
	if(!mReservedResources && !inProgress() && !hasEnded())
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Reserving Resources");

		ResourceTracker::Instance().reserveCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().reserveCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		ResourceTracker::Instance().reserveCurrentSupply(mRequiredSatisfyTime, mType.supplyRequired());
		mReservedResources = true;
	}
}

void ConstructionTask::freeResources()
{
	if(mReservedResources)
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Freeing Resources");

		ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		ResourceTracker::Instance().releaseCurrentSupply(mRequiredSatisfyTime, mType.supplyRequired());
		mReservedResources = false;
	}
}

void ConstructionTask::reserveLocation()
{
	if(!mReservedLocation && !inProgress() && !hasEnded())
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Reserving Location");
		mReservedLocation = BuildingPlacer::Instance().getBuildingReservation(mBuildingLocation, mType);
		assert(!mReservedLocation->update());
	}
}

void ConstructionTask::freeLocation()
{
	if(mReservedLocation)
	{
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Freeing Location");
		mReservedLocation->forceCompleted();
		mReservedLocation.reset();
	}
}

void ConstructionTask::setRequiredSatisfyTime(RequirementGroup requirements, int time, int delay)
{
	if(!mReservedResources)
	{
		mRequiredSatisfyTime = time;
		mRequiredDelayTime = delay;
	}
	else
		LOGMESSAGE(String_Builder() << "ConstructionTask : " << mType.getName() << " : Required Satisfy Time changed while resources are reserved");

	if(mReservedLocation)
		mReservedLocation->setStartTime(time);
}