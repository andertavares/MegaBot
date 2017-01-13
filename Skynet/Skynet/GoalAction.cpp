#include "GoalAction.h"

#include "Vector.h"

using namespace SkynetBot;

bool GoalAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	switch(squadGoal.getGoalType().underlying())
	{
	case GoalType::Base:
		{
			Unit enemy = squadGoal.getBase()->getClosestEnemyBuilding(mUnit->getPosition());
			if(enemy)
			{
				mUnit->move(enemy->getPosition(), 128);
				return true;
			}
			else
			{
				mUnit->move(squadGoal.getBase()->getCenterLocation(), 128);
				return true;
			}
		}
	case GoalType::Position:
		mUnit->move(squadGoal.getPosition(), 128);
		return true;
	case GoalType::Unit:
		mUnit->move(squadGoal.getUnit()->getPosition(), 128);
		return true;
	case GoalType::UnitGroup:
		mUnit->move(squadGoal.getEngageUnits().getClosestUnit(mUnit)->getPosition(), 128);
		return true;
	}
	return false;
}