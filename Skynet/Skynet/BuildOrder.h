#pragma once

#include "Interface.h"
#include <deque>

#include "TypeSafeEnum.h"
#include "Condition.h"
#include "TaskManager.h"
#include "ReservedLocation.h"

struct SquadTypeDef
{
	enum type
	{
		DefaultSquad,
		DefenseSquad,
		ReaverDropSquad,
		DarkTemplerSquad,
		CorsairSquad
	};
};
typedef SafeEnum<SquadTypeDef> SquadType;

struct ArmyBehaviourDef
{
	enum type
	{
		AllIn = 0, //fkit just attack
		Aggresive, //pressure
		Contain, //siege contain etc
		Default, //Attack when it knows its ahead, roughtly
		Defensive //defensive
	};
};
typedef SafeEnum<ArmyBehaviourDef> ArmyBehaviour;

struct OrderDef
{
	enum type
	{
		None,
		TrainWorkers,
		Scout,
		SupplyManager,
		RefineryManager,
		MacroArmyProduction,
		CanRemoveSquads,
		ExpansionManager,
		MacroProductionFacilities,
		MacroCanTech
	};
};
typedef SafeEnum<OrderDef> Order;

struct CallBackTypeDef
{
	enum type
	{
		None,
		onDispatched,
		onCompleted,
		onStarted
	};
};
typedef SafeEnum<CallBackTypeDef> CallBackType;

class CallBack
{
public:
	CallBack() : mBuildID(0), mCallbackType(CallBackType::None) {}
	CallBack(int buildID, CallBackType callbackType) : mBuildID(buildID), mCallbackType(callbackType) {}

	int getID() const { return mBuildID; }
	CallBackType getType() const { return mCallbackType; }

private:
	int mBuildID;
	CallBackType mCallbackType;
};

class BuildItem
{
public:
	BuildItem(BWAPI::UnitType unitType, int count, int buildID, TaskType taskType, BuildingLocation location, const std::list<CallBack> &callBacks)
		: mUnitType(unitType)
		, mTechType(BWAPI::TechTypes::None)
		, mUpgradeType(BWAPI::UpgradeTypes::None)
		, mLocation(location)
		, mNumber(count)
		, mID(buildID)
		, mTaskType(taskType)
		, mCallBacks(callBacks)
	{}

	BuildItem(BWAPI::TechType techType, int buildID, TaskType taskType, const std::list<CallBack> &callBacks)
		: mUnitType(BWAPI::UnitTypes::None)
		, mTechType(techType)
		, mUpgradeType(BWAPI::UpgradeTypes::None)
		, mLocation()
		, mNumber()
		, mID(buildID)
		, mTaskType(taskType)
		, mCallBacks(callBacks)
	{}

	BuildItem(BWAPI::UpgradeType upgradeType, int level, int buildID, TaskType taskType, const std::list<CallBack> &callBacks)
		: mUnitType(BWAPI::UnitTypes::None)
		, mTechType(BWAPI::TechTypes::None)
		, mUpgradeType(upgradeType)
		, mLocation()
		, mNumber(level)
		, mID(buildID)
		, mTaskType(taskType)
		, mCallBacks(callBacks)
	{}

	bool isFulfilled() const { return mCallBacks.empty(); }

	void removeCallback(int buildID, CallBackType callbackType)
	{
		for(std::list<CallBack>::iterator callback = mCallBacks.begin(); callback != mCallBacks.end();)
		{
			if(callback->getID() == buildID && callback->getType() == callbackType)
				mCallBacks.erase(callback++);
			else
				++callback;
		}
	}

	TaskPointer createTask() const
	{
		if(mUnitType != BWAPI::UnitTypes::None)
		{
			for(int i = 0; i < mNumber-1; ++i)
				TaskManager::Instance().build(mUnitType, mTaskType, mLocation);

			return TaskManager::Instance().build(mUnitType, mTaskType, mLocation);
		}
		else if(mTechType != BWAPI::TechTypes::None)
			return TaskManager::Instance().research(mTechType, mTaskType);
		else if(mUpgradeType != BWAPI::UpgradeTypes::None)
			return TaskManager::Instance().upgrade(mUpgradeType, mNumber, mTaskType);

		return TaskPointer();
	}

