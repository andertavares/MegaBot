#include "BuildOrderManager.h"

#include "Logger.h"
#include "MacroManager.h"
#include "SquadManager.h"
#include "GameMemory.h"
#include "Logger.h"

#include <ctime>
#include <boost/random.hpp>
#include <algorithm>
#include "boost/lexical_cast.hpp"

BuildOrderManagerClass::BuildOrderManagerClass()
	: mCurrentBuild(BuildOrderID::None)
	, mFinishedBuild(false)
	, mBuildFinishTime(0)
{
}

void BuildOrderManagerClass::onBegin()
{
	LoadOtherBuilds();
	LoadProtossBuilds();
	LoadTerranBuilds();
	LoadZergBuilds();
}

void BuildOrderManagerClass::onEnd(bool isWinner)
{
	for(std::vector<std::pair<BuildOrderID, BuildOrderID>>::iterator it = mBuildHistory.begin(); it != mBuildHistory.end(); ++it)
	{
		const std::string buildName = String_Builder() << "build_" << it->first.underlying() << "_" << it->second.underlying();
		std::vector<std::string> buildData = GameMemory::Instance().getData(buildName);

		if(buildData.empty())
		{
			buildData.push_back(isWinner ? "1" : "0");
			buildData.push_back(isWinner ? "0" : "1");
		}
		else
		{
			std::string v = buildData[isWinner ? 0 : 1];
			int count = boost::lexical_cast<int>(v);
			++count;
			buildData[isWinner ? 0 : 1] = boost::lexical_cast<std::string>(count);
		}

		GameMemory::Instance().setData(buildName, buildData);
	}
}

void BuildOrderManagerClass::calculateNextBuilds()
{
	mNextBuilds.clear();
	if(mCurrentBuild == BuildOrderID::None)
	{
		for(std::map<BuildOrderID, BuildOrder>::const_iterator it = mBuildOrders.begin(); it != mBuildOrders.end(); ++it)
		{
			if(it->second.getRace() != BWAPI::Broodwar->self()->getRace())
				continue;

			if(it->second.isStartBuild())
				mNextBuilds.push_back(FollowUpBuild(it->first));
		}
	}
	else
	{
		mNextBuilds = mBuildOrders[mCurrentBuild].getNextBuilds();
	}

	std::vector<float> winRates;
	float highestWinRate = 0.0f;
	for(size_t i = 0; i < mNextBuilds.size(); ++i)
	{
		winRates.push_back(mBuildOrders[mNextBuilds[i].getBuildID()].getWinRate(mCurrentBuild));
		highestWinRate = std::max(highestWinRate, winRates.back());
	}

	highestWinRate -= 0.1f;
	if(highestWinRate < 0.0f)
		highestWinRate = 0.0f;

	for(size_t i = 0; i < mNextBuilds.size();)
	{
		if(winRates[i] < highestWinRate)
		{
			std::swap(mNextBuilds[i], mNextBuilds[mNextBuilds.size()-1]);
			mNextBuilds.pop_back();
			std::swap(winRates[i], winRates[winRates.size()-1]);
			winRates.pop_back();
		}
		else
			++i;
	}
}

BuildOrderID BuildOrderManagerClass::getNextBuild() const
{
	if(mNextBuilds.empty())
		return BuildOrderID::Finished;

	std::vector<BuildOrderID> viableBuilds;

	int timeSinceFinished = BWAPI::Broodwar->getFrameCount() - mBuildFinishTime;
	for(size_t i = 0; i < mNextBuilds.size(); ++i)
	{
		if(mNextBuilds[i].getCompletionTime() < timeSinceFinished && mNextBuilds[i].evauluate())
			viableBuilds.push_back(mNextBuilds[i].getBuildID());
	}

	if(viableBuilds.empty())
		return BuildOrderID::None;

	static boost::mt19937 rng(static_cast<unsigned int>(std::time(0)));
	boost::uniform_int<> dist(0, viableBuilds.size() - 1);
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> randIndex(rng, dist);

	return viableBuilds[randIndex()];
}

