#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class TrainInterceptorAction : public SingleMicroActionBaseClass
{
public:
	TrainInterceptorAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};