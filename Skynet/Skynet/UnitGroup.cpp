#include "UnitGroup.h"

#include <limits>

#include "TerrainAnaysis.h"

using namespace SkynetBot;

UnitGroup::UnitGroup()
{
}

int UnitGroup::countCompletedBy(int time) const
{
	const int &timeNow = BWAPI::Broodwar->getFrameCount();

	int total = 0;
	for each(Unit unit in mUnits)
	{
		if(unit->isCompleted() || unit->getCompletedTime() <= time)
			++total;
	}
	return total;
}

std::set<Unit>::iterator UnitGroup::begin()
{
	return mUnits.begin();
}

std::set<Unit>::const_iterator UnitGroup::begin() const
{
	return mUnits.begin();
}

std::set<Unit>::iterator UnitGroup::end()
{
	return mUnits.end();
}

std::set<Unit>::const_iterator UnitGroup::end() const
{
	return mUnits.end();
}

void UnitGroup::clear()
{
	mUnits.clear();
}

std::set<Unit>::size_type UnitGroup::count(Unit const &unit) const
{
	return mUnits.count(unit);
}

bool UnitGroup::empty() const
{
	return mUnits.empty();
}

void UnitGroup::erase(std::set<Unit>::iterator position)
{
	mUnits.erase(position);
}

std::set<Unit>::size_type UnitGroup::erase(Unit const &unit)
{
	return mUnits.erase(unit);
}

void UnitGroup::erase(std::set<Unit>::iterator first, std::set<Unit>::iterator last)
{
	mUnits.erase(first, last);
}

std::set<Unit>::iterator UnitGroup::find(Unit const &unit)
{
	return mUnits.find(unit);
}

std::set<Unit>::const_iterator UnitGroup::find(Unit const &unit) const
{
	return mUnits.find(unit);
}

std::pair<std::set<Unit>::iterator, bool> UnitGroup::insert(Unit const &unit)
{
	return mUnits.insert(unit);
}

std::set<Unit>::iterator UnitGroup::insert(std::set<Unit>::iterator position, Unit const &unit)
{
	return mUnits.insert(position, unit);
}

void UnitGroup::insert(std::set<Unit>::const_iterator first, std::set<Unit>::const_iterator last)
{
	mUnits.insert(first, last);
}

std::set<Unit>::size_type UnitGroup::size() const
{
	return mUnits.size();
}

UnitGroup::operator std::set<Unit>() const
{
	return mUnits;
}

int UnitGroup::ratingDifference(const UnitGroup &other) const
{
	// not sure about this, along the right lines with using time to kill as that in turns considers weapon damage, terrain, health etc but bad atm
	if(mUnits.empty() && other.mUnits.empty())
		return 0;
	else if(mUnits.empty())
		return -100;
	else if(other.mUnits.empty())
		return 100;

	int OtherKillThisTime = 0;
	int ThisKillOtherTime = 0;
	for each(Unit thisUnit in mUnits)
	{
		for each(Unit otherUnit in other.mUnits)
		{
			if(otherUnit->canAttack(thisUnit) && !thisUnit->isUnderDarkSwarm() && otherUnit->getDistance(thisUnit) < 600)
			{
				int killTime = otherUnit->getTimeToKill(thisUnit);
				if(otherUnit->getType().groundWeapon().outerSplashRadius() > 15)
					killTime /= 2;

				OtherKillThisTime += std::min(killTime, 500);
			}
			else
				OtherKillThisTime += 500;

			if(thisUnit->canAttack(otherUnit) && !otherUnit->isUnderDarkSwarm() && thisUnit->getDistance(otherUnit) < 600)
				ThisKillOtherTime += std::min(thisUnit->getTimeToKill(otherUnit), 500);
			else
				ThisKillOtherTime += 500;
		}
	}

	OtherKillThisTime /= other.mUnits.size();
	ThisKillOtherTime /= mUnits.size();

	int ratingTotal = std::max(OtherKillThisTime, ThisKillOtherTime) - std::min(OtherKillThisTime, ThisKillOtherTime);
	if(OtherKillThisTime < ThisKillOtherTime)
		ratingTotal = 0 - ratingTotal;

	return ratingTotal;
}

