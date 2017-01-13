#pragma once

#include "Interface.h"
#include "Goal.h"
#include "UnitGroup.h"

class MicroActionBaseClass
{
public:
	MicroActionBaseClass(){}

	virtual bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup) = 0;

	virtual bool hasEnded() = 0;

	virtual void removeUnit(Unit unit) = 0;
};

typedef std::tr1::shared_ptr<MicroActionBaseClass> MicroAction;

class SingleMicroActionBaseClass : public MicroActionBaseClass
{
public:
	SingleMicroActionBaseClass(Unit unit) : MicroActionBaseClass(), mUnit(unit) {}
	SingleMicroActionBaseClass() : MicroActionBaseClass() {}

	virtual bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup) = 0;

	virtual bool hasEnded() { return !mUnit; }

	virtual void removeUnit(Unit unit)
	{
		mUnit = StaticUnits::nullunit;
	}

protected:
	Unit mUnit;
};

typedef std::tr1::shared_ptr<SingleMicroActionBaseClass> SingleMicroAction;

class GroupMicroActionBaseClass : public MicroActionBaseClass
{
public:
	GroupMicroActionBaseClass(UnitGroup units) : MicroActionBaseClass(), mUnits(units) {}
	GroupMicroActionBaseClass(Unit unit) : MicroActionBaseClass() { mUnits.insert(unit); }
	GroupMicroActionBaseClass() : MicroActionBaseClass() {}

	virtual bool update(const Goal &squadGoal, const UnitGroup &squadUnitGroup) = 0;

	virtual bool hasEnded() { return mUnits.empty(); }

	virtual bool finishedWithAUnit() { return false; }
	virtual UnitGroup getFinishedUnits() { return UnitGroup(); }

	virtual void removeUnit(Unit unit)
	{
		mUnits.erase(unit);
	}

	virtual void addUnit(Unit unit)
	{
		mUnits.insert(unit);
	}

	virtual bool requestingExtraUnits() { return false; }

	virtual std::vector<std::map<BWAPI::UnitType, int>> getRequestedUnits() { return std::vector<std::map<BWAPI::UnitType, int>>(); }

protected:
	UnitGroup mUnits;
};

typedef std::tr1::shared_ptr<GroupMicroActionBaseClass> GroupMicroAction;