#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class GoalAction : public SingleMicroActionBaseClass
{
public:
	GoalAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};