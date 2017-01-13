#pragma once

#include "Interface.h"

#include "Task.h"
#include "ScoutData.h"
#include "TypeSafeEnum.h"
#include "BuildTilePath.h"
#include "UnitGroup.h"

using namespace SkynetBot;

class WorkerScoutTask : public Task
{
public:
	WorkerScoutTask(TaskType priority, ScoutData data);

	virtual int getEndTime() const;
	virtual int getEndTime(Unit unit) const;

	virtual int getPriority(Unit unit) const;

	virtual Position getStartLocation(Unit unit) const;
	virtual Position getEndLocation(Unit unit) const;

	virtual bool preUpdate();
	virtual bool update();

	virtual void updateRequirements();

	virtual bool waitingForUnit(Unit unit) const;
	virtual void giveUnit(Unit unit);
	virtual void returnUnit(Unit unit);
	virtual bool morph(Unit unit, BWAPI::UnitType previousType);
	virtual UnitGroup getFinishedUnits();

	virtual std::string getTaskName() const { return "Worker Scout"; }
	virtual std::string getOutputName() const { return mCurrentState == ScoutState::Searching ? "Searching" : mCurrentState == ScoutState::Harassing ? "Harassing" : "Double Checking"; }

	bool isFinishedScouting() { return hasEnded() || mCurrentState == ScoutState::Harassing || !mData; }

private:
	Unit mUnit;

	ScoutData mData;
	int mFailTime;
	int mHarrasTimer;

	struct ScoutStateDef
	{
		enum type
		{
			Searching,
			Harassing,
			DoubleCheck
		};
	};
	typedef SafeEnum<ScoutStateDef> ScoutState;

	ScoutState mCurrentState;

	BuildTilePath mTilePath;
	UnitGroup mLastUnitsTargetting;

	void getNewData();
	void calculateFailTime();
};

typedef std::tr1::shared_ptr<WorkerScoutTask> WorkerScoutTaskPointer;