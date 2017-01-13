#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "GatherTask.h"
#include "Base.h"
#include "TaskPump.h"

class ResourceManagerClass
{
public:
	ResourceManagerClass();

	void update();

	void onDestroy(Unit unit);

	bool isSaturated() { return mSaturated; }
	bool requiresRefineries() { return mGasLevel == 8; }
	int getGasLevel() { return mGasLevel; }

private:
	std::map<Unit, Base> mResourceToBase;
	std::map<Base, std::map<Unit, GatherTaskPointer>> mResourceTasks;
	std::set<Base> mMyActiveBases;

	TaskPump mTaskPump;

	bool mSaturated;
	int mGasLevel;
	int mTimeAllowGasChange;
};

typedef Singleton<ResourceManagerClass> ResourceManager;