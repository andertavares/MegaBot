#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class DetectorAction : public SingleMicroActionBaseClass
{
public:
	DetectorAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};