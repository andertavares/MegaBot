#include "Requirement.h"

#include <algorithm>
#include <boost/bind.hpp>

#include "Task.h"
#include "ResourceTracker.h"
#include "UnitTracker.h"
#include "TaskManager.h"

const int Requirement::maxTime = std::numeric_limits<int>::max();

Requirement::Requirement(RequirementType type, int amount)
	: mType(type)
	, mAmount(amount)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mUnitFilter()
	, mUnitType(BWAPI::UnitTypes::None)
	, mTechType(BWAPI::TechTypes::None)
	, mUpgradeType(BWAPI::UpgradeTypes::None)
	, mTask()
	, mPriority(0)
	, mDuration(0)
	, mDelay(0)
{
}

Requirement::Requirement(int priority, int duration, Unit unit, Position position)
	: mType(RequirementType::Unit)
	, mAmount(1)
	, mPosition(position)
	, mUnit(unit)
	, mUnitFilter()
	, mUnitType(BWAPI::UnitTypes::None)
	, mTechType(BWAPI::TechTypes::None)
	, mUpgradeType(BWAPI::UpgradeTypes::None)
	, mTask()
	, mPriority(priority)
	, mDuration(duration)
	, mDelay(0)
{
}

Requirement::Requirement(int priority, int duration, UnitFilter unitFilter, Position position)
	: mType(RequirementType::UnitFilter)
	, mAmount(1)
	, mPosition(position)
	, mUnit()
	, mUnitFilter(unitFilter)
	, mUnitType(BWAPI::UnitTypes::None)
	, mTechType(BWAPI::TechTypes::None)
	, mUpgradeType(BWAPI::UpgradeTypes::None)
	, mTask()
	, mPriority(priority)
	, mDuration(duration)
	, mDelay(0)
{
}

Requirement::Requirement(TaskPointer task)
	: mType(RequirementType::Task)
	, mAmount(0)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mUnitFilter()
	, mUnitType(BWAPI::UnitTypes::None)
	, mTechType(BWAPI::TechTypes::None)
	, mUpgradeType(BWAPI::UpgradeTypes::None)
	, mTask(task)
	, mPriority(0)
	, mDuration(0)
	, mDelay(0)
{
}

Requirement::Requirement(BWAPI::UnitType unit)
	: mType(RequirementType::RequiredForUnit)
	, mAmount(0)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mUnitFilter()
	, mUnitType(unit)
	, mTechType(BWAPI::TechTypes::None)
	, mUpgradeType(BWAPI::UpgradeTypes::None)
	, mTask()
	, mPriority(0)
	, mDuration(0)
	, mDelay(0)
{
}

Requirement::Requirement(BWAPI::TechType tech)
	: mType(RequirementType::RequiredForTech)
	, mAmount(0)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mUnitFilter()
	, mUnitType(BWAPI::UnitTypes::None)
	, mTechType(tech)
	, mUpgradeType(BWAPI::UpgradeTypes::None)
	, mTask()
	, mPriority(0)
	, mDuration(0)
	, mDelay(0)
{
}

Requirement::Requirement(BWAPI::UpgradeType upgrade, int level)
	: mType(RequirementType::RequiredForUpgrade)
	, mAmount(level)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mUnitFilter()
	, mUnitType(BWAPI::UnitTypes::None)
	, mTechType(BWAPI::TechTypes::None)
	, mUpgradeType(upgrade)
	, mTask()
	, mPriority(0)
	, mDuration(0)
	, mDelay(0)
{
}

bool Requirement::operator==(const Requirement& other) const
{
	if(mType != other.mType)
		return false;
	else if(mAmount != other.mAmount)
		return false;
	else if(mUnitFilter != other.mUnitFilter)
		return false;
	else if(mUnit != other.mUnit)
		return false;
	else if(mPosition != other.mPosition)
		return false;
	else if(mPriority != other.mPriority)
		return false;
	else if(mDuration != other.mDuration)
		return false;
	else if(mDelay != other.mDelay)
		return false;
	else if(mTask != other.mTask)
		return false;
	else if(mUnitType != other.mUnitType)
		return false;
	else if(mTechType != other.mTechType)
		return false;
	else if(mUpgradeType != other.mUpgradeType)
		return false;
	else
		return true;
}

