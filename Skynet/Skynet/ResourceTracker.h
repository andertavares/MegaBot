#pragma once

#include "Interface.h"

#include "Singleton.h"

class ResourceTrackerClass
{
public:
	ResourceTrackerClass();

	void reservePlannedMinerals(int time, int amount);
	void reservePlannedGas(int time, int amount);
	void reservePlannedSupply(int time, int amount);
	void reset();

	void reserveCurrentMinerals(int time, int amount);
	void reserveCurrentGas(int time, int amount);
	void reserveCurrentSupply(int time, int amount);

	void releaseCurrentMinerals(int time, int amount);
	void releaseCurrentGas(int time, int amount);
	void releaseCurrentSupply(int time, int amount);

	int earliestMineralAvailability(int amount);
	int earliestGasAvailability(int amount);
	int earliestSupplyAvailability(int amount);

	int availableMineralAtTime(int time);
	int availableGasAtTime(int time);
	int availableSupplyAtTime(int time);

	int totalMineralAtTime(int time);
	int totalGasAtTime(int time);
	int totalSupplyAtTime(int time);

	double getMineralRate() { return mMineralRate; }
	double getGasRate() { return mGasRate; }
	void setMineralRate(double rate) { mMineralRate = rate; }
	void setGasRate(double rate) { mGasRate = rate; }

private:
	std::map<int, int> mSupplyTime;
	std::map<int, int> mFreeSupply;
	
	// Time to Reserved resources, reset each frame
	std::map<int, int> mPlannedReservedMinerals;
	std::map<int, int> mPlannedReservedGas;
	std::map<int, int> mPlannedReservedSupply;

	std::map<int, int> mCurrentReservedMinerals;
	std::map<int, int> mCurrentReservedGas;
	std::map<int, int> mCurrentReservedSupply;

	double mMineralRate;
	double mGasRate;
};

typedef Singleton<ResourceTrackerClass> ResourceTracker;