#include "ScoutManager.h"
#include "BaseTracker.h"
#include "TaskManager.h"
#include "BuildOrderManager.h"
#include "UnitTracker.h"
#include "MapHelper.h"

void ScoutManagerClass::onBegin()
{
	Base lastBase;
	std::set<Base> scoutBases;
	for each(Base base in BaseTracker::Instance().getAllBases())
	{
		mLastScoutTime[base] = 1500;
		mLastScoutType[base] = ScoutType::None;
		mCurrentScoutType[base] = ScoutType::None;

		if(base->isStartLocation())
		{
			if(base->getPlayer() == BWAPI::Broodwar->self())
				lastBase = base;
			else
				scoutBases.insert(base);
		}
	}

	if(!lastBase)
		return;

	while(!scoutBases.empty())
	{
		Base closestBase;
		int distance = std::numeric_limits<int>::max();
		for each(Base base in scoutBases)
		{
			int thisDistance = MapHelper::getGroundDistance(base->getCenterLocation(), lastBase->getCenterLocation());
			if(thisDistance < distance)
			{
				distance = thisDistance;
				closestBase = base;
			}
		}

		if(closestBase)
		{
			ScoutData newData = ScoutData(new ScoutDataClass(closestBase, ScoutType::InitialBaseLocationScout));

			mAllScoutData.insert(newData);
			mDataRequiringScout.push_back(newData);

			mCurrentScoutType[closestBase] = ScoutType::InitialBaseLocationScout;

			scoutBases.erase(closestBase);
		}

		lastBase = closestBase;
	}
}

void ScoutManagerClass::update()
{
	int time = BWAPI::Broodwar->getFrameCount();

	bool performThoroughSearch = (BaseTracker::Instance().getEnemyBases().empty() && time > 4000);
	ScoutType scoutType = performThoroughSearch ? ScoutType::ThoroughSearch : ScoutType::BaseScout;

	for each(Base base in BaseTracker::Instance().getAllBases())
	{
		if(mCurrentScoutType[base] != ScoutType::None)
			continue;

		// If we are doing thorough searches ignore last scout as we just need to find the enemy
		bool lastGroundFailed = mLastScoutType[base] == ScoutType::FailedWithGroundLow || mLastScoutType[base] == ScoutType::FailedWithGroundHigh || mLastScoutType[base] == ScoutType::CouldNotReachWithGround;
		if(performThoroughSearch && (lastGroundFailed || mLastScoutType[base] != ScoutType::FailedWithAir))
		{
			mLastScoutType[base] = ScoutType::None;
			lastGroundFailed = false;
		}

		int timeToReScout = 7000;
		if(lastGroundFailed)
			timeToReScout = 0;
		else if(performThoroughSearch && mLastScoutType[base] != scoutType && mCurrentScoutType[base] != scoutType)
			timeToReScout = 0;
		else if(performThoroughSearch && (base->isAllyBase() || base->isMyBase()))
			timeToReScout = 3500;
		else if(performThoroughSearch)
			timeToReScout = 1500;
		else if(mLastScoutType[base] != ScoutType::FailedWithAir)
			timeToReScout = 2000;
		else if(base->isEnemyBase())
			timeToReScout = 3500;

		if(mLastScoutTime[base] + timeToReScout <= time)
		{
			mLastScoutTime[base] = time;

			ScoutData newData = ScoutData(new ScoutDataClass(base, scoutType));

			mAllScoutData.insert(newData);
			mDataRequiringScout.push_back(newData);

			mCurrentScoutType[base] = scoutType;
		}
	}

	for(std::set<ScoutData>::iterator it = mAllScoutData.begin(); it != mAllScoutData.end();)
	{
		(*it)->update();

		if((*it)->isAchieved())
		{
			mCurrentScoutType[(*it)->getBase()] = ScoutType::None;
			mLastScoutType[(*it)->getBase()] = (*it)->getType();
			mLastScoutTime[(*it)->getBase()] = time;
			mAllScoutData.erase(it++);
		}
		else
			++it;
	}

	for(std::list<ScoutData>::iterator it = mDataRequiringScout.begin(); it != mDataRequiringScout.end();)
	{
#ifdef SKYNET_DRAW_DEBUG
		(*it)->drawDebug(BWAPI::Colors::Red);
#endif

		if((*it)->isAchieved()) mDataRequiringScout.erase(it++);
		else ++it;
	}

	updateObserverScouts();
	updateWorkerScouts();

#ifdef SKYNET_DRAW_DEBUG
	for each(Base base in BaseTracker::Instance().getAllBases())
	{
		int x = base->getRegion()->getCenter().x;
		int y = base->getRegion()->getCenter().y;

		BWAPI::Broodwar->drawTextMap(x, y, "Last Type: %s", ScoutType::getName(mLastScoutType[base].underlying()).c_str());
		BWAPI::Broodwar->drawTextMap(x, y+10, "This Type: %s", ScoutType::getName(mCurrentScoutType[base].underlying()).c_str());
		BWAPI::Broodwar->drawTextMap(x, y+20, "Time: %d", mLastScoutTime[base]);
	}
#endif
}

