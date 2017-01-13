#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "ScoutData.h"
#include "Base.h"
#include "WorkerScoutTask.h"
#include "ObserverScoutTask.h"

class ScoutManagerClass
{
public:
	ScoutManagerClass(){}

	void onBegin();
	void update();

	void updateWorkerScouts();
	void updateObserverScouts();

	void updateLastScoutType(ScoutData data, ScoutType type);

	ScoutData getScoutData(ScoutUnitType unit);

	int getLastScoutTime(Base base) { return mLastScoutTime[base]; }
	ScoutType getLastScoutType(Base base) { return mLastScoutType[base]; }
	ScoutType getCurrentScoutType(Base base) { return mCurrentScoutType[base]; }

private:
	std::set<ScoutData> mAllScoutData;
	std::list<ScoutData> mDataRequiringScout;

	std::map<Base, int> mLastScoutTime;
	std::map<Base, ScoutType> mLastScoutType;
	std::map<Base, ScoutType> mCurrentScoutType;

	std::set<WorkerScoutTaskPointer> mWorkerScouts;
	std::set<ObserverScoutTaskPointer> mObserverScouts;
};

typedef Singleton<ScoutManagerClass> ScoutManager;