Position UnitGroup::getCenter() const
{
	if(empty())
		return BWAPI::Positions::None;

	if(size() == 1)
		return (*begin())->getPosition();

	Vector position;

	for each(Unit unit in mUnits)
	{
		position += Vector(unit->getPosition());
	}

	return position / float(size());
}

bool UnitGroup::operator==(const UnitGroup &other) const
{
	return mUnits == other.mUnits;
}

bool UnitGroup::operator!=(const UnitGroup &other) const
{
	return mUnits != other.mUnits;
}

UnitGroup &UnitGroup::operator+=(const UnitGroup &other)
{
	for each(Unit unit in other)
	{
		insert(unit);
	}

	return *this;
}

UnitGroup UnitGroup::getBestFittingToCircle(int circleSize, int inFramesTime) const
{
	UnitGroup circleUnits(*this);
	bool removed = false;
	do
	{
		removed = false;
		Position center = circleUnits.getCenter();

		Unit furthestUnit;
		int furthestDistance = 0;

		for each(Unit unit in circleUnits)
		{
			int distanceToCenter;
			if(inFramesTime == 0)
				distanceToCenter = unit->getDistance(center);
			else
				distanceToCenter = unit->getDistance(center, inFramesTime);

			if(distanceToCenter > circleSize && distanceToCenter > furthestDistance)
			{
				furthestUnit = unit;
				furthestDistance = distanceToCenter;
			}
		}

		if(furthestUnit)
		{
			circleUnits.erase(furthestUnit);
			removed = true;
		}
	}
	while(removed);

	return circleUnits;
}

Unit UnitGroup::getClosestUnit(Unit unit) const
{
	Unit closestUnit;
	int closestDistance = std::numeric_limits<int>::max();

	for each(Unit groupUnit in mUnits)
	{
		int thisDistance = unit->getDistance(groupUnit);
		if(thisDistance < closestDistance)
		{
			closestDistance = thisDistance;
			closestUnit = groupUnit;
		}
	}

	return closestUnit;
}

Unit UnitGroup::getClosestUnit(Position position) const
{
	Unit closestUnit;
	int closestDistance = std::numeric_limits<int>::max();

	for each(Unit groupUnit in mUnits)
	{
		int thisDistance = groupUnit->getDistance(position);
		if(thisDistance < closestDistance)
		{
			closestDistance = thisDistance;
			closestUnit = groupUnit;
		}
	}

	return closestUnit;
}

bool UnitGroup::isAnyInRange(const Unit &unit) const
{
	for each(Unit groupUnit in mUnits)
	{
		if(groupUnit->isInRange(unit))
			return true;
	}

	return false;
}

bool UnitGroup::isAnyInRange(const UnitGroup &otherGroup) const
{
	for each(Unit groupUnit in mUnits)
	{
		for each(Unit otherGroupUnit in otherGroup)
		{
			if(groupUnit->isInRange(otherGroupUnit))
				return true;
		}
	}

	return false;
}

int UnitGroup::minDistanceBetween(const UnitGroup &otherGroup) const
{
	int minDistance = std::numeric_limits<int>::max();
	for each(Unit groupUnit in mUnits)
	{
		for each(Unit otherGroupUnit in otherGroup)
		{
			int thisDistance = otherGroupUnit->getDistance(groupUnit);
			if(thisDistance < minDistance)
				minDistance = thisDistance;
		}
	}
	return minDistance;
}

UnitGroup UnitGroup::operator+(const UnitGroup &other) const
{
	UnitGroup ret(*this);
	ret += other;

	return ret;
}

int UnitGroup::getBuildScore() const
{
	int rating = 0;
	for each(Unit unit in mUnits)
	{
		rating += unit->getType().buildScore();
	}

	return rating;
}

