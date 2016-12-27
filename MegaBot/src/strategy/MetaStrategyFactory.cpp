#include "MetaStrategyFactory.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "MetaStrategy.h"
#include "EpsilonGreedy.h"
#include "Probabilistic.h"
#include "RandomSwitch.h"

MetaStrategy* MetaStrategyFactory::getMetaStrategy(){
	using namespace tinyxml2;

    //retrieve what config says about strategy
    string metaStrategyName = Configuration::getInstance()->strategyID;

	Logging::getInstance()->log("Meta strategy: %s", metaStrategyName.c_str());

    if (metaStrategyName == "epsilon-greedy") {
        return new EpsilonGreedy();
    }
	else if (metaStrategyName == "probabilistic") {
		return new Probabilistic();
	}
	else if (metaStrategyName == "cyclical") {
		return new RandomSwitch();
	}
    else {	//otherwise, logs an error because meta-strategy was not found 
		Logging::getInstance()->log("Error: unrecognized meta-strategy '%s'. Defaulting to EpsilonGreedy!", metaStrategyName.c_str());
    }
	return new EpsilonGreedy();	//failsafe default...
}

