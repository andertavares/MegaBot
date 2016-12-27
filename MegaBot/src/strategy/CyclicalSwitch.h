#pragma once
#include "StrategySelector.h"
class CyclicalSwitch : public StrategySelector {
public:
	CyclicalSwitch(void);
	~CyclicalSwitch(void);

	//acts every frame 
	virtual void onFrame();

	//initializes the portfolio of behaviors
	virtual void onStart();
};

