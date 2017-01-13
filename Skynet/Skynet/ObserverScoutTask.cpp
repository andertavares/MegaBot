#include "ObserverScoutTask.h"
#include "ScoutManager.h"

using namespace SkynetBot;

ObserverScoutTask::ObserverScoutTask(TaskType priority, ScoutData data)
	: Task(priority)
	, mData(data)
{
}

int ObserverScoutTask::getEndTime() const
{
	return Requirement::maxTime;
}

int ObserverScoutTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

int ObserverScoutTask::getPriority(Unit unit) const
{
	return 10;
}

Position ObserverScoutTask::getStartLocation(Unit unit) const
{
	if(mData)
		return mData->getNextPosition(unit->getPosition());

	return unit->getPosition();
}

Position ObserverScoutTask::getEndLocation(Unit unit) const
{
	if(mData)
		return mData->getLastPosition(unit->getPosition());

	return unit->getPosition();
}

bool ObserverScoutTask::preUpdate()
{
	if(mData && mData->isAchieved())
		mData.reset();

	if(mUnit && !mData)
		mData = ScoutManager::Instance().getScoutData(ScoutUnitType::Observer);

	return false;
}

bool ObserverScoutTask::update()
{
	if(mUnit)
	{
		Goal observerGoal;

		if(mData)
			observerGoal = Goal(ActionType::FallBack, mData->getNextPosition(mUnit->getPosition()));

		if(observerGoal.getGoalType() == GoalType::None)
		{
			;//TODO: go to closest cloaked enemy unit
			//TODO: go to closest enemy scv that is in the fog
		}

		mUnitBehaviour.update(observerGoal, UnitGroup());
	}

	return hasEnded() && !mUnit;
}

void ObserverScoutTask::updateRequirements()
{
	clearRequirements();

	if(!mUnit && !shouldReturnUnit())
	{
		RequirementGroup requirementOne;
		if(mData)
			requirementOne.addUnitFilterRequirement(10, Requirement::maxTime, UnitFilter(BWAPI::UnitTypes::Protoss_Observer) && UnitFilter(UnitFilterFlags::IsComplete), mData->getNextPosition());
		else
			requirementOne.addUnitFilterRequirement(10, Requirement::maxTime, UnitFilter(BWAPI::UnitTypes::Protoss_Observer) && UnitFilter(UnitFilterFlags::IsComplete));

		addRequirement(requirementOne);
	}
}

bool ObserverScoutTask::waitingForUnit(Unit unit) const
{
	return false;
}

void ObserverScoutTask::giveUnit(Unit unit)
{
	mUnit = unit;
	mUnitBehaviour.set(unit);
}

void ObserverScoutTask::returnUnit(Unit unit)
{
	if(!mUnit->exists() && mData)
		ScoutManager::Instance().updateLastScoutType(mData, ScoutType::FailedWithAir);

	complete();

	mUnit = StaticUnits::nullunit;
	mUnitBehaviour.onDeleted();
}

bool ObserverScoutTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup ObserverScoutTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	returnUnits.insert(mUnit);

	return returnUnits;
}