bool UnitGroup::isWorthEngaging(const UnitGroup &other) const
{
	// Don't chase after a faster group
	double thisAverageSpeed = getAverageSpeed();
	double otherAverageSpeed = other.getAverageSpeed();
	if(otherAverageSpeed >= thisAverageSpeed)
		return false;

	size_t thisFlyingCount = getFlyingCount();
	double thisFlyingAverage = 0.0;
	if(!empty())
		thisFlyingAverage = thisFlyingCount / size();

	size_t otherFlyingCount = other.getFlyingCount();
	double otherFlyingAverage = 0.0;
	if(!other.empty())
		otherFlyingAverage = otherFlyingCount / other.size();

	if((thisFlyingAverage + 0.3) < otherFlyingAverage)
		return false;

	Region thisRegion = getMajorityRegion();
	Region otherRegion = other.getMajorityRegion();
	if(thisRegion && thisRegion != otherRegion)
	{
		// dont try attacking a group that isnt connected without a majority of flying units
		if(!otherRegion || thisRegion->isConnected(otherRegion))
		{
			if(thisFlyingAverage > 0.5)
				return false;
		}
	}

	size_t otherCloakedCount = other.getCloakedCount();
	if(!hasDetection() && otherCloakedCount > 0)
	{
		size_t thisCloakedCount = getCloakedCount();
		if(!other.hasDetection() && thisCloakedCount > 0)
		{
			if(thisCloakedCount < otherCloakedCount)
				return false;
		}
		else if(otherCloakedCount > 0)
			return false;
	}

	if(otherFlyingAverage > 0.2 && !canAttackAir())
		return false;

	return true;
}

int UnitGroup::getAverageTerrainHeight() const
{
	int height = 0;
	for each(Unit unit in mUnits)
	{
		height += BWAPI::Broodwar->getGroundHeight(unit->getTilePosition());
	}

	return height / mUnits.size();
}

double UnitGroup::getAverageSpeed() const
{
	double averageSpeed = 0;
	for each(Unit unit in mUnits)
	{
		averageSpeed += unit->getType().topSpeed();
	}

	return averageSpeed / double(mUnits.size());
}

Region UnitGroup::getMajorityRegion() const
{
	int currentCount = 1;
	Region currentCandidate;

	for each(Unit unit in mUnits)
	{
		const Region &thisRegion = TerrainAnaysis::Instance().getRegion(unit->getPosition());
		if(thisRegion == currentCandidate)
			++currentCount;
		else
			--currentCount;

		if(currentCount == 0)
		{
			currentCandidate = thisRegion;
			currentCount = 1;
		}
	}

	return currentCandidate;
}

bool UnitGroup::hasDetection() const
{
	for each(Unit unit in mUnits)
	{
		if(unit->getType().isDetector())
			return true;
	}

	return false;
}

std::set<Unit>::size_type UnitGroup::getCloakedCount() const
{
	std::set<Unit>::size_type cloakedCount = 0;
	for each(Unit unit in mUnits)
	{
		if(unit->getType().hasPermanentCloak() || unit->isCloaked())
			++cloakedCount;
	}

	return cloakedCount;
}

std::set<Unit>::size_type UnitGroup::getFlyingCount() const
{
	std::set<Unit>::size_type flyingCount = 0;
	for each(Unit unit in mUnits)
	{
		if(unit->getType().isFlyer() || unit->isLifted())
			++flyingCount;
	}

	return flyingCount;
}

bool UnitGroup::canAttackAir() const
{
	for each(Unit unit in mUnits)
	{
		if(unit->canAttackAir())
			return true;
	}

	return false;
}

bool UnitGroup::canAttackGround() const
{
	for each(Unit unit in mUnits)
	{
		if(unit->canAttackGround())
			return true;
	}

	return false;
}

bool UnitGroup::canMajorityAttack(const UnitGroup &other) const
{
	if(empty() || other.empty())
		return false;

	double thisGroupAttackCount = 0.0;
	for each(Unit thisUnit in mUnits)
	{
		for each(Unit otherUnit in other)
		{
			if(thisUnit->canAttack(otherUnit))
				++thisGroupAttackCount;
		}
	}

	thisGroupAttackCount /= other.size();
	thisGroupAttackCount /= size();

	return thisGroupAttackCount > 0.5;
}