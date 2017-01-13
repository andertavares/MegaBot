#pragma once

#include "Interface.h"

#include "Task.h"
#include "UnitGroup.h"

class MineBlockingMineralTask : public Task
{
public:
	MineBlockingMineralTask(TaskType priority, UnitGroup minerals);

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

	virtual std::string getTaskName() const { return "Mine Blocking Mineral"; }
	virtual std::string getOutputName() const { return ""; }

private:
	Unit mUnit;
	
	UnitGroup mMinerals;
};