#include "ScoutData.h"
#include "MapHelper.h"
#include "BaseTracker.h"

#include <limits>

ScoutDataClass::ScoutDataClass()
	: mType(ScoutType::None)
	, mBase()
{
}

ScoutDataClass::ScoutDataClass(Base base, ScoutType type)
	: mType(type)
	, mBase(base)
{
	loadPositions();
}

void ScoutDataClass::update()
{
	checkPositions();
}

bool ScoutDataClass::isAchieved()
{
	return mPositions.empty();
}

double ScoutDataClass::getDistance(Unit unit)
{
	if(mPositions.size() == 1)
		return (*mPositions.begin()).getApproxDistance(unit->getPosition());
	else
		return getNextPosition(unit->getPosition()).getApproxDistance(unit->getPosition());
}

Position ScoutDataClass::getNextPosition(Position pos)
{
	if(mPositions.size() == 1)
		return *mPositions.begin();
	else if(mPositions.empty())
		return pos;
	else
	{
		Position closestPosition = BWAPI::Positions::None;
		double closestDistance = std::numeric_limits<double>::max();

		for each(Position position in mPositions)
		{
			double distance = pos == BWAPI::Positions::None ? position.getApproxDistance(mBase->getCenterLocation()) : position.getApproxDistance(pos);
			if(distance < closestDistance)
			{
				closestDistance = distance;
				closestPosition = position;
			}
		}

		return closestPosition;
	}
}

Position ScoutDataClass::getLastPosition(Position pos)
{
	if(mPositions.size() == 1)
		return *mPositions.begin();
	else if(mPositions.empty())
		return pos;
	else
	{
		Position closestPosition = BWAPI::Positions::None;
		double closestDistance = 0;

		for each(Position position in mPositions)
		{
			double distance = pos == BWAPI::Positions::None ? position.getApproxDistance(mBase->getCenterLocation()) : position.getApproxDistance(pos);
			if(distance > closestDistance)
			{
				closestDistance = distance;
				closestPosition = position;
			}
		}

		return closestPosition;
	}
}

void ScoutDataClass::loadPositions()
{
	switch(mType.underlying())
	{
	case ScoutType::InitialBaseLocationScout:
		{
			mPositions.insert(Position(mBase->getCenterBuildLocation().x*32+16, mBase->getCenterBuildLocation().y*32+16));
			break;
		}
	case ScoutType::TechSearch:
	case ScoutType::BaseSearch:
	case ScoutType::BaseScout:
		{
			mPositions.insert(mBase->getCenterLocation());
			break;
		}
	case ScoutType::ThoroughSearch:
		{
			for each(TilePosition tile in mBase->getTiles())
			{
				if(BWAPI::Broodwar->isBuildable(tile))
					mPositions.insert(Position(tile.x*32+16, tile.y*32+16));
			}
			break;
		}
	}

	checkPositions();
}

void ScoutDataClass::checkPositions()
{
	for(std::set<Position>::iterator it = mPositions.begin(); it != mPositions.end();)
	{
		if(mType == ScoutType::InitialBaseLocationScout)
		{
			if(MapHelper::isAnyVisible(mBase->getCenterBuildLocation(), BWAPI::UnitTypes::Terran_Command_Center) || !BaseTracker::Instance().getEnemyBases().empty())
				mPositions.erase(it++);
			else
				++it;
		}
		else
		{
			if(BWAPI::Broodwar->isVisible(TilePosition(*it)))
				mPositions.erase(it++);
			else
				++it;
		}
	}
}

void ScoutDataClass::drawDebug(BWAPI::Color colour)
{
	for each(Position position in mPositions)
	{
		BWAPI::Broodwar->drawCircleMap(position.x, position.y, 16, colour);
	}
}