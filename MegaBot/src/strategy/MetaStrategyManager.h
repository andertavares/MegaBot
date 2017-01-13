#pragma once
#include "MetaStrategy.h"

class MetaStrategyManager {
	MetaStrategyManager(void);
	~MetaStrategyManager(void);

	/** The active meta-strategy */
	static MetaStrategy* activeMetaStrategy;

public:
	/** Returns the active meta-strategy or 
	  * creates one via definition in configuration file 
	  */
	static MetaStrategy* getMetaStrategy();
};

