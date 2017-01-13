#include "TechTask.h"

#include "ResourceTracker.h"

using namespace SkynetBot;

TechTask::TechTask(TaskType priority, BWAPI::TechType type)
	: Task(priority)
	, mType(type)
	, mReservedResources(false)
	, mRequiredSatisfyTime(BWAPI::Broodwar->getFrameCount())
{
}

int TechTask::getEndTime() const
{
	if(mUnit)
		return BWAPI::Broodwar->getFrameCount() + mUnit->getRemainingResearchTime();

	if(mRequiredSatisfyTime == Requirement::maxTime)
		return mRequiredSatisfyTime;

	return mRequiredSatisfyTime + mType.researchTime();
}

int TechTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position TechTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position TechTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool TechTask::preUpdate()
{
	if(BWAPI::Broodwar->self()->hasResearched(mType))
		complete();

	if((isStopped() || isCanceled() || isCompleted()) && mUnit && !mUnit->isResearching())
		return true;

	updateRequirements();

	return false;
}

bool TechTask::update()
{
	if(mUnit)
	{
		if(mUnit->isResearching())
		{
			if(mUnit->getTech() != mType || isStopped() || isPaused() || isCanceled())
				mUnit->cancel();
			else if(mReservedResources)
			{
				ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
				ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
				mReservedResources = false;
			}
		}
		else if(!shouldReturnUnit())
			mUnit->research(mType);
	}

	return hasEnded() && !mUnit;
}

bool TechTask::waitingForUnit(Unit unit) const
{
	return false;
}

void TechTask::giveUnit(Unit unit)
{
	if(unit->getType() == mType.whatResearches())
	{
		mUnit = unit;
		assert(mReservedResources == false);
		ResourceTracker::Instance().reserveCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
		ResourceTracker::Instance().reserveCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
		mReservedResources = true;
	}
}

void TechTask::returnUnit(Unit unit)
{
	if(mUnit == unit)
	{
		mUnit = StaticUnits::nullunit;

		if(mReservedResources)
		{
			ResourceTracker::Instance().releaseCurrentMinerals(mRequiredSatisfyTime, mType.mineralPrice());
			ResourceTracker::Instance().releaseCurrentGas(mRequiredSatisfyTime, mType.gasPrice());
			mReservedResources = false;
		}
	}
	else
		assert(false);
}

bool TechTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup TechTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	if(mUnit)
		returnUnits.insert(mUnit);

	return returnUnits;
}

int TechTask::getPriority(Unit unit) const
{
	return 40;
}

void TechTask::updateRequirements()
{
	clearRequirements();

	if(!mUnit && !shouldReturnUnit())
	{
		RequirementGroup requiredConditions;

		assert(mReservedResources == false);
		requiredConditions.addTechForTypeRequirement(mType);
		requiredConditions.addUnitFilterRequirement(40, mType.researchTime(), UnitFilter(mType.whatResearches()) && UnitFilter(UnitFilterFlags::IsComplete));
		requiredConditions.addMineralRequirement(mType.mineralPrice());
		requiredConditions.addGasRequirement(mType.gasPrice());

		addRequirement(requiredConditions);
	}
}