#include "AdvancedGroup.h"

using namespace SkynetBot;


void AdvancedGroup::update(Goal goal)
{
	// Reshuffle the groups
	// closest unit to the goal is start of that group

	if(mAllUnits.empty())
		return;

	Unit closestUnit = mAllUnits.getClosestUnit(goal.getPositionForMerge());
	if(mCurrectGoalGroup.count(closestUnit) == 0)
	{
		;
	}

	mLastGoal = goal;
}

void AdvancedGroup::addUnit(Unit unit)
{
	mUnitBehaviours[unit] = Behaviour(unit);
	mReinforcements.insert(unit);
	mAllUnits.insert(unit);
}

void AdvancedGroup::removeUnits(Unit unit)
{
	mCurrectGoalGroup.erase(unit);
	mReinforcements.erase(unit);

	for(std::list<std::pair<UnitGroup, Goal>>::iterator it = mStrandedUnits.begin(); it != mStrandedUnits.end(); ++it)
		it->first.erase(unit);

	mUnitBehaviours[unit].onDeleted();
	mUnitBehaviours.erase(unit);
}