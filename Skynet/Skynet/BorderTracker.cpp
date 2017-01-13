#include "BorderTracker.h"

#include "TerrainAnaysis.h"
#include "BaseTracker.h"
#include "UnitTracker.h"
#include "UnitHelper.h"

void BorderTrackerClass::onBegin()
{
	mShowDebugInfo = true;
	for each(Region region in TerrainAnaysis::Instance().getRegions())
	{
		if(region->getSize() > 100000)
			mLargeRegions.insert(region);
	}
}

void BorderTrackerClass::update()
{
	std::set<Base> myBases = BaseTracker::Instance().getImportantBases();
	std::set<Base> enemyBases = BaseTracker::Instance().getEnemyImportantBases();
	if(enemyBases != mEnemyBases || myBases != mMyBases)
	{
		mMyBases = myBases;
		mEnemyBases = enemyBases;
		recalculateBorders();
	}

	if(mShowDebugInfo)
	{
		for each(Region region in mLargeRegions)
		{
			region->draw(BWAPI::Colors::Red);
		}

		drawDebugInfo(mBorderPositions.find(PositionType::TechDefenseChokepoint));
		drawDebugInfo(mBorderPositions.find(PositionType::EnemyControlRegion));
		drawDebugInfo(mBorderPositions.find(PositionType::BotControlRegion));
		drawDebugInfo(mBorderPositions.find(PositionType::ForwardRegion));
		drawDebugInfo(mBorderPositions.find(PositionType::SafeTravelRegion));
		drawDebugInfo(mBorderPositions.find(PositionType::ContainChokepoint));
		drawDebugInfo(mBorderPositions.find(PositionType::ForwardChokepoint));
		drawDebugInfo(mBorderPositions.find(PositionType::EnemyChokepoint));
		drawDebugInfo(mBorderPositions.find(PositionType::DefenseChokepoint));
		drawDebugInfo(mBorderPositions.find(PositionType::SharedChokepoint));
		drawDebugInfo(mBorderPositions.find(PositionType::Position));
		drawDebugInfo(mBorderPositions.find(PositionType::HoldBase));
	}
}

