#pragma once

#include "Interface.h"
#include "MicroAction.h"

using namespace SkynetBot;

class DropAction : public GroupMicroActionBaseClass
{
public:
	DropAction(Unit unit) : GroupMicroActionBaseClass(unit), mShuttle(unit), mFreeSpace(mShuttle->getType().spaceProvided()) {}

	bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);
	bool hasEnded() { return !mShuttle; }
	bool finishedWithAUnit();
	UnitGroup getFinishedUnits();
	
	void removeUnit(Unit unit);
	void addUnit(Unit unit);

	bool requestingExtraUnits();
	std::vector<std::map<BWAPI::UnitType, int>> getRequestedUnits();

private:
	Unit mShuttle;

	int mFreeSpace;
};