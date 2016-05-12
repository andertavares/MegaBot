#include "WorkerScoutTask.h"
#include "ScoutManager.h"
#include "MapHelper.h"
#include "UnitInformation.h"
#include "PathFinder.h"
#include "UnitTracker.h"
#include "BaseTracker.h"

WorkerScoutTask::WorkerScoutTask(TaskType priority, ScoutData data)
	: Task(priority)
	, mData(data)
	, mFailTime(Requirement::maxTime)
{
}

int WorkerScoutTask::getEndTime() const
{
	return Requirement::maxTime;
}

int WorkerScoutTask::getEndTime(Unit unit) const
{
	return getEndTime();
}

Position WorkerScoutTask::getStartLocation(Unit unit) const
{
	if(mData)
		return mData->getNextPosition(unit->getPosition());

	return unit->getPosition();
}

Position WorkerScoutTask::getEndLocation(Unit unit) const
{
	if(mData)
		return mData->getLastPosition(unit->getPosition());

	return unit->getPosition();
}

bool WorkerScoutTask::preUpdate()
{
	if(!mData)
	{
		complete();
	}
	else if(mCurrentState == ScoutState::Searching)
	{
		if(mData->isAchieved())
		{
			if(mData->getBase()->isEnemyBase())
				mCurrentState = ScoutState::Harassing;
			else if(mData->getType() == ScoutType::InitialBaseLocationScout && !mData->getBase()->isEnemyBase())
				mCurrentState = ScoutState::DoubleCheck;
			else
				getNewData();
		}
		else if(mFailTime < BWAPI::Broodwar->getFrameCount())
		{
			ScoutManager::Instance().updateLastScoutType(mData, ScoutType::CouldNotReachWithGround);
			getNewData();
		}
	}
	else if(mCurrentState == ScoutState::Harassing)
	{
		if(!mData->getBase()->isEnemyBase() || (mUnit && BaseTracker::Instance().getBase(mUnit->getTilePosition()) != mData->getBase()))
			complete();
	}
	else if(mCurrentState == ScoutState::DoubleCheck)
	{
		if(mData->getBase()->isEnemyBase())
			mCurrentState = ScoutState::Harassing;
		else if(MapHelper::isAnyVisible(mData->getBase()->getCenterBuildLocation(), BWAPI::UnitTypes::Terran_Command_Center))
			getNewData();
	}

	return hasEnded() && mUnit;
}

