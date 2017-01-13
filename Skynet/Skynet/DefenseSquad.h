#pragma once

#include "Interface.h"

#include "BaseSquad.h"
#include "Behaviour.h"

using namespace SkynetBot;

class DefenseSquadTask : public BaseSquadTask
{
public:
	DefenseSquadTask(ArmyBehaviour behaviour);

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

	virtual std::string getTaskName() const { return "Defense Squad"; }
	virtual std::string getOutputName() const { return ""; }

	unsigned int controlSize() { return mUnits.size(); }

	virtual void updateRequirements();

	void setGoal(Goal goal) { mDefenseGoal = goal; }

private:
	UnitGroup mUnits;
	std::map<Unit, Behaviour> mUnitBehaviours;

	int mNeededWorkers;
	bool mNeedsAntiAir;
	bool mNeedsAntiGround;

	UnitGroup mWorkerDefenders;
	Unit mObserver;
	Goal mDefenseGoal;
};

typedef std::tr1::shared_ptr<DefenseSquadTask> DefenseSquadPointer;