#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class BasicUnitAction : public SingleMicroActionBaseClass
{
public:
	BasicUnitAction(Unit unit, const std::list<std::set<BWAPI::UnitType>> &targetPriorities) : SingleMicroActionBaseClass(unit), mTargetPriorities(targetPriorities) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);

private:
	std::list<std::set<BWAPI::UnitType>> mTargetPriorities;
};