#include "ExpansionManager.h"

#include "BuildOrderManager.h"
#include "BaseTracker.h"
#include "UnitTracker.h"
#include "TaskManager.h"
#include "PlayerTracker.h"
#include "MacroManager.h"
#include "ResourceManager.h"
#include "Logger.h"

ExpansionManagerClass::ExpansionManagerClass()
{
}

void ExpansionManagerClass::update()
{
	if(BuildOrderManager::Instance().getOrder(Order::RefineryManager))
		updateRefineries();

	if(BuildOrderManager::Instance().getOrder(Order::ExpansionManager))
	{
		updateDefense();
		updateExpands();
	}
}

void ExpansionManagerClass::updateRefineries()
{
	int refNeeded = 0;
	for each(Base base in BaseTracker::Instance().getActiveBases(true))
	{
		if(base->getActivateTime() < unsigned int(BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->self()->getRace().getRefinery().buildTime()))
			refNeeded += base->getGeysers().size();
	}

	for(std::list<TaskPointer>::iterator it = mRefineryTasks.begin(); it != mRefineryTasks.end();)
	{
		if((*it)->hasEnded())
			mRefineryTasks.erase(it++);
		else
		{
			if(!(*it)->inProgress())
				--refNeeded;
			++it;
		}
	}

	if(ResourceManager::Instance().requiresRefineries() && refNeeded > 0 && BWAPI::Broodwar->getFrameCount() % 50 == 0)
		mRefineryTasks.push_front(TaskManager::Instance().build(BWAPI::Broodwar->self()->getRace().getRefinery(), TaskType::RefineryManager));
	else if(refNeeded < 0)
	{
		for(int i = 0; i < refNeeded; ++i)
		{
			std::list<TaskPointer>::iterator begin = mRefineryTasks.begin();
			(*begin)->cancel();
			mRefineryTasks.erase(begin);
		}
	}
}

void ExpansionManagerClass::updateDefense()
{
	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg)
		return;

	BWAPI::UnitType defenseType = BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss ? BWAPI::UnitTypes::Protoss_Photon_Cannon : BWAPI::UnitTypes::Terran_Missile_Turret;

	if(!MacroManager::Instance().hasRequirements(defenseType))
		return;

	std::set<Base> myBases = BaseTracker::Instance().getActiveBases(true);
	if(myBases.size() >= 2)
	{
		int defensesNeeded = 0;
		int neededPerBase = PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Zerg) ? 4 : 2;

		for each(Base base in myBases)
		{
			if(base->getMinerals().empty())
				continue;

			bool hasPylon = defenseType.requiresPsi() ? false : true;
			int thisCount = 0;
			for each(Unit building in base->getBuildings())
			{
				if(building->getType() == defenseType)
					++thisCount;

				if(building->getType() == BWAPI::UnitTypes::Protoss_Pylon)
				{
					if(building->isCompleted())
						hasPylon = true;
				}
			}

			if(hasPylon)
			{
				defensesNeeded += neededPerBase;
				defensesNeeded -= std::min(thisCount, neededPerBase);
			}
			else if(BWAPI::Broodwar->self()->supplyTotal() >= 380 && (!mPylon || mPylon->hasEnded()))
				mPylon = TaskManager::Instance().build(BWAPI::UnitTypes::Protoss_Pylon, TaskType::Defense);
		}

		for(std::list<TaskPointer>::iterator it = mDefenseTasks.begin(); it != mDefenseTasks.end();)
		{
			if((*it)->hasEnded())
				mDefenseTasks.erase(it++);
			else
			{
				if(!(*it)->inProgress())
					--defensesNeeded;
				++it;
			}
		}

		if(defensesNeeded > 0)
		{
			for(int i = 0; i < defensesNeeded; ++i)
			{
				LOGMESSAGE(String_Builder() << "Built Defense.");
				mDefenseTasks.push_front(TaskManager::Instance().build(defenseType, TaskType::Defense));
			}
		}
		else if(defensesNeeded < 0)
		{
			for(int i = 0; i < defensesNeeded; ++i)
			{
				std::list<TaskPointer>::iterator begin = mDefenseTasks.begin();
				LOGMESSAGE(String_Builder() << "Cancelled Defense.");
				(*begin)->cancel();
				mDefenseTasks.erase(begin);
			}
		}
	}
}

void ExpansionManagerClass::updateExpands()
{
	bool unstartedTasks = false;

	for(std::list<TaskPointer>::iterator it = mExpandTasks.begin(); it != mExpandTasks.end();)
	{
		if((*it)->hasEnded())
			mExpandTasks.erase(it++);
		else
		{
			if(!(*it)->inProgress())
				unstartedTasks = true;
			++it;
		}
	}

	if(ResourceManager::Instance().isSaturated() && !unstartedTasks)
	{
		LOGMESSAGE(String_Builder() << "Expanded because im saturated.");
		mExpandTasks.push_front(TaskManager::Instance().build(BWAPI::Broodwar->self()->getRace().getCenter(), TaskType::Expansion, BuildingLocation::Expansion));
	}
}