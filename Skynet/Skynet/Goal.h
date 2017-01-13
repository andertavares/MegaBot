#pragma once

#include "Interface.h"
#include "TypeSafeEnum.h"
#include "UnitGroup.h"
#include "Base.h"
#include "AOEThreat.h"

using namespace SkynetBot;

struct GoalTypeDef
{
	enum type
	{
		None,
		Position,
		Unit,
		UnitGroup,
		Base
	};
};
typedef SafeEnum<GoalTypeDef> GoalType;

struct ActionTypeDef
{
	enum type
	{
		None,
		Defend,
		Hold,
		FallBack,
		Retreat,
		Attack
	};
};
typedef SafeEnum<ActionTypeDef> ActionType;

class Goal
{
public:
	Goal();
	Goal(ActionType type, Position position, UnitGroup engageUnits = UnitGroup(), UnitGroup avoidUnits = UnitGroup());
	Goal(ActionType type, Unit unit, UnitGroup engageUnits = UnitGroup(), UnitGroup avoidUnits = UnitGroup());
	Goal(ActionType type, UnitGroup engageUnits, UnitGroup avoidUnits = UnitGroup());
	Goal(ActionType type, Base base, UnitGroup engageUnits = UnitGroup(), UnitGroup avoidUnits = UnitGroup());

	GoalType getGoalType() const { return mGoalType; }
	ActionType getActionType() const { return mActionType; }

	const Position &getPosition() const { return mPosition; }
	const Unit &getUnit() const { return mUnit; }
	const Base &getBase() const { return mBase; }
	const UnitGroup &getAvoidUnits() const { return mAvoidUnits; }
	const UnitGroup &getEngageUnits() const { return mEngageUnits; }

	Position getPositionForMerge() const;

private:
	GoalType mGoalType;
	ActionType mActionType;

	Position mPosition;
	Unit mUnit;
	Base mBase;

	UnitGroup mAvoidUnits;
	UnitGroup mEngageUnits;
};