void BuildOrderManagerClass::update()
{
	if(mCurrentBuild != BuildOrderID::Finished)
	{
		if(BuildOrderFinished())
		{
			if(!mFinishedBuild)
			{
				mBuildFinishTime = BWAPI::Broodwar->getFrameCount();
				mFinishedBuild = true;
				calculateNextBuilds();
			}

			BuildOrderID buildID = getNextBuild();
			if(buildID != BuildOrderID::None)
			{
				changeCurrentBuild(buildID);
			}
		}
	}

	checkBuildStatus();

	if(mShowDebugInfo)
	{
		int y = 25;
		BWAPI::Broodwar->drawTextScreen(5, 5, "Build: %s", mBuildOrders[mCurrentBuild].getName().c_str());
		BWAPI::Broodwar->drawTextScreen(5, 15, "Orders:");
		for(std::map<Order, bool>::iterator it = mControlValues.begin(); it != mControlValues.end(); ++it)
		{
			if(it->second)
			{
				BWAPI::Broodwar->drawTextScreen(5, y, "%s", getOrderName(it->first).c_str());
				y += 10;
			}
		}
	}
}

void BuildOrderManagerClass::BuildCallback(int buildID, CallBackType callbackType)
{
	for(std::list<BuildItem>::iterator buildItem = mItemsWaiting.begin(); buildItem != mItemsWaiting.end();)
	{
		buildItem->removeCallback(buildID, callbackType);

		if(buildItem->isFulfilled())
		{
			handleBuildItem(*buildItem);
			mItemsWaiting.erase(buildItem++);
		}
		else
			++buildItem;
	}

	for(std::list<OrderItem>::iterator orderItem = mOrdersWaiting.begin(); orderItem != mOrdersWaiting.end();)
	{
		orderItem->removeCallback(buildID, callbackType);

		if(orderItem->isFulfilled())
		{
			handleOrderItem(*orderItem);
			mOrdersWaiting.erase(orderItem++);
		}
		else
			++orderItem;
	}

	for(std::list<ArmyBehaviourItem>::iterator armyBehaviourItem = mArmyBehavioursWaiting.begin(); armyBehaviourItem != mArmyBehavioursWaiting.end();)
	{
		armyBehaviourItem->removeCallback(buildID, callbackType);

		if(armyBehaviourItem->isFulfilled())
		{
			handleArmyBehaviourItem(*armyBehaviourItem);
			mArmyBehavioursWaiting.erase(armyBehaviourItem++);
		}
		else
			++armyBehaviourItem;
	}
}

bool BuildOrderManagerClass::BuildOrderFinished()
{
	return mItemsWaiting.empty() && mOrdersWaiting.empty() && mArmyBehavioursWaiting.empty();
}

void BuildOrderManagerClass::changeCurrentBuild(BuildOrderID ID)
{
	if(mBuildOrders.find(ID) == mBuildOrders.end())
	{
		LOGMESSAGEWARNING(String_Builder() << "Couldn't find Build Order");
		changeCurrentBuild(BuildOrderID::Finished);
		return;
	}

	if(mBuildOrders[ID].getRace() != BWAPI::Broodwar->self()->getRace() && mBuildOrders[ID].getRace() != BWAPI::Races::Unknown)
	{
		LOGMESSAGE(String_Builder() << "Build " << mBuildOrders[ID].getName() << " is for " << mBuildOrders[ID].getRace().getName());
		return;
	}

	const BuildOrder &buildOrder = mBuildOrders[ID];

	if(ID != BuildOrderID::Finished)
	{
		LOGMESSAGEWARNING(String_Builder() << ((mCurrentBuild == BuildOrderID::None) ? "Opening with " : "Transitioning to ") << buildOrder.getName());
		mBuildHistory.push_back(std::pair<BuildOrderID, BuildOrderID>(mCurrentBuild, ID));
	}
	else
	{
		LOGMESSAGEWARNING(String_Builder() << "Build finished at " << mBuildOrders[mCurrentBuild].getName());
	}

	mCurrentBuild = ID;

	if(ID != BuildOrderID::Finished)
	{
		mFinishedBuild = false;
		mItemsWaiting.clear();
		mOrdersWaiting.clear();
		mArmyBehavioursWaiting.clear();
		mControlValues.clear();

		for each(const OrderItem &item in buildOrder.getOrderItems())
		{
			if(item.isFulfilled())
				handleOrderItem(item);
			else
				mOrdersWaiting.push_back(item);
		}

		for each(const BuildItem &item in buildOrder.getBuildItems())
		{
			if(item.isFulfilled())
				handleBuildItem(item);
			else
				mItemsWaiting.push_back(item);
		}

		for each(const ArmyBehaviourItem &item in buildOrder.getArmyBehaviourItems())
		{
			if(item.isFulfilled())
				handleArmyBehaviourItem(item);
			else
				mArmyBehavioursWaiting.push_back(item);
		}

		MacroManager::Instance().onChangeBuild();
		SquadManager::Instance().onChangeBuild();
	}
}

