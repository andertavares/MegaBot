#include "ResourceTracker.h"

#include <math.h>
#include <cassert>

#include "UnitTracker.h"
#include "Requirement.h"
#include "Logger.h"

ResourceTrackerClass::ResourceTrackerClass()
{
}

void ResourceTrackerClass::reservePlannedMinerals(int time, int amount)
{
	if(time == Requirement::maxTime || amount == 0)
		return;

	mPlannedReservedMinerals[time] += amount;
}

void ResourceTrackerClass::reservePlannedGas(int time, int amount)
{
	if(time == Requirement::maxTime || amount == 0)
		return;

	mPlannedReservedGas[time] += amount;
}

void ResourceTrackerClass::reservePlannedSupply(int time, int amount)
{
	if(time == Requirement::maxTime || amount == 0)
		return;

	mPlannedReservedSupply[time] += amount;

	for(std::map<int, int>::reverse_iterator i = mFreeSupply.rbegin(); i != mFreeSupply.rend(); ++i)
	{
		if(i->first <= time)
		{
			i->second -= amount;
			if(i->second <= 0)
			{
				amount = -i->second;
				i->second = 0;

				if(amount == 0)
					return;
			}
			else
				return;
		}
	}
	for(std::map<int, int>::iterator i = mFreeSupply.begin(); i != mFreeSupply.end(); ++i)
	{
		i->second -= amount;
		if(i->second <= 0)
		{
			amount = -i->second;
			i->second = 0;

			if(amount == 0)
				return;
		}
		else
			return;
	}

	LOGMESSAGEWARNING("It tried to reserve supply it didn't have");
}

void ResourceTrackerClass::reset()
{
	mPlannedReservedMinerals.clear();
	mPlannedReservedGas.clear();
	mPlannedReservedSupply.clear();
	mFreeSupply.clear();
	mSupplyTime.clear();

	int timeNow = BWAPI::Broodwar->getFrameCount();
	for(std::map<int, int>::iterator it = mCurrentReservedMinerals.begin(); it != mCurrentReservedMinerals.end();)
	{
		if(it->second <= 0)
			mCurrentReservedMinerals.erase(it++);
		else
			++it;
	}
	mPlannedReservedMinerals = mCurrentReservedMinerals;

	for(std::map<int, int>::iterator it = mCurrentReservedGas.begin(); it != mCurrentReservedGas.end();)
	{
		if(it->second <= 0)
			mCurrentReservedGas.erase(it++);
		else
			++it;
	}
	mPlannedReservedGas = mCurrentReservedGas;
	
	// Calculate free supply we have in the future
	for each(Unit unit in UnitTracker::Instance().getSupplyProviders())//TODO: hatcheries and lairs morphing return false but still give supply
	{
		if(unit->isCompleted())
			mFreeSupply[timeNow] += unit->getType().supplyProvided();
		else
			mFreeSupply[unit->getCompletedTime()] += unit->getType().supplyProvided();
	}

	// Cap it at 400
	int supplyTotal = 0;
	for(std::map<int, int>::iterator it = mFreeSupply.begin(); it != mFreeSupply.end(); ++it)
	{
		supplyTotal += it->second;
		if(supplyTotal > 400)
		{
			int difference = supplyTotal - 400;
			it->second -= difference;
			supplyTotal = 400;
		}
	}
	mSupplyTime = mFreeSupply;

	// Remove the supply we are already using and that we plan to use
	int supplyUsed = BWAPI::Broodwar->self()->supplyUsed();
	supplyTotal = 0;
	for(std::map<int, int>::iterator it = mFreeSupply.begin(); it != mFreeSupply.end();)
	{
		if(supplyUsed > 0)
		{
			int supplyToUse = std::min(supplyUsed, it->second);
			it->second -= supplyToUse;
			supplyUsed -= supplyToUse;
		}

		supplyTotal += it->second;

		if(it->second == 0)
			mFreeSupply.erase(it++);
		else
			++it;
	}

	for(std::map<int, int>::iterator i = mCurrentReservedSupply.begin(); i != mCurrentReservedSupply.end();)
	{
		if(i->second > 0)
		{
			int supplyToTake = std::min(supplyTotal, i->second);
			supplyTotal -= supplyToTake;
			reservePlannedSupply(i->first, supplyToTake);
			
			++i;
		}
		else
			mCurrentReservedSupply.erase(i++);
	}
}

void ResourceTrackerClass::reserveCurrentMinerals(int time, int amount)
{
	if(time == Requirement::maxTime || amount == 0)
		return;

	mCurrentReservedMinerals[time] += amount;
	reservePlannedMinerals(time, amount);
}

void ResourceTrackerClass::reserveCurrentGas(int time, int amount)
{
	if(time == Requirement::maxTime || amount == 0)
		return;

	mCurrentReservedGas[time] += amount;
	reservePlannedGas(time, amount);
}

void ResourceTrackerClass::reserveCurrentSupply(int time, int amount)
{
	if(time == Requirement::maxTime || amount == 0)
		return;

	mCurrentReservedSupply[time] += amount;
	reservePlannedSupply(time, amount);
}

void ResourceTrackerClass::releaseCurrentMinerals(int time, int amount)
{
	mCurrentReservedMinerals[time] -= amount;
}

void ResourceTrackerClass::releaseCurrentGas(int time, int amount)
{
	mCurrentReservedGas[time] -= amount;
}

void ResourceTrackerClass::releaseCurrentSupply(int time, int amount)
{
	mCurrentReservedSupply[time] -= amount;
}

