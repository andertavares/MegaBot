#pragma once

#include "Interface.h"

#include "Task.h"
#include "ScoutData.h"
#include "TypeSafeEnum.h"
#include "BuildTilePath.h"
#include "UnitGroup.h"
#include "Behaviour.h"

using namespace SkynetBot;

class ObserverScoutTask : public Task
{
public:
	ObserverScoutTask(TaskType priority, ScoutData data);

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

	virtual std::string getTaskName() const { return "Observer Scout"; }
	virtual std::string getOutputName() const { return ""; }

private:
	Unit mUnit;
	Behaviour mUnitBehaviour;

	ScoutData mData;
};

typedef std::tr1::shared_ptr<ObserverScoutTask> ObserverScoutTaskPointer;