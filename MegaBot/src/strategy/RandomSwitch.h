#pragma once
#include "MetaStrategy.h"
class RandomSwitch : public MetaStrategy {
public:
	RandomSwitch(void);
	~RandomSwitch(void);

	//acts every frame 
	virtual void onFrame();

	//initializes the portfolio of behaviors
	virtual void onStart();
};

