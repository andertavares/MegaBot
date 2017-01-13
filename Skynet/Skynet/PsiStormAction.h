#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class PsiStormAction : public SingleMicroActionBaseClass
{
public:
	PsiStormAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};