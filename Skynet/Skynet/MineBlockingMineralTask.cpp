#include "MineBlockingMineralTask.h"

using namespace SkynetBot;

MineBlockingMineralTask::MineBlockingMineralTask(TaskType priority, UnitGroup minerals)
	: Task(priority)
	, mMinerals(minerals)
{
}

int MineBlockingMineralTask::getEndTime() const
{
	return Requirement::maxTime;
}

int MineBlockingMineralTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

int MineBlockingMineralTask::getPriority(Unit unit) const
{
	return 35;
}

Position MineBlockingMineralTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position MineBlockingMineralTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool MineBlockingMineralTask::preUpdate()
{
	for(std::set<Unit>::iterator it = mMinerals.begin(); it != mMinerals.end();)
	{
		if((*it)->accessibility() == AccessType::Dead)
			mMinerals.erase(it++);
		else
			++it;
	}

	if(mMinerals.empty())
		complete();

	return hasEnded() && mUnit;
}

bool MineBlockingMineralTask::update()
{
	if(mUnit)
	{
		if(mUnit->isCarryingGas() || mUnit->isCarryingMinerals())
			mUnit->returnCargo();
		else
		{
			Unit closestMineral;
			int closestDistance = std::numeric_limits<int>::max();
			for each(Unit mineral in mMinerals)
			{
				int distance = mineral->getDistance(mUnit);
				if(distance < closestDistance)
				{
					closestMineral = mineral;
					closestDistance = distance;
				}
			}

			if(closestMineral)
				mUnit->gather(closestMineral);
		}
	}

	return hasEnded() && !mUnit;
}

void MineBlockingMineralTask::updateRequirements()
{
	clearRequirements();

	if(!mUnit && !shouldReturnUnit() && !mMinerals.empty())
	{
		RequirementGroup requirementOne;
		requirementOne.addUnitFilterRequirement(35, Requirement::maxTime, UnitFilter(UnitFilterFlags::type(UnitFilterFlags::IsWorker | UnitFilterFlags::IsComplete)), (*mMinerals.begin())->getPosition());

		addRequirement(requirementOne);
	}
}

bool MineBlockingMineralTask::waitingForUnit(Unit unit) const
{
	return false;
}

void MineBlockingMineralTask::giveUnit(Unit unit)
{
	mUnit = unit;
}

void MineBlockingMineralTask::returnUnit(Unit unit)
{
	mUnit = StaticUnits::nullunit;
}

bool MineBlockingMineralTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup MineBlockingMineralTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	returnUnits.insert(mUnit);

	return returnUnits;
}