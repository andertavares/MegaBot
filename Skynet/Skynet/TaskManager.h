#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Unit.h"
#include "Task.h"
#include "ReservedLocation.h"

class TaskManagerClass
{
public:
	TaskManagerClass();

	void onDiscover(Unit unit);
	void onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType);
	void onDestroy(Unit unit);

	void update();

	void removeTask(Unit unit);
	void giveTask(Unit unit);

	std::map<int, int> earliestFreeTimes(Unit unit, int priority, int lastBlockEnd, int maxTime, int blockTimeNeeded, bool allowAnyBlockLength);
	void reserveUnit(Unit unit, int time, int priority, int duration);

	void addTask(TaskPointer task);

	TaskPointer build(BWAPI::UnitType type, TaskType taskType, BuildingLocation position = BuildingLocation::Base);
	TaskPointer upgrade(BWAPI::UpgradeType type, int level, TaskType taskType);
	TaskPointer research(BWAPI::TechType type, TaskType taskType);

private:
	std::list<TaskType> getPriorityList();

	bool updateRequirement(TaskPointer task, RequirementGroup requirement);

	std::map<BWAPI::UnitType, std::map<Unit, TaskPointer>> mUnitTasks;

	std::map<TaskType, std::list<TaskPointer>> mTypeTasks;
	std::set<TaskPointer> mTasks;

	std::map<Unit, std::map<int, std::pair<int, int>>> mReservedUnits;
};

typedef Singleton<TaskManagerClass> TaskManager;