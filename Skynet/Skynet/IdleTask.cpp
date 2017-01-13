#include "IdleTask.h"

using namespace SkynetBot;

IdleTask::IdleTask(TaskType priority)
: Task(priority)
{
}

int IdleTask::getEndTime() const
{
	return Requirement::maxTime;
}

int IdleTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position IdleTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position IdleTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool IdleTask::preUpdate()
{
	if(!mUnit)
		complete();

	return false;
}

bool IdleTask::update()
{
	return hasEnded() && !mUnit;
}

bool IdleTask::waitingForUnit(Unit unit) const
{
	return false;
}

void IdleTask::giveUnit(Unit unit)
{
	assert(mUnit == StaticUnits::nullunit);
	mUnit = unit;
}

void IdleTask::returnUnit(Unit unit)
{
	assert(mUnit == unit);
	mUnit = StaticUnits::nullunit;
}

bool IdleTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup IdleTask::getFinishedUnits()
{
	assert(false);
	return UnitGroup();
}

int IdleTask::getPriority(Unit unit) const
{
	return 0;
}