void ScoutManagerClass::updateWorkerScouts()
{
	for(std::set<WorkerScoutTaskPointer>::iterator it = mWorkerScouts.begin(); it != mWorkerScouts.end();)
	{
		if((*it)->isFinishedScouting())
			mWorkerScouts.erase(it++);
		else
			++it;
	}

	// use more workers the longer the game
	unsigned int maxForThisTime = !BuildOrderManager::Instance().getOrder(Order::Scout) ? 0 : BWAPI::Broodwar->getFrameCount() > 24*60*7 ? 2 : 1;

	//BWAPI::Broodwar->drawTextScreen(5, 15, "Max Worker Scouts : %u", maxForThisTime);
	//BWAPI::Broodwar->drawTextScreen(5, 25, "Current Worker Scouts : %u", mWorkerScouts.size());

	// if we are short on tasks for workers, create some
	if(maxForThisTime > mWorkerScouts.size())
	{
		ScoutData data = getScoutData(ScoutUnitType::Worker);

		if(data)
		{
			WorkerScoutTaskPointer task = WorkerScoutTaskPointer(new WorkerScoutTask(TaskType::Scout, data));
			mWorkerScouts.insert(task);

			TaskManager::Instance().addTask(task);
		}
	}
}

void ScoutManagerClass::updateObserverScouts()
{
	for(std::set<ObserverScoutTaskPointer>::iterator it = mObserverScouts.begin(); it != mObserverScouts.end();)
	{
		if((*it)->hasEnded())
			mObserverScouts.erase(it++);
		else
			++it;
	}

	unsigned int curreentObservers = UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_Observer).size();
	unsigned int maxObservers = curreentObservers > 1 ? curreentObservers - 1 : 0;
	//BWAPI::Broodwar->drawTextScreen(5, 35, "Max Observer Scouts : %u", maxObservers);
	//BWAPI::Broodwar->drawTextScreen(5, 45, "Current Observer Scouts : %u", mObserverScouts.size());

	if(maxObservers > mObserverScouts.size())
	{
		ScoutData data = getScoutData(ScoutUnitType::Observer);

		ObserverScoutTaskPointer task = ObserverScoutTaskPointer(new ObserverScoutTask(TaskType::Scout, data));
		mObserverScouts.insert(task);

		TaskManager::Instance().addTask(task);
	}
}

void ScoutManagerClass::updateLastScoutType(ScoutData data, ScoutType type)
{
	mCurrentScoutType[data->getBase()] = ScoutType::None;
	mAllScoutData.erase(data);

	mLastScoutTime[data->getBase()] = BWAPI::Broodwar->getFrameCount();
	mLastScoutType[data->getBase()] = type;
}

ScoutData ScoutManagerClass::getScoutData(ScoutUnitType unit)
{
	for(std::list<ScoutData>::iterator it = mDataRequiringScout.begin(); it != mDataRequiringScout.end(); ++it)
	{
		const ScoutData data = *it;
		if(unit == ScoutUnitType::Worker)
		{
			if(data->getBase()->isEnemyBase() && BWAPI::Broodwar->getFrameCount() > 6000)
				continue;
			else if(mLastScoutType[data->getBase()] == ScoutType::CouldNotReachWithGround)
				continue;
			else if(mLastScoutType[data->getBase()] == ScoutType::FailedWithGroundHigh)
				continue;
			else if(mLastScoutType[data->getBase()] == ScoutType::FailedWithGroundLow && mLastScoutTime[data->getBase()] + 3500 > BWAPI::Broodwar->getFrameCount())
				continue;
		}
		else if(unit == ScoutUnitType::Observer)
		{
			if(mLastScoutType[data->getBase()] == ScoutType::FailedWithAir)
				continue;
		}

		mDataRequiringScout.erase(it);
		return data;
	}

	return ScoutData();
}