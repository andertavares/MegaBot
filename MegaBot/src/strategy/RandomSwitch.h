#pragma once
#include "StrategySelector.h"
class RandomSwitch : public StrategySelector {
public:
	RandomSwitch(void);
	~RandomSwitch(void);

	//acts every frame 
	virtual void onFrame();

	//initializes the portfolio of behaviors
	virtual void onStart();
};

