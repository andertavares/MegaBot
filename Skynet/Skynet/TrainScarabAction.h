#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class TrainScarabAction : public SingleMicroActionBaseClass
{
public:
	TrainScarabAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};