int ResourceTrackerClass::earliestMineralAvailability(int amount)
{
	double freeMinerals = BWAPI::Broodwar->self()->minerals();

	bool hasSpace = false;
	if(freeMinerals >= amount)
	{
		freeMinerals -= amount;
		hasSpace = true;
	}

	int earliestTime = BWAPI::Broodwar->getFrameCount();
	int lastTime = BWAPI::Broodwar->getFrameCount();
	for(std::map<int, int>::iterator i = mPlannedReservedMinerals.begin(); i != mPlannedReservedMinerals.end(); ++i)
	{
		freeMinerals -= i->second;

		if(i->first >= lastTime)
		{
			int timePassed = i->first - lastTime;
			lastTime = i->first;
			freeMinerals += timePassed * mMineralRate;
		}

		if(!hasSpace && freeMinerals >= amount)
		{
			if(mMineralRate == 0)
				return Requirement::maxTime;

			freeMinerals -= amount;
			hasSpace = true;
			earliestTime = int(i->first - (freeMinerals / mMineralRate));
		}
		else if(freeMinerals < 0 && hasSpace)
		{
			hasSpace = false;
			freeMinerals += amount;
		}
	}

	if(!hasSpace)
	{
		if(mMineralRate == 0)
			return Requirement::maxTime;

		freeMinerals -= amount;
		earliestTime = lastTime + int(-freeMinerals / mMineralRate);
	}

	return earliestTime;
}

int ResourceTrackerClass::earliestGasAvailability(int amount)
{
	double freeGas = BWAPI::Broodwar->self()->gas();

	bool hasSpace = false;
	if(freeGas >= amount)
	{
		freeGas -= amount;
		hasSpace = true;
	}

	int earliestTime = BWAPI::Broodwar->getFrameCount();
	int lastTime = BWAPI::Broodwar->getFrameCount();
	for(std::map<int, int>::iterator i = mPlannedReservedGas.begin(); i != mPlannedReservedGas.end(); ++i)
	{
		freeGas -= i->second;

		if(i->first >= lastTime)
		{
			int timePassed = i->first - lastTime;
			lastTime = i->first;
			freeGas += timePassed * mGasRate;
		}

		if(!hasSpace && freeGas >= amount)
		{
			if(mGasRate == 0)
				return Requirement::maxTime;

			freeGas -= amount;
			hasSpace = true;
			earliestTime = int(i->first - (freeGas / mGasRate));
		}
		else if(freeGas < 0 && hasSpace)
		{
			hasSpace = false;
			freeGas += amount;
		}
	}

	if(!hasSpace)
	{
		if(mGasRate == 0)
			return Requirement::maxTime;

		freeGas -= amount;
		earliestTime = lastTime + int(-freeGas / mGasRate);
	}

	return earliestTime;
}

int ResourceTrackerClass::earliestSupplyAvailability(int amount)
{
	int supplyTotal = 0;
	for(std::map<int, int>::iterator i = mFreeSupply.begin(); i != mFreeSupply.end(); ++i)
	{
		supplyTotal += i->second;
		if(supplyTotal >= amount)
			return i->first;
	}

	return Requirement::maxTime;
}

int ResourceTrackerClass::availableMineralAtTime(int time)
{
	double freeMinerals = BWAPI::Broodwar->self()->minerals();

	int lastTime = BWAPI::Broodwar->getFrameCount();
	for(std::map<int, int>::iterator i = mPlannedReservedMinerals.begin(); i != mPlannedReservedMinerals.end(); ++i)
	{
		if(i->first > time)
			break;

		freeMinerals -= i->second;

		if(i->first >= lastTime)
		{
			int timePassed = i->first - lastTime;
			lastTime = i->first;
			freeMinerals += timePassed * mMineralRate;
		}
	}

	if(time > lastTime)
	{
		int timePassed = time - lastTime;
		freeMinerals += timePassed * mMineralRate;
	}

	return int(freeMinerals);
}

int ResourceTrackerClass::availableGasAtTime(int time)
{
	double freeGas = BWAPI::Broodwar->self()->gas();

	int lastTime = BWAPI::Broodwar->getFrameCount();
	for(std::map<int, int>::iterator i = mPlannedReservedGas.begin(); i != mPlannedReservedGas.end(); ++i)
	{
		if(i->first > time)
			break;

		freeGas -= i->second;

		if(i->first >= lastTime)
		{
			int timePassed = i->first - lastTime;
			lastTime = i->first;
			freeGas += timePassed * mGasRate;
		}
	}

	if(time > lastTime)
	{
		int timePassed = time - lastTime;
		freeGas += timePassed * mGasRate;
	}

	return int(freeGas);
}

int ResourceTrackerClass::availableSupplyAtTime(int time)
{
	int total = totalSupplyAtTime(time);

	for(std::map<int, int>::iterator i = mPlannedReservedSupply.begin(); i != mPlannedReservedSupply.end(); ++i)
	{
		if(i->first > time)
			break;
		else
			total -= i->second;
	}

	total -= BWAPI::Broodwar->self()->supplyUsed();

	return total;
}

int ResourceTrackerClass::totalMineralAtTime(int time)
{
	return BWAPI::Broodwar->self()->minerals() + int((time - BWAPI::Broodwar->getFrameCount()) * mMineralRate);
}

int ResourceTrackerClass::totalGasAtTime(int time)
{
	return BWAPI::Broodwar->self()->gas() + int((time - BWAPI::Broodwar->getFrameCount()) * mGasRate);
}

int ResourceTrackerClass::totalSupplyAtTime(int time)
{
	int total = 0;
	for(std::map<int, int>::iterator i = mSupplyTime.begin(); i != mSupplyTime.end(); ++i)
	{
		if(i->first > time)
			break;
		else
			total += i->second;
	}

	return total;
}