	int getID() const { return mID; }

private:
	BWAPI::UnitType mUnitType;
	BWAPI::TechType mTechType;
	BWAPI::UpgradeType mUpgradeType;
	BuildingLocation mLocation;
	int mNumber;
	int mID;
	TaskType mTaskType;

	std::list<CallBack> mCallBacks;
};

class OrderItem
{
public:
	OrderItem(Order type, int ID, const std::list<CallBack> &callBacks)
		: mType(type)
		, mID(ID)
		, mCallbacks(callBacks)
	{}

	Order getType() const { return mType; }
	int getID() const { return mID; }

	bool isFulfilled() const { return mCallbacks.empty(); }

	void removeCallback(int buildID, CallBackType callbackType)
	{
		for(std::list<CallBack>::iterator callback = mCallbacks.begin(); callback != mCallbacks.end();)
		{
			if(callback->getID() == buildID && callback->getType() == callbackType)
				mCallbacks.erase(callback++);
			else
				++callback;
		}
	}

private:
	Order mType;
	int mID;

	std::list<CallBack> mCallbacks;
};

class ArmyBehaviourItem
{
public:
	ArmyBehaviourItem(ArmyBehaviour armyBehaiour, const std::list<CallBack> &callBacks)
		: mArmyBehaiour(armyBehaiour)
		, mCallbacks(callBacks)
	{}

	ArmyBehaviour getArmyBehaviour() const { return mArmyBehaiour; }

	bool isFulfilled() const { return mCallbacks.empty(); }

	void removeCallback(int buildID, CallBackType callbackType)
	{
		for(std::list<CallBack>::iterator callback = mCallbacks.begin(); callback != mCallbacks.end();)
		{
			if(callback->getID() == buildID && callback->getType() == callbackType)
				mCallbacks.erase(callback++);
			else
				++callback;
		}
	}

private:
	ArmyBehaviour mArmyBehaiour;

	std::list<CallBack> mCallbacks;
};

struct BuildOrderIDDef
{
	enum type
	{
		None,
		Finished,
		ForgeExpand,
		StargateArcon,
		ArconTiming,
		PvZEndGame,
		TwoGate,
		CoreIntoStargate,
		FourteenNexus,
		OneGateCore,
		AdditionalGateWays,
		FourGateGoon,
		Nexus,
		CitadelFirst,
		PvTMidGame,
		PvTCarrierSwitch,
		PvTEndGame,
		RoboVsProtoss,
		RoboVsTerran,
		PvPMidGame,
		PvPEndGame,
		DTRush
	};
};
typedef SafeEnum<BuildOrderIDDef> BuildOrderID;

std::list<CallBack> CB(int buildID, CallBackType type, std::list<CallBack> &cb = std::list<CallBack>());

class UnitToProduce
{
public:
	//************************************
	// BWAPI::UnitType unitType: Type of unit to produce
	// int weight: Used for the ratio of how many of this unit to create compared to others
	// int priority: Used for how important teching to this unit is
	// Condition unitCond: Condition required to be true to build this unit
	// Condition factoryCond: Condition required to be true to build addition of what builds this unit
	//************************************
	UnitToProduce(BWAPI::UnitType unitType, int weight, int priority = 100, const Condition &unitCond = Condition(ConditionTest::None, true), const Condition &factoryCond = Condition(ConditionTest::None, true))
		: mUnitType(unitType)
		, mWeight(weight)
		, mCreateUnitCondition(unitCond)
		, mCreateFactoryCondition(factoryCond)
		, mPriority(priority)
	{}

	bool canBuildUnit() const { return mCreateUnitCondition.evauluate(); }
	bool canBuildFactory() const { return mCreateFactoryCondition.evauluate(); }

	BWAPI::UnitType getUnitType() const { return mUnitType; }
	int getUnitWeight() const { return mWeight; }

	int getPriority() const { return mPriority; }

	bool operator==(const UnitToProduce &other) const
	{
		if(mUnitType != other.mUnitType)
			return false;
		else if(mWeight != other.mWeight)
			return false;
		else if(mPriority != other.mPriority)
			return false;

		return true;
	}

private:
	BWAPI::UnitType mUnitType;
	int mWeight;
	int mPriority;

