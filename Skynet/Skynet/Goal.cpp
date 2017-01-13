#include "Goal.h"

using namespace SkynetBot;

Goal::Goal()
	: mGoalType(GoalType::None)
	, mActionType(ActionType::None)
	, mPosition()
	, mUnit()
	, mBase()
	, mAvoidUnits()
	, mEngageUnits()
{
}

Goal::Goal(ActionType type, Position position, UnitGroup engageUnits, UnitGroup avoidUnits)
	: mGoalType(GoalType::Position)
	, mActionType(type)
	, mPosition(position)
	, mUnit()
	, mBase()
	, mAvoidUnits(avoidUnits)
	, mEngageUnits(engageUnits)
{
}

Goal::Goal(ActionType type, Unit unit, UnitGroup engageUnits, UnitGroup avoidUnits)
	: mGoalType(GoalType::Unit)
	, mActionType(type)
	, mPosition(BWAPI::Positions::None)
	, mUnit(unit)
	, mBase()
	, mAvoidUnits(avoidUnits)
	, mEngageUnits(engageUnits)
{
}

Goal::Goal(ActionType type, UnitGroup engageUnits, UnitGroup avoidUnits)
	: mGoalType(GoalType::UnitGroup)
	, mActionType(type)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mBase()
	, mAvoidUnits(avoidUnits)
	, mEngageUnits(engageUnits)
{
}

Goal::Goal(ActionType type, Base base, UnitGroup engageUnits, UnitGroup avoidUnits)
	: mGoalType(GoalType::Base)
	, mActionType(type)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mBase(base)
	, mAvoidUnits(avoidUnits)
	, mEngageUnits(engageUnits)
{
}

Position Goal::getPositionForMerge() const
{
	switch(mGoalType.underlying())
	{
	case GoalType::Position:
		return mPosition;
	case GoalType::Base:
		return mBase->getCenterLocation();
	case GoalType::Unit:
		return mUnit->getPosition();
	case GoalType::UnitGroup:
		return mEngageUnits.getCenter();
	}

	return Position(BWAPI::Broodwar->mapWidth()*16, BWAPI::Broodwar->mapHeight()*16);
}