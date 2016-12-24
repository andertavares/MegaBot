#pragma once
#include "StrategySelector.h"

class MetaStrategyFactory {
	MetaStrategyFactory(void);
	~MetaStrategyFactory(void);

public:
	//retrieves a meta-strategy defined in configuration file
	static StrategySelector* getMetaStrategy();
};

