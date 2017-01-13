#pragma once

#include "Interface.h"

#include "TaskManager.h"
#include "BuildOrder.h"

class BuildOrderManagerClass
{
public:
	BuildOrderManagerClass();

	void onBegin();
	void onEnd(bool isWinner);
	void update();

	bool BuildOrderFinished();
	
	bool getOrder(Order type) const { return (mControlValues.count(type) != 0 ? mControlValues.find(type)->second : false); }

	const BuildOrder &getCurrentBuild() { return mBuildOrders[mCurrentBuild]; }

	void toggleDebugInfo() { mShowDebugInfo = !mShowDebugInfo; }

	std::string getOrderName(Order type);

private:
	void handleBuildItem(const BuildItem &item);
	void handleOrderItem(const OrderItem &item);
	void handleArmyBehaviourItem(const ArmyBehaviourItem &item);

	void checkBuildStatus();

	void LoadProtossBuilds();
	void LoadTerranBuilds();
	void LoadZergBuilds();
	void LoadOtherBuilds();

	void changeCurrentBuild(BuildOrderID ID);

	void toggleOrder(Order type);

	void BuildCallback(int buildID, CallBackType callbackType);

	void calculateNextBuilds();
	BuildOrderID getNextBuild() const;

	std::map<CallBackType, std::map<TaskPointer, int>> mWaitingItems;

	std::list<BuildItem> mItemsWaiting;
	std::list<OrderItem> mOrdersWaiting;
	std::list<ArmyBehaviourItem> mArmyBehavioursWaiting;

	std::map<BuildOrderID, BuildOrder> mBuildOrders;
	BuildOrderID mCurrentBuild;

	std::map<Order, bool> mControlValues;

	bool mShowDebugInfo;

	bool mFinishedBuild;
	int mBuildFinishTime;
	std::vector<FollowUpBuild> mNextBuilds;

	std::vector<std::pair<BuildOrderID, BuildOrderID>> mBuildHistory;
};

typedef Singleton<BuildOrderManagerClass> BuildOrderManager;