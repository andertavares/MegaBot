#pragma once

#include "Interface.h"

#include "Task.h"
#include "ReservedLocation.h"

class TaskPump
{
public:
	TaskPump(BWAPI::UnitType type, TaskType taskType, BuildingLocation location = BuildingLocation::Base);

	void update();

	void changeTargetQuantity(int iQuantity) { mTargetQuantity = iQuantity; }

private:
	BWAPI::UnitType mType;
	TaskType mTaskType;
	BuildingLocation mLocation;

	int mTargetQuantity;

	std::list<TaskPointer> mQueuedItems;
};