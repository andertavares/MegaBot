#pragma once

#include "Interface.h"

#include "BaseSquad.h"
#include "Behaviour.h"

using namespace SkynetBot;

class DefaultSquadTask : public BaseSquadTask
{
public:
	DefaultSquadTask(ArmyBehaviour behaviour);

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

	virtual std::string getTaskName() const { return "Default Squad"; }
	virtual std::string getOutputName() const { return ""; }

	unsigned int controlSize() { return mUnits.size(); }

	virtual void updateRequirements();

private:
	UnitGroup mUnits;
	std::map<Unit, Behaviour> mUnitBehaviours;
	int mNumZealots;

	Unit mObserver;

	Goal mLastGoal;
	bool mEngageFull;

	int mFailedBaseAttacks;
};