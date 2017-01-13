#include "TaskManager.h"

#include <boost/bind.hpp>

#include "UnitGroup.h"
#include "ResourceTracker.h"
#include "IdleTask.h"
#include "Logger.h"

#include "MorphTask.h"
#include "ConstructionTask.h"
#include "TrainTask.h"
#include "UpgradeTask.h"
#include "TechTask.h"
#include "UnitTracker.h"
#include "BuildOrderManager.h"
#include "MacroManager.h"
#include "GameProgressDetection.h"

using namespace SkynetBot;

TaskManagerClass::TaskManagerClass()
{
}

void TaskManagerClass::update()
{
	for each(const TaskPointer &task in mTasks)
	{
		if(task->preUpdate())
		{
			for each(Unit unit in task->getFinishedUnits())
				removeTask(unit);
		}
	}

	// Clear previous reservations
	ResourceTracker::Instance().reset();
	mReservedUnits.clear();

	for each(TaskType type in getPriorityList())
	{
		std::map<TaskType, std::list<TaskPointer>>::iterator l = mTypeTasks.find(type);

		if(l != mTypeTasks.end())
		{
			for(std::list<TaskPointer>::iterator i = l->second.begin(); i != l->second.end(); ++i)
			{
				if((*i)->hasEnded())
					continue;

				(*i)->updateRequirements(boost::bind<bool>(&TaskManagerClass::updateRequirement, this, *i, _1));
			}
		}
	}

	for(std::map<TaskType, std::list<TaskPointer>>::iterator itFirst = mTypeTasks.begin(); itFirst != mTypeTasks.end(); ++itFirst)
	{
		for(std::list<TaskPointer>::iterator it = itFirst->second.begin(); it != itFirst->second.end();)
		{
			if((*it)->update())
			{
				//TODO: shouldn't have to but don't delete if it has control of units or just assert
				/*
				int count = 0;
				for(std::map<BWAPI::UnitType, std::map<Unit, TaskPointer>>::iterator it2 = mUnitTasks.begin(); it2 != mUnitTasks.end(); ++it2)
				{
					for(std::map<Unit, TaskPointer>::iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
					{
						if(it3->second == *it)
						{
							++count;
						}
					}
				}

				if(count > 0)
				{
					LOGMESSAGE(String_Builder() << "TaskManager : Task " << (*it)->getTaskName() << " " << (*it)->getOutputName() << " deleted while still controlling " << count << " units");
				}
				*/

				mTasks.erase(*it);
				itFirst->second.erase(it++);
			}
			else
				++it;
		}
	}
}

std::list<TaskType> TaskManagerClass::getPriorityList()
{
	std::list<TaskType> returnList;

	//Thing to consider for sorts these:
	// If I am vulnerable to counter attack / have no map control, place defense higher
	// If I am not behind on army size but its not safe to attack, tech
	// If I am not behind on army size but its safe to attack, produce
	// If I'm behind on army supply, produce

	StateType currentState = GameProgressDetection::Instance().getState();
	
	returnList.push_back(TaskType::Highest);
	returnList.push_back(TaskType::Supply);
	returnList.push_back(TaskType::Worker);
	returnList.push_back(TaskType::RefineryManager);
	returnList.push_back(TaskType::MacroUrgent);
	returnList.push_back(TaskType::Scout);
	returnList.push_back(TaskType::Expansion);

	if(currentState == StateType::TechHigh && BuildOrderManager::Instance().getOrder(Order::MacroCanTech))
	{
		returnList.push_back(TaskType::BuildOrder);
		returnList.push_back(TaskType::MacroTech);
		returnList.push_back(TaskType::Defense);
		returnList.push_back(TaskType::Army);
	}
	else
	{
		returnList.push_back(TaskType::Army);
		returnList.push_back(TaskType::Defense);
		returnList.push_back(TaskType::BuildOrder);
	}
	
	returnList.push_back(TaskType::Medium);

	if(currentState == StateType::TechNormal && BuildOrderManager::Instance().getOrder(Order::MacroCanTech))
		returnList.push_back(TaskType::MacroTech);

	returnList.push_back(TaskType::MacroExtraProduction);

	if(currentState == StateType::BuildArmy && BuildOrderManager::Instance().getOrder(Order::MacroCanTech))
		returnList.push_back(TaskType::MacroTech);

	returnList.push_back(TaskType::Lowest);

	return returnList;
}

void TaskManagerClass::onDiscover(Unit unit)
{
	if(unit->getPlayer() != BWAPI::Broodwar->self())
		return;

	giveTask(unit);
}

