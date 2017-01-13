#include "GatherTask.h"

#include "ResourceManager.h"

using namespace SkynetBot;

GatherTask::GatherTask(Unit resource, Base base)
	: Task(TaskType::Medium)
	, mResource(resource)
	, mBase(base)
{
}

int GatherTask::getEndTime() const
{
	return Requirement::maxTime;
}

int GatherTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position GatherTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position GatherTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool GatherTask::preUpdate()
{
	if(hasEnded())
		return true;

	updateRequirements();

	return false;
}

bool GatherTask::update()
{
	if(mWorkerOne)
		updateWorker(mWorkerOne);
	if(mWorkerTwo)
		updateWorker(mWorkerTwo);
	if(mWorkerThree)
		updateWorker(mWorkerThree);

	return hasEnded() && !mWorkerOne && !mWorkerTwo && !mWorkerThree;
}

bool GatherTask::waitingForUnit(Unit unit) const
{
	return false;
}

void GatherTask::giveUnit(Unit unit)
{
	if(!mWorkerOne)
		mWorkerOne = unit;
	else if(!mWorkerTwo)
		mWorkerTwo = unit;
	else if(!mWorkerThree)
		mWorkerThree = unit;
	else
		assert(false);
}

void GatherTask::returnUnit(Unit unit)
{
	if(mWorkerOne == unit)
	{
		mWorkerOne = mWorkerTwo;
		mWorkerTwo = mWorkerThree;
		mWorkerThree = StaticUnits::nullunit;
	}
	else if(mWorkerTwo == unit)
	{
		mWorkerTwo = mWorkerThree;
		mWorkerThree = StaticUnits::nullunit;
	}
	else if(mWorkerThree == unit)
	{
		mWorkerThree = StaticUnits::nullunit;
	}
	else
		assert(false);
}

bool GatherTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup GatherTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	if(mWorkerOne)
		returnUnits.insert(mWorkerOne);
	if(mWorkerTwo)
		returnUnits.insert(mWorkerTwo);
	if(mWorkerThree)
		returnUnits.insert(mWorkerThree);

	return returnUnits;
}

int GatherTask::getPriority(Unit unit) const
{
	if(!mWorkerOne || unit == mWorkerOne)
		return workerPriority(1);
	else if(!mWorkerTwo || unit == mWorkerTwo)
		return workerPriority(2);
	else if(!mWorkerThree || unit == mWorkerThree)
		return workerPriority(3);

	assert(false);
	return 10;
}

void GatherTask::updateWorker(Unit worker)
{
	if(worker->isCarryingGas() || worker->isCarryingMinerals())
	{
		Unit resourceDepot = mBase->getResourceDepot();
		if(resourceDepot)
			worker->returnCargo(resourceDepot);
	}
	else
		worker->gather(mResource);
}

void GatherTask::updateRequirements()
{
	clearRequirements();

	//TODO: any worker will do that i can control, dont limit to my races workers
	Unit resourceDepot = mBase->getResourceDepot();
	if(resourceDepot)
	{
		int completeTime = 0;
		if(!resourceDepot->isCompleted())
			completeTime = resourceDepot->getCompletedTime();

		if(mResource->getType().isRefinery() && !mResource->isCompleted())
			completeTime = std::max(completeTime, mResource->getCompletedTime());

		if(!mWorkerOne)
		{
			RequirementGroup requirementOne;
			requirementOne.addUnitFilterRequirement(workerPriority(1), Requirement::maxTime, UnitFilter(UnitFilterFlags::IsWorker) && UnitFilter(UnitFilterFlags::IsComplete) && UnitFilter(UnitPositionFlags::CanTravelSafely, mResource->getPosition()), mResource->getPosition());
			if(completeTime > 0)
				requirementOne.addTimeRequirement(completeTime);
			addRequirement(requirementOne);
		}

		if(!mWorkerTwo)
		{
			RequirementGroup requirementTwo;
			requirementTwo.addUnitFilterRequirement(workerPriority(2), Requirement::maxTime, UnitFilter(UnitFilterFlags::IsWorker) && UnitFilter(UnitFilterFlags::IsComplete) && UnitFilter(UnitPositionFlags::CanTravelSafely, mResource->getPosition()), mResource->getPosition());
			if(completeTime > 0)
				requirementTwo.addTimeRequirement(completeTime);
			addRequirement(requirementTwo);
		}

		if(!mWorkerThree)
		{
			RequirementGroup requirementThree;
			requirementThree.addUnitFilterRequirement(workerPriority(3), Requirement::maxTime, UnitFilter(UnitFilterFlags::IsWorker) && UnitFilter(UnitFilterFlags::IsComplete) && UnitFilter(UnitPositionFlags::CanTravelSafely, mResource->getPosition()), mResource->getPosition());
			if(completeTime > 0)
				requirementThree.addTimeRequirement(completeTime);
			addRequirement(requirementThree);
		}
	}
}

double GatherTask::getMineralRate()
{
	if(mResource->getType().isRefinery())
		return 0.0;

	if(mWorkerThree)
		return 3 * 8/180.0;

	if(mWorkerTwo)
		return 2 * 8/180.0;

	if(mWorkerOne)
		return 8/180.0;

	return 0.0;
}

double GatherTask::getGasRate()
{
	if(!mResource->getType().isRefinery())
		return 0.0;

	if(mWorkerThree)
		return 3 * 8/180.0;

	if(mWorkerTwo)
		return 2 * 8/180.0;

	if(mWorkerOne)
		return 8/180.0;

	return 0.0;
}

int GatherTask::workerPriority(int workerNumber) const
{
	if(mResource->getType().isRefinery())
	{
		int gLevel = ResourceManager::Instance().getGasLevel();
		if(gLevel == 8)
			return 25;
		else if(workerNumber == 1)
		{
			if(gLevel > 3)
				return 25;
			else if(gLevel > 2)
				return 20;
			else if(gLevel > 1)
				return 15;
			else if(gLevel > 0)
				return 10;
			else
				return 0;
		}
		else if(workerNumber == 2)
		{
			if(gLevel > 5)
				return 25;
			else if(gLevel > 4)
				return 20;
			else if(gLevel > 3)
				return 15;
			else if(gLevel > 2)
				return 10;
			else
				return 0;
		}
		else if(workerNumber == 3)
		{
			if(gLevel > 7)
				return 25;
			else if(gLevel > 6)
				return 20;
			else if(gLevel > 5)
				return 15;
			else if(gLevel > 4)
				return 10;
			else
				return 0;
		}

	}
	else if(workerNumber == 1)
		return 20;
	else if(workerNumber == 2)
		return 15;
	else if(workerNumber == 3)
		return 10;

	return 0;
}