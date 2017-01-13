#include "BlockedPathManager.h"

#include "UnitTracker.h"
#include "BaseTracker.h"
#include "MapHelper.h"
#include "TerrainAnaysis.h"
#include "MineBlockingMineralTask.h"
#include "TaskManager.h"
#include "BuildingPlacer.h"
#include "BuildOrderManager.h"

BlockedPathManagerClass::BlockedPathManagerClass()
{
}

void BlockedPathManagerClass::onBegin()
{
	for each(Unit mineral in UnitTracker::Instance().getMinerals())
	{
		if(mineral->getResources() <= 1)
		{
			const TilePosition &mineralTile = mineral->getTilePosition();
			Base base = BaseTracker::Instance().getBase(mineralTile);

			bool isBlocking = false;
			if(base)
			{
				const TilePosition &baseTile = base->getCenterBuildLocation();

				isBlocking = (mineralTile.x > baseTile.x - 5 &&
								   mineralTile.y > baseTile.y - 4 &&
								   mineralTile.x < baseTile.x + 7 &&
								   mineralTile.y < baseTile.y + 6);
			}

			if(!isBlocking)
			{
				const Position &mineralPos = mineral->getPosition();

				Chokepoint closestChoke;
				int closestDistance = std::numeric_limits<int>::max();
				for each(Chokepoint chokepoint in TerrainAnaysis::Instance().getChokepoints())
				{
					int distance = chokepoint->getCenter().getApproxDistance(mineralPos);
					if(distance < closestDistance)
					{
						closestChoke = chokepoint;
						closestDistance = distance;
					}
				}

				if(closestChoke)
				{
					std::set<Base> bases;
					bases.insert(closestChoke->getRegions().first->getBases().begin(), closestChoke->getRegions().first->getBases().end());
					bases.insert(closestChoke->getRegions().second->getBases().begin(), closestChoke->getRegions().second->getBases().end());

					Base closestBase;
					int closestDistance = std::numeric_limits<int>::max();
					for each(Base base in bases)
					{
						if(base->isStartLocation())
							continue;

						if(base->getCenterBuildLocation() == BWAPI::TilePositions::None)
							continue;

						int distance = Position(base->getCenterBuildLocation()).getApproxDistance(mineralPos);
						if(distance < closestDistance)
						{
							closestDistance = distance;
							closestBase = base;
						}
					}

					if(closestBase)
					{
						base = closestBase;
						isBlocking = true;
					}
				}
			}

			if(isBlocking)
				mBlockingMinerals[base].insert(mineral);
		}
	}
}

void BlockedPathManagerClass::update()
{
	const std::set<Base> &myBases = BaseTracker::Instance().getActiveBases(true);
	if(myBases != mMyBases)
	{
		mMyBases = myBases;

		for each(Base base in mMyBases)
		{
			if(mBlockingMinerals.count(base) != 0)
			{
				TaskPointer task = TaskPointer(new MineBlockingMineralTask(TaskType::Expansion, mBlockingMinerals[base]));
				TaskManager::Instance().addTask(task);

				mBlockingMinerals.erase(base);
			}
		}

		Base nextExpansion = BuildingPlacer::Instance().getExpandLocation().second;
		if(nextExpansion)
		{
			if(mBlockingMinerals.count(nextExpansion) != 0)
			{
				TaskPointer task = TaskPointer(new MineBlockingMineralTask(TaskType::Expansion, mBlockingMinerals[nextExpansion]));
				TaskManager::Instance().addTask(task);

				mBlockingMinerals.erase(nextExpansion);
			}
		}
	}
}