void TaskManagerClass::onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType)
{
	if(previousPlayer != NULL)
	{
		if(previousPlayer == BWAPI::Broodwar->self())
		{
			if(previousType != BWAPI::UnitTypes::None)
			{
				mUnitTasks[previousType][unit]->returnUnit(unit);
				if(!mUnitTasks[previousType][unit]->hasEnded())
					mUnitTasks[previousType][unit]->updateRequirements();
				mUnitTasks[previousType].erase(unit);
			}
			else
			{
				mUnitTasks[unit->getType()][unit]->returnUnit(unit);
				if(!mUnitTasks[unit->getType()][unit]->hasEnded())
					mUnitTasks[unit->getType()][unit]->updateRequirements();
				mUnitTasks[unit->getType()].erase(unit);
			}
		}
		else if(unit->getPlayer() == BWAPI::Broodwar->self())
		{
			giveTask(unit);
		}
	}
	else if(previousType != BWAPI::UnitTypes::None && unit->getPlayer() == BWAPI::Broodwar->self())
	{
		if(mUnitTasks[previousType][unit]->morph(unit, previousType))
		{
			mUnitTasks[previousType][unit]->returnUnit(unit);
			if(!mUnitTasks[previousType][unit]->hasEnded())
				mUnitTasks[previousType][unit]->updateRequirements();
			mUnitTasks[previousType].erase(unit);

			if(unit->accessibility() == AccessType::Full)
				giveTask(unit);
		}
		else
		{
			TaskPointer task = mUnitTasks[previousType][unit];
			mUnitTasks[previousType].erase(unit);
			mUnitTasks[unit->getType()][unit] = task;
		}
	}
}

void TaskManagerClass::onDestroy(Unit unit)
{
	removeTask(unit);
}

void TaskManagerClass::removeTask(Unit unit)
{
	if(mUnitTasks[unit->getType()].find(unit) == mUnitTasks[unit->getType()].end())
		return;

	LOGMESSAGE(String_Builder() << "TaskManager : Removed Unit " << unit->getType().getName() << " from " << mUnitTasks[unit->getType()][unit]->getTaskName() << " " << mUnitTasks[unit->getType()][unit]->getOutputName());

	mUnitTasks[unit->getType()][unit]->returnUnit(unit);
	if(!mUnitTasks[unit->getType()][unit]->hasEnded())
		mUnitTasks[unit->getType()][unit]->updateRequirements();
	mUnitTasks[unit->getType()].erase(unit);

	if(unit->accessibility() == AccessType::Full)
		giveTask(unit);
}

void TaskManagerClass::giveTask(Unit unit)
{
	TaskPointer bestTask;
	int priority = 0;

	for each(TaskPointer task in mTasks)
	{
		if(task->waitingForUnit(unit))
		{
			int thisPriority = task->getPriority(unit);
			if(thisPriority > priority)
			{
				priority = thisPriority;
				bestTask = task;
			}
		}
	}

	if(bestTask)
	{
		LOGMESSAGE(String_Builder() << "TaskManager : Given new unit " << unit->getType().getName() << " to " << bestTask->getTaskName() << " " << bestTask->getOutputName());
		mUnitTasks[unit->getType()][unit] = bestTask;
		bestTask->giveUnit(unit);
	}
	else
	{
		LOGMESSAGE(String_Builder() << "TaskManager : Created idle task for " << unit->getType().getName());
		TaskPointer idleTask(new IdleTask(TaskType::Lowest));
		mUnitTasks[unit->getType()][unit] = idleTask;
		mTasks.insert(idleTask);
		mTypeTasks[idleTask->getType()].push_back(idleTask);
		idleTask->giveUnit(unit);
	}
}

std::map<int, int> TaskManagerClass::earliestFreeTimes(Unit unit, int priority, int lastBlockEnd, int maxTime, int blockTimeNeeded, bool allowAnyBlockLength)
{
	std::map<int, int> suitableBlocks;

	// test this priority against the current task to test if we can override it
	TaskPointer currentTask = mUnitTasks[unit->getType()][unit];
	if(!currentTask)
	{
		LOGMESSAGE(String_Builder() << "TaskManager : Tried to access task for unit " << unit->getType().getName() << " that we don't control");
		return suitableBlocks;
	}

	if(priority <= currentTask->getPriority(unit))
	{
		// If we can't get the end time of the task
		int endTime = currentTask->getEndTime(unit);
		int frametime = BWAPI::Broodwar->getFrameCount();
		assert(endTime >= frametime);

		// Get the end time of the current task
		lastBlockEnd = std::max(endTime == frametime ? endTime+1 : endTime, lastBlockEnd);
	}

	// If it has no end return nothing
	if(lastBlockEnd == Requirement::maxTime)
		return suitableBlocks;

	int currentFreeBlockStart = lastBlockEnd;
	int currentFreeBlockLength = 0;

	for(std::map<int, std::pair<int, int>>::iterator i = mReservedUnits[unit].begin(); i != mReservedUnits[unit].end(); ++i)
	{
		// This is before our current time slot so we don't need to worry about it
		if(i->first + i->second.second < lastBlockEnd)
			continue;

		// this is after our current time so we should return
		if(i->first > maxTime)
			return suitableBlocks;

		// We can move this block if we are higher priority
		bool canMoveThisBlock = priority > i->second.first;

		// How long the current block of time we have to fit in is
		currentFreeBlockLength += i->first - lastBlockEnd;
		
		// How long will this task last
		lastBlockEnd = i->second.second;

		// If it will last forever....
		if(lastBlockEnd == Requirement::maxTime)
		{
			// If we can move this block, steal its time
			if(canMoveThisBlock)
				suitableBlocks[currentFreeBlockStart] = Requirement::maxTime;
			
			//return as there is no more time to work with
			return suitableBlocks;
		}
		
		// add the start time so we get the real end
		lastBlockEnd += i->first;

		// if we cannot move this block
		if(!canMoveThisBlock)
		{
			// If its long enough, or we are looking we are looking for an endless slot
			if(currentFreeBlockLength >= blockTimeNeeded || allowAnyBlockLength)
			{
				suitableBlocks[currentFreeBlockStart] = currentFreeBlockLength;
			}

			// reset the block length
			currentFreeBlockLength = 0;
			currentFreeBlockStart = lastBlockEnd;
		}
	}

	// If we made it this far we have the rest of the time for this task
	suitableBlocks[lastBlockEnd] = Requirement::maxTime;

	return suitableBlocks;
}

