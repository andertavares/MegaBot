#pragma once

#include "Interface.h"

#include "Goal.h"
#include "Behaviour.h"

using namespace SkynetBot;

class AdvancedGroup
{
public:
	AdvancedGroup();

	void update(Goal goal);

	void addUnit(Unit unit);
	void removeUnits(Unit unit);

private:
	UnitGroup mAllUnits;
	std::map<Unit, Behaviour> mUnitBehaviours;

	Goal mLastGoal;
	UnitGroup mCurrectGoalGroup;

	UnitGroup mReinforcements;

	std::list<std::pair<UnitGroup, Goal>> mStrandedUnits;
};