	Condition mCreateUnitCondition;
	Condition mCreateFactoryCondition;
};

class FollowUpBuild
{
public:
	FollowUpBuild(BuildOrderID id, int timeAfterCompletion = 0, const Condition & condition = Condition(ConditionTest::None, true))
		: mID(id)
		, mTimeAfterCompletion(timeAfterCompletion)
		, mCondition(condition)
	{
	}

	BuildOrderID getBuildID() const { return mID; }
	int getCompletionTime() const { return mTimeAfterCompletion; }
	bool evauluate() const { return mCondition.evauluate(); }

private:
	BuildOrderID mID;
	int mTimeAfterCompletion;
	Condition mCondition;
};

class BuildOrder
{
public:
	BuildOrder(BuildOrderID id = BuildOrderID::None, std::string name = "None");
	BuildOrder(BWAPI::Race race, BuildOrderID id, std::string name);

	int addItem(BWAPI::UnitType type, std::list<CallBack> &callBacks = std::list<CallBack>(), int count = 1, BuildingLocation position = BuildingLocation::Base);
	int addItem(BWAPI::TechType type, std::list<CallBack> &callBacks = std::list<CallBack>());
	int addItem(BWAPI::UpgradeType type, int level = 1, std::list<CallBack> &callBacks = std::list<CallBack>());

	int addItem(BWAPI::UnitType type, int count, BuildingLocation position = BuildingLocation::Base, TaskType taskType = TaskType::BuildOrder);
	int addItem(BWAPI::UnitType type, int count, TaskType taskType, BuildingLocation position = BuildingLocation::Base);

	int addItem(BWAPI::UnitType type, TaskType taskType, std::list<CallBack> &callBacks = std::list<CallBack>(), int count = 1, BuildingLocation position = BuildingLocation::Base);
	int addItem(BWAPI::TechType type, TaskType taskType, std::list<CallBack> &callBacks = std::list<CallBack>());
	int addItem(BWAPI::UpgradeType type, int level, TaskType taskType, std::list<CallBack> &callBacks = std::list<CallBack>());

	int addOrder(Order orderType, std::list<CallBack> &callBacks = std::list<CallBack>());

	void addProduce(BWAPI::UnitType type, int weight, int priority = 100, const Condition &unitCond = Condition(ConditionTest::None, true), const Condition &factoryCond = Condition(ConditionTest::None, true));

	void addNextBuild(BuildOrderID id, int timeAfterCompletion = 0, const Condition & condition = Condition(ConditionTest::None, true));

	void addSquad(SquadType type, int count = 1);

	void addArmyBehaviour(ArmyBehaviour armyBehaiour, std::list<CallBack> &callBacks = std::list<CallBack>());

	void setStartingCondition(Condition condition);

	BWAPI::Race getRace() const { return mRace; }
	BuildOrderID getID() const { return mID; }
	const std::string &getName() const { return mName; }

	const std::deque<BuildItem> &getBuildItems() const { return mItems; }
	const std::deque<OrderItem> &getOrderItems() const { return mOrders; }
	const std::deque<ArmyBehaviourItem> &getArmyBehaviourItems() const { return mArmyBehaviours; }
	const std::list<UnitToProduce> &getUnitsToProduce() const { return mProduces; }
	const std::map<SquadType, int> &getSquads() const { return mSquads; }

	bool isStartBuild() const { return mStartingCondition.evauluate(); }

	const std::vector<FollowUpBuild> &getNextBuilds() const { return mNextBuilds; }

	float getWinRate(BuildOrderID currentBuild) const;

private:
	BWAPI::Race mRace;
	BuildOrderID mID;
	std::string mName;

	std::deque<BuildItem> mItems;
	std::deque<OrderItem> mOrders;
	std::deque<ArmyBehaviourItem> mArmyBehaviours;
	std::list<UnitToProduce> mProduces;
	std::map<SquadType, int> mSquads;

	Condition mStartingCondition;

	std::vector<FollowUpBuild> mNextBuilds;

	int mItemCounter;
};