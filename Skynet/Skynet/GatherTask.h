#pragma once

#include "Interface.h"

#include "Task.h"
#include "Base.h"

using namespace SkynetBot;

class GatherTask : public Task
{
public:
	GatherTask(Unit resource, Base base);

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

	virtual std::string getTaskName() const { return "Gathering"; }
	virtual std::string getOutputName() const { return mResource->getType().getName(); }

	double getMineralRate();
	double getGasRate();

private:
	void updateWorker(Unit worker);

	Base mBase;
	Unit mResource;

	Unit mWorkerOne;
	Unit mWorkerTwo;
	Unit mWorkerThree;

	int workerPriority(int workerNumber) const;
};

typedef std::tr1::shared_ptr<GatherTask> GatherTaskPointer;