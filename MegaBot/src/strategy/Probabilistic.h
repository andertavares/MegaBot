#pragma once
#include "strategyselector.h"
class Probabilistic :
	public StrategySelector
{
public:
	Probabilistic(void);
	~Probabilistic(void);

	virtual void onStart();
};

