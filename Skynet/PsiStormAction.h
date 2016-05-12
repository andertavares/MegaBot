#pragma once

#include "Interface.h"
#include "MicroAction.h"

class PsiStormAction : public SingleMicroActionBaseClass
{
public:
	PsiStormAction(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};