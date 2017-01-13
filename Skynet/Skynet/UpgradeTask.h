#pragma once

#include "Interface.h"

#include "Task.h"

using namespace SkynetBot;

class UpgradeTask : public Task
{
public:
	UpgradeTask(TaskType priority, BWAPI::UpgradeType type, int level);

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

	virtual std::string getTaskName() const { return "Upgrading"; }
	virtual std::string getOutputName() const { return mType.getName(); }

	virtual void setRequiredSatisfyTime(RequirementGroup requirements, int time, int delay) { mRequiredSatisfyTime = time; }

	virtual int dispatchTime() const { return mRequiredSatisfyTime; }
	virtual int startTime() const { return mRequiredSatisfyTime; }

	virtual bool hasDispatched() const { return mUnit; }
	virtual bool inProgress() const { return mUnit && mUnit->isUpgrading(); }

private:
	BWAPI::UpgradeType mType;
	int mLevel;

	Unit mUnit;

	int mRequiredSatisfyTime;

	bool mReservedResources;
};