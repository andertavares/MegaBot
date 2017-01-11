#pragma once
#include "MetaStrategy.h"

class SingleChoice : public MetaStrategy {
	string choiceName;

public:
	SingleChoice(string choice);
	~SingleChoice(void);

	virtual void onStart();
};

