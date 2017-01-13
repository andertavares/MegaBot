#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Unit.h"
#include "UnitGroup.h"

using namespace SkynetBot;

class UnitInformationClass
{
public:
	UnitInformationClass();

	void update();

	const UnitGroup &getUnitsTargetting(Unit unit) { return mUnitTargets[unit]; }

private:
	std::map<Unit, UnitGroup> mUnitTargets;
};

typedef Singleton<UnitInformationClass> UnitInformation;