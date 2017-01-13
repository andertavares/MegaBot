#include "ResourceManager.h"
#include "BaseTracker.h"
#include "TaskManager.h"
#include "GatherTask.h"
#include "ResourceTracker.h"
#include "BuildOrderManager.h"
#include "UnitTracker.h"

using namespace SkynetBot;

ResourceManagerClass::ResourceManagerClass()
	: mTaskPump(BWAPI::Broodwar->self()->getRace().getWorker(), TaskType::Worker)
	, mSaturated(false)
	, mGasLevel(0)
	, mTimeAllowGasChange(0)
{
}

void ResourceManagerClass::update()
{
	std::set<Base> bases = BaseTracker::Instance().getActiveBases(true);
	if(mMyActiveBases != bases)
	{
		for each(Base base in mMyActiveBases)
		{
			std::set<Base>::iterator it = bases.find(base);
			if(it == bases.end())
			{
				for(std::map<Unit, GatherTaskPointer>::iterator resourceIT = mResourceTasks[base].begin(); resourceIT != mResourceTasks[base].end(); ++resourceIT)
					resourceIT->second->cancel();

				mResourceTasks.erase(base);
			}
		}

		mMyActiveBases = bases;
	}

	unsigned int neededWorkers = 0;
	for each(Base base in mMyActiveBases)
	{
		if(!base->getResourceDepot() || !base->getResourceDepot()->exists())
			continue;

		const UnitGroup &minerals = base->getMinerals();
		const UnitGroup &refineries = base->getRefineries();

		for(std::map<Unit, GatherTaskPointer>::iterator resourceIT = mResourceTasks[base].begin(); resourceIT != mResourceTasks[base].end();)
		{
			if(minerals.count(resourceIT->first) == 0 && refineries.count(resourceIT->first) == 0)
			{
				resourceIT->second->cancel();
				mResourceToBase.erase(resourceIT->first);
				mResourceTasks[base].erase(resourceIT++);
			}
			else
				++resourceIT;
		}

		for each(Unit mineral in minerals)
		{
			neededWorkers += 2;
			if(mResourceTasks[base].count(mineral) == 0)
			{
				GatherTaskPointer gatherTask(new GatherTask(mineral, base));
				TaskManager::Instance().addTask(gatherTask);
				mResourceTasks[base][mineral] = gatherTask;
				mResourceToBase[mineral] = base;
			}
		}

		for each(Unit refinery in refineries)
		{
			if(refinery->getPlayer() == BWAPI::Broodwar->self())
			{
				neededWorkers += 3;
				if(mResourceTasks[base].count(refinery) == 0 && refinery->exists())
				{
					GatherTaskPointer gatherTask(new GatherTask(refinery, base));
					TaskManager::Instance().addTask(gatherTask);
					mResourceTasks[base][refinery] = gatherTask;
					mResourceToBase[refinery] = base;
				}
			}
		}
	}

	if(!BuildOrderManager::Instance().getOrder(Order::TrainWorkers))
		neededWorkers = 0;
	else
		neededWorkers += 4;

	unsigned int numWorkers = UnitTracker::Instance().selectAllUnits(BWAPI::Broodwar->self()->getRace().getWorker()).size();

	mSaturated = numWorkers >= neededWorkers;

	neededWorkers = std::min(neededWorkers, 50u);
	neededWorkers -= std::min(numWorkers, neededWorkers);
	neededWorkers = std::min(neededWorkers, UnitTracker::Instance().selectAllUnits(BWAPI::Broodwar->self()->getRace().getWorker().whatBuilds().first).size() + 1);

	mTaskPump.changeTargetQuantity(neededWorkers);
	mTaskPump.update();

	double mineralRate = 0;
	double gasRate = 0;
	for(std::map<Base, std::map<Unit, GatherTaskPointer>>::iterator it = mResourceTasks.begin(); it != mResourceTasks.end(); ++it)
	{
		for(std::map<Unit, GatherTaskPointer>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			mineralRate += it2->second->getMineralRate();
			gasRate += it2->second->getGasRate();
		}
	}

	ResourceTracker::Instance().setMineralRate(mineralRate);
	ResourceTracker::Instance().setGasRate(gasRate);

	int futureMinerals = ResourceTracker::Instance().availableMineralAtTime(BWAPI::Broodwar->getFrameCount() + 450);
	int futureGas = ResourceTracker::Instance().availableGasAtTime(BWAPI::Broodwar->getFrameCount() + 450);

	if(mTimeAllowGasChange < BWAPI::Broodwar->getFrameCount())
	{
		int newLevel = 0;
		if(UnitTracker::Instance().selectAllUnits(BWAPI::Broodwar->self()->getRace().getRefinery()).size() == 0)
			newLevel = 8;
		else if(futureMinerals != 0)
		{
			float ratio = float(futureGas) / float(futureMinerals);

			if(ratio > 2.5f)
				newLevel = 0;
			else if(ratio > 1.9f)
				newLevel = 1;
			else if(ratio > 1.3f)
				newLevel = 2;
			else if(ratio > 1.0f)
				newLevel = 3;
			else if(ratio > 0.95f)
				newLevel = 4;
			else if(ratio > 0.9f)
				newLevel = 5;
			else if(ratio > 0.85f)
				newLevel = 6;
			else if(ratio > 0.4f)
				newLevel = 7;
			else
				newLevel = 8;
		}

		if(mGasLevel != newLevel)
		{
			mTimeAllowGasChange = BWAPI::Broodwar->getFrameCount() + 24;
			if(mGasLevel > newLevel)
				--mGasLevel;
			else if(newLevel > mGasLevel)
				++mGasLevel;
		}
	}
}

void ResourceManagerClass::onDestroy(Unit unit)
{
	std::map<Unit, Base>::iterator baseIT = mResourceToBase.find(unit);
	if(baseIT != mResourceToBase.end())
	{
		std::map<Unit, GatherTaskPointer>::iterator resourceIT = mResourceTasks[baseIT->second].find(unit);
		if(resourceIT != mResourceTasks[baseIT->second].end())
		{
			resourceIT->second->cancel();
			mResourceTasks[baseIT->second].erase(unit);
			mResourceToBase.erase(baseIT);
		}
	}
}