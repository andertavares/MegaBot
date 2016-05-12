#pragma once

#include "Interface.h"
#include "MicroAction.h"

class TrainScarabAction : public SingleMicroActionBaseClass
{
public:
	TrainScarabAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};