void TaskManagerClass::reserveUnit(Unit unit, int time, int priority, int duration)
{
	// TODO: change from a map to a sorted vector or sumin, should be faster at updating times
	mReservedUnits[unit][time] = std::pair<int, int>(priority, duration);

	int lastTaskEndTime = 0;
	for(std::map<int, std::pair<int, int>>::iterator i = mReservedUnits[unit].begin(); i != mReservedUnits[unit].end();)
	{
		if(lastTaskEndTime > i->first)
		{
			std::pair<int, int> tempTask = i->second;
			mReservedUnits[unit].erase(i++);
			mReservedUnits[unit][lastTaskEndTime] = tempTask;

			lastTaskEndTime += tempTask.second;
		}
		else
		{
			lastTaskEndTime = i->first + i->second.second;
			++i;
		}
	}
}

void TaskManagerClass::addTask(TaskPointer task)
{
	LOGMESSAGE(String_Builder() << "TaskManager : New task added : " << task->getTaskName() << " " << task->getOutputName());

	mTypeTasks[task->getType()].push_back(task);
	mTasks.insert(task);

	if(!task->hasEnded())
		task->updateRequirements();
}

bool TaskManagerClass::updateRequirement(TaskPointer task, RequirementGroup requirement)
{
	if(!requirement.empty())
	{
		std::pair<int, int> earliestTime = requirement.earliestTime();
		task->setRequiredSatisfyTime(requirement, earliestTime.first, earliestTime.second);

		if(earliestTime.first - earliestTime.second <= BWAPI::Broodwar->getFrameCount())
		{
			for each(Unit unit in requirement.getUnits())
			{
				TaskPointer oldTask = mUnitTasks[unit->getType()][unit];
				oldTask->returnUnit(unit);
				if(!oldTask->hasEnded())
					oldTask->updateRequirements();

				mUnitTasks[unit->getType()][unit] = task;

				LOGMESSAGE(String_Builder() << "TaskManager : Taken " << unit->getType().getName() << " from " << oldTask->getTaskName() << " " << oldTask->getOutputName());
				LOGMESSAGE(String_Builder() << "TaskManager : Given " << unit->getType().getName() << " to " << task->getTaskName() << " " << task->getOutputName());
				task->giveUnit(unit);
			}

			return true;
		}
		else
		{
			requirement.reserve(earliestTime.first);
			//TODO see what can be done to make this task happen sooner
		}
	}

	return false;
}

TaskPointer TaskManagerClass::build(BWAPI::UnitType type, TaskType taskType, BuildingLocation position)
{
	if(type == BWAPI::UnitTypes::None || type == BWAPI::UnitTypes::Unknown)
		return TaskPointer();

	if(type.getRace() == BWAPI::Races::Zerg && type.isBuilding() == type.whatBuilds().first.isBuilding())
	{
		TaskPointer returnTask = TaskPointer(new MorphTask(taskType, type));
		addTask(returnTask);
		MacroManager::Instance().onBuildTask(returnTask, type);
		return returnTask;
	}
	else
	{
		if(type.isBuilding())
		{
			if(type.isAddon())
			{
				return TaskPointer(); // NYI
			}
			else
			{
				TaskPointer returnTask = TaskPointer(new ConstructionTask(taskType, position, type));
				addTask(returnTask);
				MacroManager::Instance().onBuildTask(returnTask, type);
				return returnTask;
			}
		}
		else
		{
			TaskPointer returnTask = TaskPointer(new TrainTask(taskType, type));
			addTask(returnTask);
			MacroManager::Instance().onBuildTask(returnTask, type);
			return returnTask;
		}
	}

	return TaskPointer();
}

TaskPointer TaskManagerClass::upgrade(BWAPI::UpgradeType type, int level, TaskType taskType)
{
	TaskPointer returnTask = TaskPointer(new UpgradeTask(taskType, type, level));
	addTask(returnTask);
	MacroManager::Instance().onUpgradeTask(returnTask, type, level);
	return returnTask;
}

TaskPointer TaskManagerClass::research(BWAPI::TechType type, TaskType taskType)
{
	TaskPointer returnTask = TaskPointer(new TechTask(taskType, type));
	addTask(returnTask);
	MacroManager::Instance().onTechTask(returnTask, type);
	return returnTask;
}