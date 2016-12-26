#pragma once
#include "strategyselector.h"
#include "BWAPI.h"

class Probabilistic :
	public StrategySelector
{
public:
	Probabilistic(void);
	~Probabilistic(void);

	virtual void onStart();

	/* 
	 * Performs a weighted selection based on 
	 * probabilities defined in a file and 
	 * returns the corresponding strategy
	 */
	BWAPI::AIModule* selectStrategy();
};

