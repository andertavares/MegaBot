#pragma once
#include "strategyselector.h"
#include "BWAPI.h"
class EpsilonGreedy : public StrategySelector {

	void discountCrashes();
	BWAPI::AIModule* randomUniform();

public:
	EpsilonGreedy(void);
	~EpsilonGreedy(void);

	virtual void onStart();
	virtual void onFrame();
};