void BorderTrackerClass::recalculateBorders()
{
	mMyRegions.clear();
	mMyForwardRegions.clear();
	mMyBorder.clear();
	mMyForwardBorder.clear();
	mEnemyRegions.clear();
	mEnemyBorder.clear();
	mBorderPositions.clear();
	mSafeRegions.clear();

	std::set<Region> canReachSelf;
	std::set<Region> canReachEnemy;
	std::map<Region, int> regionTech;

	//Add all my regions to the set
	for each(Base base in mMyBases)
	{
		if(mLargeRegions.count(base->getRegion()) == 0)
		{
			regionTech[base->getRegion()] += base->getNumberOfTechBuildings();
			mMyRegions.insert(base->getRegion());
			canReachSelf.insert(base->getRegion());
		}
	}

	//Add all enemy regions to the set
	for each(Base base in mEnemyBases)
	{
		if(mLargeRegions.count(base->getRegion()) == 0)
		{
			regionTech[base->getRegion()] += base->getNumberOfTechBuildings();
			mEnemyRegions.insert(base->getRegion());
			canReachEnemy.insert(base->getRegion());
		}
	}

	//If there are enemy bases
	if(!mEnemyBases.empty())
	{
		bool exploring = true;
		while(exploring)
		{
			exploring = false;

			//Go through the regions
			for each(Region region in TerrainAnaysis::Instance().getRegions())
			{
				for each(Chokepoint chokepoint in region->getChokepoints())
				{
					Region secondRegion = chokepoint->getRegions().first;
					if(region == secondRegion)
						secondRegion = chokepoint->getRegions().second;

					//if this region is in the set and the connected region cannot be reached by the enemy, it can be considered ours.
					if(canReachSelf.count(region) != 0 && mEnemyRegions.count(secondRegion) == 0 && canReachSelf.count(secondRegion) == 0)
					{
						canReachSelf.insert(secondRegion);
						exploring = true;
					}

					//if this region is in the set and the connected region cannot be reached by me, it can be considered the enemies.
					if(canReachEnemy.count(region) != 0 && mMyRegions.count(secondRegion) == 0 && canReachEnemy.count(secondRegion) == 0)
					{
						canReachEnemy.insert(secondRegion);
						exploring = true;
					}
				}
			}
		}
	}

	for each(Region region in TerrainAnaysis::Instance().getRegions())
	{
		//if we can reach this region and the enemy can't, it can be ours
		if(canReachSelf.count(region) != 0 && canReachEnemy.count(region) == 0)
		{
			mBorderPositions[PositionType::BotControlRegion].insert(BorderPosition(PositionType::BotControlRegion, region));
			mMyRegions.insert(region);
			mMyForwardRegions.insert(region);
		}

		//if we cant reach this region and the enemy can, it can be the enemies
		if(canReachSelf.count(region) == 0 && canReachEnemy.count(region) != 0)
		{
			mBorderPositions[PositionType::EnemyControlRegion].insert(BorderPosition(PositionType::EnemyControlRegion, region));
			mEnemyRegions.insert(region);
		}
	}

	//any chokepoints that dont straddle 2 regions are defense chokepoints
	for each(Region region in mMyRegions)
	{
		for each(Chokepoint chokepoint in region->getChokepoints())
		{
			if(mMyBorder.count(chokepoint) == 0)
				mMyBorder.insert(chokepoint);
			else
				mMyBorder.erase(chokepoint);
		}
	}

	std::map<Chokepoint, int> borderChokeTech;
	for each(Chokepoint chokepoint in mMyBorder)
	{
		Region startRegion = chokepoint->getRegions().first;
		if(mMyRegions.count(startRegion) == 0)
			startRegion = chokepoint->getRegions().second;

		std::set<Region> visitedRegions;
		std::set<Region> unvisitedRegions;
		int controlledAreaTech = 0;
		unvisitedRegions.insert(startRegion);

		while(!unvisitedRegions.empty())
		{
			std::set<Region>::iterator it = unvisitedRegions.begin();
			visitedRegions.insert(*it);

			controlledAreaTech += regionTech[*it];

			for each(Chokepoint connectedChoke in (*it)->getChokepoints())
			{
				if(mMyBorder.count(connectedChoke) != 0)
					continue;

				Region connectedRegion = connectedChoke->getRegions().first;
				if(connectedRegion == *it)
					connectedRegion = connectedChoke->getRegions().second;

				if(visitedRegions.count(connectedRegion) == 0)
					unvisitedRegions.insert(connectedRegion);
			}

			unvisitedRegions.erase(it);
		}

		borderChokeTech[chokepoint] = controlledAreaTech;
	}

	for each(Region region in mEnemyRegions)
	{
		for each(Chokepoint chokepoint in region->getChokepoints())
		{
			if (mEnemyBorder.count(chokepoint) == 0)
				mEnemyBorder.insert(chokepoint);
			else
				mEnemyBorder.erase(chokepoint);
		}
	}

	mMyForwardBorder = mMyBorder;
	for each(Chokepoint chokepoint in mMyBorder)
	{
		mBorderPositions[PositionType::DefenseChokepoint].insert(BorderPosition(PositionType::DefenseChokepoint, chokepoint));

		if(mEnemyBorder.count(chokepoint) != 0)
			mBorderPositions[PositionType::SharedChokepoint].insert(BorderPosition(PositionType::SharedChokepoint, chokepoint));

		if(borderChokeTech[chokepoint] > 0)
			mBorderPositions[PositionType::TechDefenseChokepoint].insert(BorderPosition(PositionType::TechDefenseChokepoint, chokepoint));
	}

	for each(Chokepoint chokepoint in mEnemyBorder)
	{
		mBorderPositions[PositionType::EnemyChokepoint].insert(BorderPosition(PositionType::EnemyChokepoint, chokepoint));
	}

	if(!mEnemyBases.empty())
	{
		bool expanding = true;
		while(expanding)
		{
			expanding = false;

			for each(Chokepoint chokepoint in mMyForwardBorder)
			{
				//get the region the otherside of this chokepoint
				Region region = chokepoint->getRegions().first;
				if(mMyForwardRegions.count(region) != 0)
					region = chokepoint->getRegions().second;

				//make sure it isn't an enemy region
				if(mEnemyRegions.count(region) != 0)
					continue;

				int oldCount = 0;
				int newCount = 0;

				//count the number of chokepoints part of our border and those not
				for each(Chokepoint chokepoint in region->getChokepoints())
				{
					if(mMyForwardBorder.count(chokepoint) != 0)
						++oldCount;
					else
						++newCount;
				}

				//if we can reduce the number of chokepoints
				if(newCount <= oldCount)
				{
					//reverse regions, so we consider this region ours.
					for each(Chokepoint chokepoint in region->getChokepoints())
					{
						if(mMyForwardBorder.count(chokepoint) != 0)
							mMyForwardBorder.erase(chokepoint);
						else
							mMyForwardBorder.insert(chokepoint);
					}

					mMyForwardRegions.insert(region);

					expanding = true;
					break;
				}
			}
		}
	}

	for each(Chokepoint chokepoint in mMyForwardBorder)
	{
		if(mMyBorder.count(chokepoint) == 0)
			mBorderPositions[PositionType::ForwardChokepoint].insert(BorderPosition(PositionType::ForwardChokepoint, chokepoint));
	}

	for each(Region region in mMyForwardRegions)
	{
		if(mMyRegions.count(region) == 0)
			mBorderPositions[PositionType::ForwardRegion].insert(BorderPosition(PositionType::ForwardRegion, region));
	}

	std::map<Region, int> regionMyArmySupply;
	mRegionEnemyArmySupply.clear();

	for each(Unit unit in UnitTracker::Instance().selectAllUnits())
	{
		if(UnitHelper::isArmyUnit(unit->getType()))
		{
			regionMyArmySupply[TerrainAnaysis::Instance().getRegion(unit->getPosition())] += unit->getType().supplyRequired();
		}
	}

	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		if(UnitHelper::isArmyUnit(unit->getType()))
		{
			mRegionEnemyArmySupply[TerrainAnaysis::Instance().getRegion(unit->getPosition())] += unit->getType().supplyRequired();
		}
	}

	for each(Region region in TerrainAnaysis::Instance().getRegions())
	{
		if(mMyRegions.count(region) == 0 && mEnemyRegions.count(region) == 0 && canReachSelf.count(region) != 0)
		{
			if(regionMyArmySupply[region] >= mRegionEnemyArmySupply[region])
			{
				mBorderPositions[PositionType::SafeTravelRegion].insert(BorderPosition(PositionType::SafeTravelRegion, region));
				mSafeRegions.insert(region);
			}
		}
	}
}

