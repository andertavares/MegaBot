#pragma once

#include "Interface.h"

#include "Task.h"
#include "BuildingPlacer.h"

using namespace SkynetBot;

class ConstructionTask : public Task
{
public:
	ConstructionTask(TaskType priority, BuildingLocation position, BWAPI::UnitType type);

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

	virtual std::string getTaskName() const { return "Constructing"; }
	virtual std::string getOutputName() const { return mType.getName(); }

	virtual void setRequiredSatisfyTime(RequirementGroup requirements, int time, int delay);

	virtual int dispatchTime() const { return mRequiredSatisfyTime - mRequiredDelayTime; }
	virtual int startTime() const { return mRequiredSatisfyTime; }

	virtual bool hasDispatched() const { return inProgress() || mBuilder; }
	virtual bool inProgress() const { return mProducedUnit && mProducedUnit->exists(); }

private:
	BWAPI::UnitType mType;

	Unit mBuilder;
	Unit mProducedUnit;

	BuildingLocation mBuildingLocation;
	ReservedLocation mReservedLocation;

	int mRequiredSatisfyTime;
	int mRequiredDelayTime;

	bool mReservedResources;

	inline bool finishedWithBuilder() { return shouldReturnUnit() || (inProgress() && mType.getRace() == BWAPI::Races::Protoss); }

	void reserveResources();
	void freeResources();

	void reserveLocation();
	void freeLocation();
};