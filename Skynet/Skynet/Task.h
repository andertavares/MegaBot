#pragma once

#include <boost\tr1\functional.hpp>
#include <boost\tr1\memory.hpp>

#include "Interface.h"
#include <assert.h>

#include "RequirementGroup.h"
#include "TypeSafeEnum.h"
#include "UnitGroup.h"

using namespace SkynetBot;

struct TaskTypeDef
{
	enum type
	{
		Highest = 0,
		BuildOrder,
		Expansion,
		Army,
		MacroUrgent,
		MacroExtraProduction,
		MacroTech,
		Medium,
		Worker,
		Defense,
		Scout,
		Supply,
		RefineryManager,
		Lowest
	};
};
typedef SafeEnum<TaskTypeDef> TaskType;

class Task
{
public:
	Task(TaskType priority)
		: mPaused(false)
		, mStopped(false)
		, mCanceled(false)
		, mCompleted(false)
		, mTaskType(priority)
	{}

	virtual ~Task() {};

	virtual int getEndTime() const = 0;
	virtual int getEndTime(Unit unit) const = 0;

	virtual TaskType getType() const { return mTaskType; }
	virtual int getPriority(Unit unit) const = 0;

	virtual Position getStartLocation(Unit unit) const = 0;
	virtual Position getEndLocation(Unit unit) const = 0;

	// return values are whether it has units to relinquish
	virtual bool preUpdate() = 0;
	virtual bool update() = 0;
	
	virtual bool waitingForUnit(Unit unit) const = 0;
	virtual void giveUnit(Unit unit) = 0;
	virtual void returnUnit(Unit unit) = 0;
	virtual bool morph(Unit unit, BWAPI::UnitType previousType) = 0;
	virtual SkynetBot::UnitGroup getFinishedUnits() = 0;

	virtual std::string getTaskName() const = 0;
	virtual std::string getOutputName() const = 0;

	virtual int dispatchTime() const { return Requirement::maxTime; }
	virtual int startTime() const { return Requirement::maxTime; }

	virtual bool hasDispatched() const { return false; }
	virtual bool inProgress() const { return false; }

	bool isPaused() const { return mPaused; }
	bool isStopped() const { return mStopped; }
	bool isCanceled() const { return mCanceled; }
	bool isCompleted() const { return mCompleted; }
	bool hasEnded() const { return mCompleted || mCanceled; }
	bool shouldReturnUnit() const { return hasEnded() || mStopped || mPaused; }

	void pause() { mPaused = true; }
	void stop() { mStopped = true; mPaused = false; }
	void cancel() { mCanceled = true; }
	void resume() { mPaused = false; mStopped = false; }

	virtual void setRequiredSatisfyTime(RequirementGroup requirements, int time, int delay) {};
	virtual void updateRequirements() {};
	void updateRequirements(std::tr1::function<bool (RequirementGroup)> updateFunction)
	{
		for(std::list<RequirementGroup>::iterator it = mRequirements.begin(); it != mRequirements.end();)
		{
			if(updateFunction(*it))
				it = mRequirements.erase(it);
			else
				++it;
		}
	}

protected:
	void complete() { mCompleted = true; }

	void clearRequirements() { mRequirements.clear(); }
	void addRequirement(RequirementGroup requirement) { mRequirements.push_back(requirement); }

	std::list<RequirementGroup> mRequirements;

private:
	bool mPaused;
	bool mStopped;
	bool mCanceled;
	bool mCompleted;

	TaskType mTaskType;
};

typedef std::tr1::shared_ptr<Task> TaskPointer;