void BorderTrackerClass::drawDebugInfo(std::map<PositionType, std::set<BorderPosition>>::const_iterator it)
{
	if(it == mBorderPositions.end() || it->second.size() == 0)
		return;

	for(std::set<BorderPosition>::const_iterator bp = it->second.begin(); bp != it->second.end(); ++bp)
	{
		switch(bp->mType.underlying())
		{
		case PositionType::ContainChokepoint: bp->mChoke->draw(BWAPI::Colors::Yellow); break;
		case PositionType::ForwardChokepoint: bp->mChoke->draw(BWAPI::Colors::Blue); break;
		case PositionType::EnemyChokepoint: bp->mChoke->draw(BWAPI::Colors::Red); break;
		case PositionType::DefenseChokepoint: bp->mChoke->draw(BWAPI::Colors::Green); break;
		case PositionType::EnemyControlRegion: bp->mRegion->draw(BWAPI::Colors::Purple); break;
		case PositionType::BotControlRegion: bp->mRegion->draw(BWAPI::Colors::Grey); break;
		case PositionType::ForwardRegion: bp->mRegion->draw(BWAPI::Colors::Orange); break;
		case PositionType::SafeTravelRegion: bp->mRegion->draw(BWAPI::Colors::Teal); break;
		}
	}
}