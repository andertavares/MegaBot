#pragma once

#include "Interface.h"
#include <deque>
#include <sstream>

#include "Singleton.h"
#include "BuildOrder.h"
#include "TypeSafeEnum.h"
#include "TrainTask.h"
#include "UnitTracker.h"

struct TrainTypeDef
{
	enum type
	{
		Normal,
		LowMineral,
		LowGas
	};
};
typedef SafeEnum<TrainTypeDef> TrainType;

class MacroItem
{
public:
	MacroItem() : mUnit(BWAPI::UnitTypes::None), mTech(BWAPI::TechTypes::None), mUpgrade(BWAPI::UpgradeTypes::None), mLevel(0), mPriority(0){}
	MacroItem(BWAPI::UnitType unit, int priority) : mUnit(unit), mTech(BWAPI::TechTypes::None), mUpgrade(BWAPI::UpgradeTypes::None), mLevel(0), mPriority(priority){}
	MacroItem(BWAPI::TechType tech, int priority) : mUnit(BWAPI::UnitTypes::None), mTech(tech), mUpgrade(BWAPI::UpgradeTypes::None), mLevel(0), mPriority(priority){}
	MacroItem(BWAPI::UpgradeType upgrade, int level, int priority) : mUnit(BWAPI::UnitTypes::None), mTech(BWAPI::TechTypes::None), mUpgrade(upgrade), mLevel(level), mPriority(priority){}

	std::string getDebugInfo() const
	{
		std::stringstream returnString;
		returnString << mPriority << " : ";

		if(mUnit != BWAPI::UnitTypes::None)
			returnString << mUnit.getName();
		else if(mTech != BWAPI::TechTypes::None)
			returnString << mTech.getName();
		else if(mUpgrade != BWAPI::UpgradeTypes::None)
			returnString << mUpgrade.getName() << " (" << mLevel << ")";
		else
			returnString << "null";

		return returnString.str();
	}

	bool isUnitType() const { return mUnit != BWAPI::UnitTypes::None; }
	bool isTechType() const { return mTech != BWAPI::TechTypes::None; }
	bool isUpgradeType() const { return mUpgrade != BWAPI::UpgradeTypes::None; }

	BWAPI::UnitType getUnitType() const { return mUnit; }
	BWAPI::TechType getTechType() const { return mTech; }
	BWAPI::UpgradeType getUpgradeType() const { return mUpgrade; }

	int getUpgradeLevel() const { return mLevel; }

	int getPriority() const { return mPriority; }

	bool inProgress() const;

	TaskPointer createTask(TaskType taskType) const;

private:
	BWAPI::UnitType mUnit;
	BWAPI::TechType mTech;
	BWAPI::UpgradeType mUpgrade;
	int mLevel;

	int mPriority;
};

class MacroManagerClass
{
public:
	MacroManagerClass();

	void onBegin();
	void update();
	void updateTaskLists();

	void updateUnitProduction();
	void updateObserverProduction();
	void updateProductionProduction();
	void updateTech();

	void onChangeBuild();

	bool hasRequirements(BWAPI::UnitType type);
	bool hasRequirements(BWAPI::TechType type);
	bool hasRequirements(BWAPI::UpgradeType type, int level);

	int getPlannedCount(BWAPI::UnitType unitType);
	int getPlannedTotal(BWAPI::UnitType unitType);
	bool isPlanningUnit(BWAPI::UnitType unitType);

	int getPlannedCount(BWAPI::TechType techType);
	int getPlannedTotal(BWAPI::TechType techType);
	bool isPlanningTech(BWAPI::TechType techType);

	int getPlannedCount(BWAPI::UpgradeType upgradeType, int level);
	int getPlannedTotal(BWAPI::UpgradeType upgradeType, int level);
	bool isPlanningUpgrade(BWAPI::UpgradeType upgradeType, int level);

	std::set<BWAPI::UnitType> getNeededUnits(BWAPI::UnitType type);
	std::set<BWAPI::UnitType> getNeededUnits(BWAPI::TechType type);
	std::set<BWAPI::UnitType> getNeededUnits(BWAPI::UpgradeType type, int level);

	void onBuildTask(TaskPointer task, BWAPI::UnitType unitType);
	void onTechTask(TaskPointer task,BWAPI::TechType techType);
	void onUpgradeTask(TaskPointer task, BWAPI::UpgradeType upgradeType, int level);

private:
	std::map<BWAPI::TechType, int> mTechPriorityMap;
	std::map<BWAPI::UpgradeType, int> mUpgradePriorityMap;

	std::list<UnitToProduce> mNormalUnits;
	std::list<UnitToProduce> mLowGasUnits;
	std::list<UnitToProduce> mLowMineralUnits;

	std::list<std::pair<TaskPointer, BWAPI::UnitType>> mUnitProduce;
	std::map<BWAPI::UnitType, std::list<TaskPointer>> mTasksPerProductionType;
	std::map<BWAPI::TechType, std::list<TaskPointer>> mTasksPerTechType;
	std::map<BWAPI::UpgradeType, std::map<int, std::list<TaskPointer>>> mTasksPerUpgradeType;
	std::map<BWAPI::UnitType, std::list<TaskPointer>> mTasksPerProducedType;

	TaskPointer mObserver;

	std::list<MacroItem> mTechItemsToCreate;
	std::list<std::pair<MacroItem, TaskPointer>> mTechItems;

	int mLastStateChangeTime;
	std::list<UnitToProduce> mLastUnitsToProduce;
	
	void addNeeded(std::set<BWAPI::UnitType> &neededUnits);
	void createTechItems();

	bool mDebugDraw;
};

typedef Singleton<MacroManagerClass> MacroManager;