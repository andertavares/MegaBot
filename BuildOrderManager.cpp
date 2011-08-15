#include "BuildOrderManager.h"

#include "Logger.h"
#include "MacroManager.h"
#include "SquadManager.h"

#include <ctime>
#include <boost/random.hpp>

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

BuildOrderID getRandomBuild(const std::vector<BuildOrderID> &builds)
{
	if(builds.empty())
		return BuildOrderID::Unknown;

	static boost::mt19937 rng(static_cast<unsigned int>(std::time(0)));
	boost::uniform_int<> dist(0, builds.size() - 1);
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> randIndex(rng, dist);

	return builds[randIndex()];
}

void BuildOrderManagerClass::update()
{
	if(mCurrentBuild == BuildOrderID::None)
	{
		std::vector<BuildOrderID> viableBuilds;
		for each(std::pair<BuildOrderID, BuildOrder> build in mBuildOrders)
		{
			if(build.second.getRace() != BWAPI::Broodwar->self()->getRace())
				continue;

			if(build.second.isStartBuild())
				viableBuilds.push_back(build.first);
		}

		changeCurrentBuild(getRandomBuild(viableBuilds));
	}

	if(BuildOrderFinished())
	{
		if(!mFinishedBuild)
		{
			mBuildFinishTime = BWAPI::Broodwar->getFrameCount();
			mFinishedBuild = true;
		}

		std::vector<BuildOrderID> viableBuilds;
		for(std::map<BuildOrderID, Condition>::const_iterator i = mBuildOrders[mCurrentBuild].getNextBuilds().begin(); i != mBuildOrders[mCurrentBuild].getNextBuilds().end(); ++i)
		{
			if(i->second.evauluate())
				viableBuilds.push_back(i->first);
		}

		if(!viableBuilds.empty())
			changeCurrentBuild(getRandomBuild(viableBuilds));
		else if(mBuildOrders[mCurrentBuild].getFallbackBuild() != BuildOrderID::None && mBuildOrders[mCurrentBuild].getFallbackTime() < BWAPI::Broodwar->getFrameCount() - mBuildFinishTime)
			changeCurrentBuild(mBuildOrders[mCurrentBuild].getFallbackBuild());
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
}

bool BuildOrderManagerClass::BuildOrderFinished()
{
	return mItemsWaiting.empty() && mOrdersWaiting.empty();
}

void BuildOrderManagerClass::changeCurrentBuild(BuildOrderID ID)
{
	if(mBuildOrders.find(ID) == mBuildOrders.end())
	{
		LOGMESSAGEWARNING(String_Builder() << "Couldn't find Build Order");
		changeCurrentBuild(BuildOrderID::Unknown);
		return;
	}

	if(mBuildOrders[ID].getRace() != BWAPI::Broodwar->self()->getRace() && mBuildOrders[ID].getRace() != BWAPI::Races::Unknown)
	{
		LOGMESSAGE(String_Builder() << "Build " << mBuildOrders[ID].getName() << " is for " << mBuildOrders[ID].getRace().getName());
		return;
	}

	mFinishedBuild = false;
	mItemsWaiting.clear();
	mOrdersWaiting.clear();
	mControlValues.clear();

	const BuildOrder &order = mBuildOrders[ID];

	if(ID != BuildOrderID::Unknown)
		LOGMESSAGEWARNING(String_Builder() << ((mCurrentBuild == BuildOrderID::None) ? "Opening with " : "Transitioning to ") << order.getName());

	mCurrentBuild = ID;

	for each(const OrderItem &item in order.getOrderItems())
	{
		if(item.isFulfilled())
			handleOrderItem(item);
		else
			mOrdersWaiting.push_back(item);
	}

	for each(const BuildItem &item in order.getBuildItems())
	{
		if(item.isFulfilled())
			handleBuildItem(item);
		else
			mItemsWaiting.push_back(item);
	}

	MacroManager::Instance().onChangeBuild();
	SquadManager::Instance().onChangeBuild();
}

void BuildOrderManagerClass::handleOrderItem(const OrderItem &item)
{
	toggleOrder(item.getType());

	LOGMESSAGE(String_Builder() << "Handled Order " << getOrderName(item.getType()) << (getOrder(item.getType()) ? ": Set to True" : ": Set to False"));
	LOGMESSAGE(String_Builder() );
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