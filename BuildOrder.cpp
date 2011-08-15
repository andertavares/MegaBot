#include "BuildOrder.h"

BuildOrder::BuildOrder(BuildOrderID id, std::string name)
	: mItemCounter(0)
	, mRace(BWAPI::Races::Unknown)
	, mID(id)
	, mName(name)
	, mFallbackBuild(BuildOrderID::None)
	, mFallbackTime(0)
	, mArmyBehaiour(ArmyBehaviour::Default)
{
}

BuildOrder::BuildOrder(BWAPI::Race race, BuildOrderID id, std::string name)
	: mItemCounter(0)
	, mRace(race)
	, mID(id)
	, mName(name)
	, mFallbackBuild(BuildOrderID::None)
	, mFallbackTime(0)
	, mArmyBehaiour(ArmyBehaviour::Default)
{
}

int BuildOrder::addItem(BWAPI::UnitType type, std::list<CallBack> &callBacks, int count, BuildingLocation position)
{
	return addItem(type, TaskType::BuildOrder, callBacks, count, position);
}

int BuildOrder::addItem(BWAPI::TechType type, std::list<CallBack> &callBacks)
{
	return addItem(type, TaskType::BuildOrder, callBacks);
}

int BuildOrder::addItem(BWAPI::UpgradeType type, int level, std::list<CallBack> &callBacks)
{
	return addItem(type, level, TaskType::BuildOrder, callBacks);
}

int BuildOrder::addItem(BWAPI::UnitType type, int count, BuildingLocation position, TaskType taskType)
{
	return addItem(type, taskType, std::list<CallBack>(), count, position);
}

int BuildOrder::addItem(BWAPI::UnitType type, int count, TaskType taskType, BuildingLocation position)
{
	return addItem(type, taskType, std::list<CallBack>(), count, position);
}

int BuildOrder::addItem(BWAPI::UnitType type, TaskType taskType, std::list<CallBack> &callBacks, int count, BuildingLocation position)
{
	mItems.push_back(BuildItem(type, count, ++mItemCounter, taskType, position, callBacks));

	return mItemCounter;
}

int BuildOrder::addItem(BWAPI::TechType type, TaskType taskType, std::list<CallBack> &callBacks)
{
	mItems.push_back(BuildItem(type, ++mItemCounter, taskType, callBacks));

	return mItemCounter;
}

int BuildOrder::addItem(BWAPI::UpgradeType type, int level, TaskType taskType, std::list<CallBack> &callBacks)
{
	mItems.push_back(BuildItem(type, level, ++mItemCounter, taskType, callBacks));

	return mItemCounter;
}

int BuildOrder::addOrder(Order orderType, std::list<CallBack> &callBacks)
{
	mOrders.push_back(OrderItem(orderType, ++mItemCounter, callBacks));

	return mItemCounter;
}

void BuildOrder::addProduce(BWAPI::UnitType type, int weight, int priority, const Condition &unitCond, const Condition &factoryCond)
{
	mProduces.push_back(UnitToProduce(type, weight, priority, unitCond, factoryCond));
}

void BuildOrder::addNextBuild(BuildOrderID id, Condition condition)
{
	mNextBuilds[id] = condition;
}

void BuildOrder::addSquad(SquadType type, int count)
{
	mSquads[type] += count;
}

void BuildOrder::setDefaultBuild(BuildOrderID fallbackBuild, int fallbackTime)
{
	mFallbackBuild = fallbackBuild;
	mFallbackTime = fallbackTime;
}

void BuildOrder::setArmyBehaviour(ArmyBehaviour armyBehaiour)
{
	mArmyBehaiour = armyBehaiour;
}

void BuildOrder::setStartingCondition(Condition condition)
{
	mStartingCondition = condition;
}

std::list<CallBack> CB(int buildID, CallBackType type, std::list<CallBack> &cb)
{
	cb.push_back(CallBack(buildID, type));

	return cb;
}