bool WorkerScoutTask::update()
{
#ifdef SKYNET_DRAW_DEBUG
	if(mData)
		mData->drawDebug(BWAPI::Colors::Blue);
#endif

	//TODO: change to use goal / behavior system that squads use
	if(mUnit && mData)
	{
		if(mCurrentState == ScoutState::Searching)
		{
			Position target = mData->getNextPosition(mUnit->getPosition());

			if(target != BWAPI::Positions::None)
				mUnit->move(target);
		}
		else if(mCurrentState == ScoutState::Harassing)
		{
			bool doneSomething = false;
			const UnitGroup &targetting = UnitInformation::Instance().getUnitsTargetting(mUnit);
			bool targetsChanged = mLastUnitsTargetting != targetting;
			if(targetsChanged)
				mLastUnitsTargetting = targetting;

			//TODO: add units that could be targetting it, including enemy workers so it thinks they are scary and doesn't get surrounded attacking one of them

			if(mLastUnitsTargetting.size() > 0)
			{
				mHarrasTimer = 72;

				int targettingSupply = 0;
				for each(Unit unit in mLastUnitsTargetting)
				{
					if(!unit->getType().isWorker() && (unit->getType().groundWeapon().maxRange() > 32 || unit->getType().topSpeed() > mUnit->getType().topSpeed()))
						targettingSupply += unit->getType().supplyRequired();
				}

				//TODO: fight back against other workers if it has the advantage
				if(!doneSomething)
				{
					bool isEnemyToClose = false;
					for each(Unit unit in mLastUnitsTargetting)
					{
						if(unit->isInRange(mUnit))
						{
							isEnemyToClose = true;
							break;
						}
					}

					//If the enemy is too close, don't use the path as it lacks precision
					if(!isEnemyToClose)
					{
						if(targetsChanged || mTilePath.path.size() <= 7 || !mTilePath.isComplete || !mTilePath.isStillValid())
						{
							bool stickToRegion = targettingSupply == 0;
							if(mData->getBase()->getNumberOfTechBuildings() == 0 && BWAPI::Broodwar->getFrameCount() > 24*60*4)
								stickToRegion = false;

							mTilePath = PathFinder::Instance().CreateAdvancedFleePath(mUnit->getTilePosition(), mLastUnitsTargetting, stickToRegion);
						}

						if(mTilePath.isComplete)
						{
							while(!mTilePath.path.empty())
							{
								std::list<TilePosition>::iterator currentTile = mTilePath.path.begin();
								const Position pos(currentTile->x()*32 + 16, currentTile->y()*32 + 16);
								
								if(pos.getApproxDistance(mUnit->getPosition()) <= 64)
									mTilePath.path.erase(currentTile);
								else
								{
									mUnit->move(pos);

									doneSomething = true;
#ifdef SKYNET_DRAW_DEBUG
									BWAPI::Broodwar->drawTextMap(mUnit->getPosition().x(), mUnit->getPosition().y() - 10, "Fleeing with path");
									mTilePath.drawPath();
#endif
									break;
								}
							}
						}
					}
				}

				// If we haven't done anything to avoid targets, steer away from them
				if(!doneSomething)
				{
					mTilePath.path.clear();
					mTilePath.isComplete = false;

					//TODO: dont just use the the group center, 

					Vector direction = mUnit->getPosition() - mLastUnitsTargetting.getCenter();

					direction.normalise();
					direction *= float(TILE_SIZE * 4);
					direction += Vector(mUnit->getPosition());

					mUnit->move(direction);

					doneSomething = true;
#ifdef SKYNET_DRAW_DEBUG
					BWAPI::Broodwar->drawTextMap(mUnit->getPosition().x(), mUnit->getPosition().y() - 10, "Fleeing with vectors");
					BWAPI::Broodwar->drawLineMap(mUnit->getPosition().x(), mUnit->getPosition().y(), (int)direction.x, (int)direction.y, BWAPI::Colors::Red);
#endif
				}
			}

			if(!doneSomething)
			{
				mTilePath.path.clear();
				mTilePath.isComplete = false;

				// If we have been provoking units for abit plain out attack them
				if(mHarrasTimer == 0)
				{
					//TODO: change when better unit search functions are added
					bool oneConstructing = false;
					Unit closestUnit;
					double minDist = std::numeric_limits<double>::max();

					for each(Unit unit in UnitTracker::Instance().selectAllUnits(mData->getBase()->getPlayer()->getRace().getWorker(), mData->getBase()->getPlayer()))
					{
						if(mData->getBase() != BaseTracker::Instance().getBase(unit->getTilePosition()))
							continue;
						
						if(!oneConstructing || unit->isConstructing())
						{
							double thisDist = mUnit->getPosition().getDistance(unit->getPosition());
							if(thisDist < minDist || (oneConstructing && unit->isConstructing()))
							{
								oneConstructing = unit->isConstructing();
								closestUnit = unit;
								minDist = thisDist;
							}
						}
					}

					if(closestUnit)
					{
						doneSomething = true;
						mUnit->attack(closestUnit);
#ifdef SKYNET_DRAW_DEBUG
						BWAPI::Broodwar->drawTextMap(mUnit->getPosition().x(), mUnit->getPosition().y() - 10, "Attacking Unit");
#endif
					}
				}
			}

			if(!doneSomething)
			{
				if(mUnit->getDistance(mData->getBase()->getCenterLocation()) > 320)
				{
					doneSomething = true;
					mUnit->move(mData->getBase()->getCenterLocation());
#ifdef SKYNET_DRAW_DEBUG
					BWAPI::Broodwar->drawTextMap(mUnit->getPosition().x(), mUnit->getPosition().y() - 10, "Moving Closer to base");
#endif
				}
			}

			if(!doneSomething)
			{
				Unit unit = mData->getBase()->getClosestEnemyBuilding(mData->getBase()->getCenterLocation());

				if(unit)
				{
					if(mUnit->isInRange(unit))
					{
						doneSomething = true;
						mUnit->attack(unit);
					}
					else
					{
						doneSomething = true;
						mHarrasTimer = 72;
						mUnit->move(unit->getPosition());
					}

					if(mHarrasTimer > 0)
						--mHarrasTimer;
				}
			}
		}
		else if(mCurrentState == ScoutState::DoubleCheck)
			mUnit->move(mData->getBase()->getCenterLocation());
	}

	return hasEnded() && !mUnit;
}

