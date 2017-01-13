#include "BuildOrder.h"

#include "GameMemory.h"
#include "Logger.h"
#include "boost/lexical_cast.hpp"

BuildOrder::BuildOrder(BuildOrderID id, std::string name)
	: mItemCounter(0)
	, mRace(BWAPI::Races::Unknown)
	, mID(id)
	, mName(name)
{
}

BuildOrder::BuildOrder(BWAPI::Race race, BuildOrderID id, std::string name)
	: mItemCounter(0)
	, mRace(race)
	, mID(id)
	, mName(name)
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

void BuildOrder::addNextBuild(BuildOrderID id, int timeAfterCompletion, const Condition & condition)
{
	mNextBuilds.push_back(FollowUpBuild(id, timeAfterCompletion, condition));
}

void BuildOrder::addSquad(SquadType type, int count)
{
	mSquads[type] += count;
}

void BuildOrder::addArmyBehaviour(ArmyBehaviour armyBehaiour, std::list<CallBack> &callBacks)
{
	mArmyBehaviours.push_back(ArmyBehaviourItem(armyBehaiour, callBacks));
}

void BuildOrder::setStartingCondition(Condition condition)
{
	mStartingCondition = condition;
}

float BuildOrder::getWinRate(BuildOrderID currentBuild) const
{
	const std::string buildName = String_Builder() << "build_" << currentBuild.underlying() << "_" << mID.underlying();
	const std::vector<std::string> &buildData = GameMemory::Instance().getData(buildName);

	float winRate = 0.92f;

	if(buildData.size() < 2)
		return winRate;

	int gamesWon = boost::lexical_cast<int>(buildData[0]);
	int gamesLost = boost::lexical_cast<int>(buildData[1]);

	int numGames = gamesWon + gamesLost;
	
	if(numGames > 6)
	{
		winRate = (float)gamesWon / (float)numGames;
	}

	return winRate;
}

std::list<CallBack> CB(int buildID, CallBackType type, std::list<CallBack> &cb)
{
	cb.push_back(CallBack(buildID, type));

	return cb;
}