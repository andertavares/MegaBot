#include "TaskPump.h"

#include "TaskManager.h"

TaskPump::TaskPump(BWAPI::UnitType type, TaskType taskType, BuildingLocation location)
	: mType(type)
	, mTaskType(taskType)
	, mLocation(location)
	, mTargetQuantity(0)
{
}

void TaskPump::update()
{
	for(std::list<TaskPointer>::iterator it = mQueuedItems.begin(); it != mQueuedItems.end();)
	{
		if((*it)->hasEnded())
			mQueuedItems.erase(it++);
		else
			++it;
	}

	int currentTotal = mQueuedItems.size();
	for(std::list<TaskPointer>::iterator it2 = mQueuedItems.begin(); it2 != mQueuedItems.end();)
	{
		if(currentTotal > mTargetQuantity && !(*it2)->inProgress())
		{
			(*it2)->cancel();
			mQueuedItems.erase(it2++);
			--currentTotal;
		}
		else
			++it2;
	}

	while(currentTotal > mTargetQuantity && !mQueuedItems.empty())
	{
		(*mQueuedItems.rbegin())->cancel();
		mQueuedItems.pop_back();
		--currentTotal;
	}

	while(currentTotal < mTargetQuantity)
	{
		TaskPointer tempPtr = TaskManager::Instance().build(mType, mTaskType, mLocation);

		mQueuedItems.push_back(tempPtr);

		++currentTotal;
	}
}