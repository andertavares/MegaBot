#pragma once
#include "strategyselector.h"
#include "BWAPI.h"
class EpsilonGreedy : public StrategySelector {

	/** Counts the number of crashes to reduce score during strategy selection */
	void discountCrashes();
	BWAPI::AIModule* randomUniform();

public:
	EpsilonGreedy(void);
	~EpsilonGreedy(void);

	virtual void onStart();
	virtual void onFrame();
};

