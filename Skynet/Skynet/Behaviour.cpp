#include "Behaviour.h"

#include "GoalAction.h"
#include "BasicUnitAction.h"
#include "TrainScarabAction.h"
#include "PsiStormAction.h"
#include "MineDragAction.h"
#include "ArconZealotKillUnDetected.h"
#include "ArbiterAction.h"
#include "DetectorAction.h"
#include "TrainInterceptorAction.h"

using namespace SkynetBot;

Behaviour::Behaviour(Unit unit)
	: mUnit(unit)
{
	createDefaultActions();
}

Behaviour::Behaviour(Unit unit, const std::list<MicroAction> &microActions)
	: mUnit(unit)
	, mMicroActions(microActions)
{
}

void Behaviour::addMicroAction(MicroAction action)
{
	mMicroActions.push_front(action);
}

void Behaviour::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	for each(MicroAction action in mMicroActions)
	{
		if(action->update(squadGoal, squadUnitGroup))
			break;
	}

	for(std::list<MicroAction>::iterator it = mMicroActions.begin(); it != mMicroActions.end();)
	{
		if((*it)->hasEnded())
			mMicroActions.erase(it++);
		else
			++it;
	}
}

void Behaviour::onDeleted()
{
	for each(MicroAction action in mMicroActions)
	{
		action->removeUnit(mUnit);
	}

	mMicroActions.clear();
	mUnit.reset();
}

void Behaviour::set(Unit unit)
{
	mUnit = unit;
	createDefaultActions();
}

void Behaviour::set(Unit unit, const std::list<MicroAction> &microActions)
{
	mUnit = unit;
	mMicroActions = microActions;
}

void Behaviour::createDefaultActions()
{
	const BWAPI::UnitType &unitType = mUnit->getType();

	std::list<std::set<BWAPI::UnitType>> targetPriorities;

	std::set<BWAPI::UnitType> firstTargets;

	if(unitType != BWAPI::UnitTypes::Protoss_Zealot)
		firstTargets.insert(BWAPI::UnitTypes::Terran_Vulture_Spider_Mine);

	if(unitType == BWAPI::UnitTypes::Protoss_Corsair)
		firstTargets.insert(BWAPI::UnitTypes::Zerg_Scourge);

	if(unitType == BWAPI::UnitTypes::Protoss_Zealot || unitType == BWAPI::UnitTypes::Protoss_Archon)
	{
		firstTargets.insert(BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode);
		mMicroActions.push_back(MicroAction(new ArconZealotKillUnDetected(mUnit)));
	}

	targetPriorities.push_back(firstTargets);

	if(unitType.isDetector())
		mMicroActions.push_back(MicroAction(new DetectorAction(mUnit)));

	if(unitType == BWAPI::UnitTypes::Protoss_Arbiter)
		mMicroActions.push_back(MicroAction(new ArbiterAction(mUnit)));

	if(unitType == BWAPI::UnitTypes::Protoss_Reaver)
		mMicroActions.push_back(MicroAction(new TrainScarabAction(mUnit)));
	if(unitType == BWAPI::UnitTypes::Protoss_Carrier)
		mMicroActions.push_back(MicroAction(new TrainInterceptorAction(mUnit)));

	if(unitType == BWAPI::UnitTypes::Protoss_High_Templar)
		mMicroActions.push_back(MicroAction(new PsiStormAction(mUnit)));

	if(unitType == BWAPI::UnitTypes::Protoss_Zealot)
		mMicroActions.push_back(MicroAction(new MineDragAction(mUnit)));

	mMicroActions.push_back(MicroAction(new BasicUnitAction(mUnit, targetPriorities)));
	mMicroActions.push_back(MicroAction(new GoalAction(mUnit)));
}