bool WorkerScoutTask::waitingForUnit(Unit unit) const
{
	return false;
}

void WorkerScoutTask::giveUnit(Unit unit)
{
	assert(mUnit == StaticUnits::nullunit);
	mUnit = unit;
	calculateFailTime();
}

void WorkerScoutTask::returnUnit(Unit unit)
{
	assert(mUnit == unit);

	if(mData && !mUnit->exists())
	{
		ScoutManager::Instance().updateLastScoutType(mData, mCurrentState == ScoutState::Harassing ? ScoutType::BaseSearch : BWAPI::Broodwar->getFrameCount() < 8000 ? ScoutType::FailedWithGroundLow : ScoutType::FailedWithGroundHigh);
		complete();
	}
	
	mUnit = StaticUnits::nullunit;
}

bool WorkerScoutTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return true;
}

UnitGroup WorkerScoutTask::getFinishedUnits()
{
	UnitGroup returnUnits;

	returnUnits.insert(mUnit);

	return returnUnits;
}

int WorkerScoutTask::getPriority(Unit unit) const
{
	if(unit == mUnit && mCurrentState != ScoutState::Harassing)
		return 50;
	else
		return 30;
}

void WorkerScoutTask::updateRequirements()
{
	clearRequirements();

	if(!mUnit && !shouldReturnUnit() && mData)
	{
		mFailTime = BWAPI::Broodwar->getFrameCount() + 24*10;

		RequirementGroup requirementOne;
		requirementOne.addUnitFilterRequirement(30, Requirement::maxTime, UnitFilter(UnitFilterFlags::IsWorker) && UnitFilter(UnitFilterFlags::IsComplete), mData->getNextPosition());
		addRequirement(requirementOne);
	}
}

void WorkerScoutTask::getNewData()
{
	mData = ScoutManager::Instance().getScoutData(ScoutUnitType::Worker);
	if(!mData)
		complete();
	else if(mUnit && !mUnit->hasPath(mData->getNextPosition(mUnit->getPosition())))
	{
		ScoutManager::Instance().updateLastScoutType(mData, ScoutType::CouldNotReachWithGround);
		getNewData();
	}
	else
	{
		calculateFailTime();
		mCurrentState = ScoutState::Searching;
	}
}

void WorkerScoutTask::calculateFailTime()
{
	mFailTime = BWAPI::Broodwar->getFrameCount() + 24*60;

	if(mUnit)
	{
		int groundDistance = MapHelper::getGroundDistance(mUnit->getPosition(), mData->getNextPosition(mUnit->getPosition()));
		if(groundDistance != std::numeric_limits<int>::max())
			mFailTime = BWAPI::Broodwar->getFrameCount() + 24*10 + int(groundDistance / mUnit->getType().topSpeed());
	}
	
}