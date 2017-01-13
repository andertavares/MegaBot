#pragma once

#include "Interface.h"
#include "MicroAction.h"
#include "Goal.h"

using namespace SkynetBot;

class Behaviour
{
public:
	Behaviour(){}
	Behaviour(Unit unit);
	Behaviour(Unit unit, const std::list<MicroAction> &microActions);

	void addMicroAction(MicroAction action);

	void update(const Goal &squadGoal, const UnitGroup &squadUnitGroup);

	void onDeleted();

	void set(Unit unit);
	void set(Unit unit, const std::list<MicroAction> &microActions);

	void createDefaultActions();

private:
	Unit mUnit;

	std::list<MicroAction> mMicroActions;
};