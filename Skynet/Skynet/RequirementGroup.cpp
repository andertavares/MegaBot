#include "RequirementGroup.h"

#include <assert.h>

using namespace SkynetBot;

RequirementGroup::RequirementGroup()
{
}

std::pair<int, int> RequirementGroup::earliestTime()
{
	int latestTime = BWAPI::Broodwar->getFrameCount();

	// Loop through all normal requirements
	for(std::vector<Requirement>::iterator requirement = mOtherRequirements.begin(); requirement != mOtherRequirements.end(); ++requirement)
	{
		assert(!requirement->unitRequirement());

		// If the easliest time this requirement is ready is the latest yet, save it
		int thisTime = requirement->earliestTime();
		if(thisTime > latestTime)
			latestTime = thisTime;
	}

	// Recurse through unit requirements
	std::set<Unit> units;
	if(recurseForUnitTime(latestTime, mUnitRequirements.begin(), latestTime, Requirement::maxTime, units))
	{
		int biggestDelay = 0;
		// now loop through to find any delay in the execution
		for(std::vector<Requirement>::iterator requirement = mUnitRequirements.begin(); requirement != mUnitRequirements.end(); ++requirement)
		{
			int thisDelay = requirement->getDelay();
			if(thisDelay > biggestDelay)
				biggestDelay = thisDelay;
		}

		return std::make_pair(latestTime, biggestDelay);
	}

	// if it didn't find a time, just return
	return std::make_pair(Requirement::maxTime, 0);
}

bool RequirementGroup::recurseForUnitTime(int &latestTime, std::vector<Requirement>::iterator currentRequirement, int currentStartTime, int currentEndTime, std::set<Unit> &currentUnits)
{
	// If we have reached the end we have found a time, save it and return
	if(currentRequirement == mUnitRequirements.end())
	{
		latestTime = currentStartTime;
		return true;
	}

	assert(currentRequirement->unitRequirement());

	// get a selection of free times for this requirement
	const std::map<int, int> &unitTimes = currentRequirement->earliestUnitTime(currentStartTime, currentEndTime, currentUnits);

	// For each one
	for(std::map<int, int>::const_iterator it = unitTimes.begin(); it != unitTimes.end(); ++it)
	{
		currentUnits.insert(currentRequirement->getUnit());

		int newEndTime = it->second;
		if(newEndTime != Requirement::maxTime)
			newEndTime += it->first;

		// go to the next requirement with these times to see if it is suitable for the rest too
		if(recurseForUnitTime(latestTime, ++(std::vector<Requirement>::iterator(currentRequirement)), it->first, newEndTime, currentUnits))
			return true;

		currentUnits.erase(currentRequirement->getUnit());
	}

	return false;
}

bool RequirementGroup::operator==(const RequirementGroup& other) const
{
	if(mOtherRequirements == other.mOtherRequirements)
		return mUnitRequirements == other.mUnitRequirements;

	return false;
}

bool RequirementGroup::operator<(const RequirementGroup& other) const
{
	if(mOtherRequirements < other.mOtherRequirements)
		return true;

	return mUnitRequirements < other.mUnitRequirements;
}

bool RequirementGroup::empty() const
{
	if(!mOtherRequirements.empty())
		return false;

	return mUnitRequirements.empty();
}

void RequirementGroup::clear()
{
	mOtherRequirements.clear();
	mUnitRequirements.clear();
}

UnitGroup RequirementGroup::getUnits() const
{
	UnitGroup units;
	for(std::vector<Requirement>::const_iterator requirement = mUnitRequirements.begin(); requirement != mUnitRequirements.end(); ++requirement)
		units.insert(requirement->getUnit());

	return units;
}

void RequirementGroup::reserve(int time)
{
	for(std::vector<Requirement>::iterator requirement = mOtherRequirements.begin(); requirement != mOtherRequirements.end(); ++requirement)
		requirement->reserve(time);

	for(std::vector<Requirement>::iterator requirement = mUnitRequirements.begin(); requirement != mUnitRequirements.end(); ++requirement)
		requirement->reserve(time);
}

void RequirementGroup::addUnitFilterRequirement(int priority, int duration, UnitFilter unitFilter, int quantity, Position position)
{
	for(int i = 0; i < quantity; ++i)
		addUnitFilterRequirement(priority, duration, unitFilter, position);
}