#pragma once

#include "Interface.h"

#include "Task.h"

using namespace SkynetBot;

class IdleTask : public Task
{
public:
	IdleTask(TaskType priority);

	virtual int getEndTime() const;
	virtual int getEndTime(Unit unit) const;

	virtual int getPriority(Unit unit) const;

	virtual Position getStartLocation(Unit unit) const;
	virtual Position getEndLocation(Unit unit) const;

	virtual bool preUpdate();
	virtual bool update();

	virtual bool waitingForUnit(Unit unit) const;
	virtual void giveUnit(Unit unit);
	virtual void returnUnit(Unit unit);
	virtual bool morph(Unit unit, BWAPI::UnitType previousType);
	virtual UnitGroup getFinishedUnits();

	virtual std::string getTaskName() const { return "Idle"; }
	virtual std::string getOutputName() const { return ""; }

private:
	Unit mUnit;
};