bool Requirement::operator<(const Requirement& other) const
{
	if(mType < other.mType)
		return true;
	else if(mType != other.mType)
		return false;

	if(mAmount < other.mAmount)
		return true;
	else if(mAmount != other.mAmount)
		return false;

	if(mUnitFilter < other.mUnitFilter)
		return true;
	else if(mUnitFilter != other.mUnitFilter)
		return false;

	if(mUnit < other.mUnit)
		return true;
	else if(mUnit != other.mUnit)
		return false;

	if(mPosition < other.mPosition)
		return true;
	else if(mPosition != other.mPosition)
		return false;

	if(mPriority < other.mPriority)
		return true;
	else if(mPriority != other.mPriority)
		return false;

	if(mDuration < other.mDuration)
		return true;
	else if(mDuration != other.mDuration)
		return false;

	if(mDelay < other.mDelay)
		return true;
	else if(mDelay != other.mDelay)
		return false;

	if(mTask < other.mTask)
		return true;
	else if(mTask != other.mTask)
		return false;

	if(mUnitType < other.mUnitType)
		return true;
	else if(mUnitType != other.mUnitType)
		return false;

	if(mTechType < other.mTechType)
		return true;
	else if(mTechType != other.mTechType)
		return false;

	if(mUpgradeType < other.mUpgradeType)
		return true;
	else if(mUpgradeType != other.mUpgradeType)
		return false;

	return false;
}

int Requirement::earliestTime()
{
	if(mType == RequirementType::Mineral)
		return ResourceTracker::Instance().earliestMineralAvailability(mAmount);
	else if(mType == RequirementType::Gas)
		return ResourceTracker::Instance().earliestGasAvailability(mAmount);
	else if(mType == RequirementType::Supply)
		return ResourceTracker::Instance().earliestSupplyAvailability(mAmount);
	else if(mType == RequirementType::Time)
		return mAmount;
	else if(mType == RequirementType::Task)
		return mTask->getEndTime();
	else if(mType == RequirementType::RequiredForUnit)
	{
		int latestTime = 0;

		for(std::map<BWAPI::UnitType, int>::const_iterator it = mUnitType.requiredUnits().begin(); it != mUnitType.requiredUnits().end(); ++it)
			latestTime = std::max(latestTime, earliestTimeForType(it->first));

		return latestTime;
	}
	else if(mType == RequirementType::RequiredForUpgrade)
	{
		int latestTime = earliestTimeForType(mUpgradeType.whatUpgrades());

		if(mAmount > 1)
		{
			if(mUpgradeType == BWAPI::UpgradeTypes::Protoss_Ground_Weapons || mUpgradeType == BWAPI::UpgradeTypes::Protoss_Ground_Armor)
				latestTime = std::max(latestTime, earliestTimeForType(BWAPI::UnitTypes::Protoss_Templar_Archives));
			else if(mUpgradeType == BWAPI::UpgradeTypes::Protoss_Plasma_Shields)
				latestTime = std::max(latestTime, earliestTimeForType(BWAPI::UnitTypes::Protoss_Cybernetics_Core));
			else if(mUpgradeType == BWAPI::UpgradeTypes::Protoss_Air_Armor || mUpgradeType == BWAPI::UpgradeTypes::Protoss_Air_Weapons)
				latestTime = std::max(latestTime, earliestTimeForType(BWAPI::UnitTypes::Protoss_Fleet_Beacon));
		}

		return latestTime;
	}
	else if(mType == RequirementType::RequiredForTech)
		return earliestTimeForType(mTechType.whatResearches());

	return Requirement::maxTime;
}

