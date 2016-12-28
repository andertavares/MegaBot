#pragma once
#include "MetaStrategy.h"

class MetaStrategyFactory {
	MetaStrategyFactory(void);
	~MetaStrategyFactory(void);

public:
	/** Retrieves a meta-strategy defined in configuration file */
	static MetaStrategy* getMetaStrategy();
};