void BuildOrderManagerClass::handleOrderItem(const OrderItem &item)
{
	toggleOrder(item.getType());

	LOGMESSAGE(String_Builder() << "Handled Order " << getOrderName(item.getType()) << (getOrder(item.getType()) ? ": Set to True" : ": Set to False"));
}

void BuildOrderManagerClass::handleArmyBehaviourItem(const ArmyBehaviourItem &item)
{
	SquadManager::Instance().setBehaviour(item.getArmyBehaviour());
}

void BuildOrderManagerClass::toggleOrder(Order type)
{
	if(mControlValues.find(type) != mControlValues.end())
	{
		mControlValues[type] = !mControlValues[type];
	}
	else
		mControlValues[type] = true;
}

void BuildOrderManagerClass::checkBuildStatus()
{
	for(std::map<CallBackType, std::map<TaskPointer, int>>::iterator i = mWaitingItems.begin(); i != mWaitingItems.end();)
	{
		for(std::map<TaskPointer, int>::iterator u = i->second.begin(); u != i->second.end();)
		{
			if(i->first == CallBackType::onDispatched && u->first->hasDispatched())
			{
				BuildCallback(u->second, CallBackType::onDispatched);
				i->second.erase(u++);
			}
			else if(i->first == CallBackType::onStarted && u->first->inProgress())
			{
				BuildCallback(u->second, CallBackType::onStarted);
				i->second.erase(u++);
			}
			else if(i->first == CallBackType::onCompleted && u->first->isCompleted())
			{
				BuildCallback(u->second, CallBackType::onCompleted);
				i->second.erase(u++);
			}
			else
				++u;
		}

		if(i->second.empty())
			mWaitingItems.erase(i++);
		else
			++i;
	}
}

void BuildOrderManagerClass::handleBuildItem(const BuildItem &item)
{
	TaskPointer ptr = item.createTask();
	if(ptr)
	{
		mWaitingItems[CallBackType::onDispatched][ptr] = item.getID();
		mWaitingItems[CallBackType::onStarted][ptr] = item.getID();
		mWaitingItems[CallBackType::onCompleted][ptr] = item.getID();
	}
	else
	{
		BuildCallback(item.getID(), CallBackType::onDispatched);
		BuildCallback(item.getID(), CallBackType::onStarted);
		BuildCallback(item.getID(), CallBackType::onCompleted);
	}
}

std::string BuildOrderManagerClass::getOrderName(Order type)
{
	switch(type.underlying())
	{
	case Order::TrainWorkers:
		return "Train Workers";
	case Order::Scout:
		return "Scout";
	case Order::SupplyManager:
		return "Supply Manager";
	case Order::RefineryManager:
		return "Refinery Manager";
	case Order::MacroArmyProduction:
		return "Army Train Manager";
	case Order::CanRemoveSquads:
		return "Can Remove Squads";
	case Order::ExpansionManager:
		return "Expansion Manager";
	case Order::MacroProductionFacilities:
		return "Macro : Can Produce Production Facilities";
	case Order::MacroCanTech:
		return "Macro : Can Tech";
	}
	return "None";
}