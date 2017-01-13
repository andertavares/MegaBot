#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class ArconZealotKillUnDetected : public SingleMicroActionBaseClass
{
public:
	ArconZealotKillUnDetected(Unit unit) : SingleMicroActionBaseClass(unit) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
};