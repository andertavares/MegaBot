#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class ArbiterAction : public SingleMicroActionBaseClass
{
public:
	ArbiterAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};