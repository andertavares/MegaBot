#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class MineDragAction : public SingleMicroActionBaseClass
{
public:
	MineDragAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};