std::map<int, int> Requirement::earliestUnitTime(int startTime, int endTime, std::set<Unit> &currentUnits)
{
	const bool &allowAnyBlockLength = mDuration == Requirement::maxTime;

	if(mType == RequirementType::Unit)
	{
		if(currentUnits.count(mUnit) > 0)
			return std::map<int, int>();

		if(mPosition != BWAPI::Positions::None)
			mDelay = int((mUnit->getPosition().getApproxDistance(mPosition) * 1.6) / mUnit->getType().topSpeed()) + 15;

		return TaskManager::Instance().earliestFreeTimes(mUnit, mPriority, startTime - mDelay, endTime, mDuration, allowAnyBlockLength);
	}

	std::map<int, int> bestTimes;
	int bestStartTime = Requirement::maxTime;
	int bestDelay = Requirement::maxTime;

	for each(Unit unit in UnitTracker::Instance().selectAllUnits())
	{
		if(currentUnits.count(unit) > 0)
			continue;

		if(!mUnitFilter.passesFilter(unit))//TODO: only checks if it currently passes the filter, but some it can tell when in the future it will pass, might be worth implementing
			continue;

		int delay = 0;
		if(mPosition != BWAPI::Positions::None)
		{
			if(!unit->hasPath(mPosition))
				continue;

			delay = int((unit->getPosition().getApproxDistance(mPosition) * 1.6) / unit->getType().topSpeed()) + 15;
		}

		// If the delay for this unit pushes it outside the endtime, continue
		if(mDuration != Requirement::maxTime && startTime + mDuration + delay > endTime)
			continue;

		const std::map<int, int> &currentTimes = TaskManager::Instance().earliestFreeTimes(unit, mPriority, startTime - delay, endTime, mDuration + delay, allowAnyBlockLength);
		if(!currentTimes.empty())
		{
			std::map<int, int> currentTimesPlusDelay;
			for(std::map<int, int>::const_iterator it = currentTimes.begin(); it != currentTimes.end(); ++it)
				currentTimesPlusDelay[it->first + delay] = it->second;

			// Find the earliest time in these blocks
			std::map<int, int>::const_iterator it = currentTimesPlusDelay.begin();
			if(it != currentTimesPlusDelay.end())
			{
				// Save it if its the best we have found, also prefer if the delay is shorter so it
				// doesn't choose a unit really far away even though it will make it in time
				if(it->first < bestStartTime || (it->first == bestStartTime && delay < bestDelay))
				{
					mUnit = unit;
					mDelay = delay;
					
					bestTimes = currentTimesPlusDelay;
					bestStartTime = it->first;
					bestDelay = delay;
				}
			}
		}
	}

	return bestTimes;
}

void Requirement::reserve(int frameTime)
{
	switch(mType.underlying())
	{
	case RequirementType::Mineral:
		ResourceTracker::Instance().reservePlannedMinerals(frameTime, mAmount);
		break;
	case RequirementType::Gas:
		ResourceTracker::Instance().reservePlannedGas(frameTime, mAmount);
		break;
	case RequirementType::Supply:
		ResourceTracker::Instance().reservePlannedSupply(frameTime, mAmount);
		break;
	case RequirementType::Unit:
	case RequirementType::UnitFilter:
		TaskManager::Instance().reserveUnit(mUnit, frameTime, mPriority, mDuration);
		break;
	}
}

int Requirement::earliestTimeForType(BWAPI::UnitType unitType)
{
	int thisTime = Requirement::maxTime;
	for each(Unit requiredUnit in UnitTracker::Instance().selectAllUnits(unitType))
	{
		if(requiredUnit->isCompleted())
			return BWAPI::Broodwar->getFrameCount();
		else
		{
			int completeTime = requiredUnit->getCompletedTime();
			if(completeTime < thisTime)
				thisTime = completeTime;
		}
	}

	return thisTime;
}