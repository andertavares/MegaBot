#pragma once
#include "MetaStrategy.h"
#include "BWAPI.h"

class Probabilistic :
	public MetaStrategy
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

