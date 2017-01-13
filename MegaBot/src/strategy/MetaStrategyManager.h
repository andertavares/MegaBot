#pragma once
#include "MetaStrategy.h"

class MetaStrategyManager {
	MetaStrategyManager(void);
	~MetaStrategyManager(void);

public:
	/** Retrieves a meta-strategy defined in configuration file */
	static MetaStrategy* getMetaStrategy();
};

