#include "MetaStrategyFactory.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "StrategySelector.h"
#include "EpsilonGreedy.h"
#include "Probabilistic.h"

StrategySelector* MetaStrategyFactory::getMetaStrategy(){
	using namespace tinyxml2;

    //retrieve what config says about strategy
    string strategyId = Configuration::getInstance()->strategyID;

	Logging::getInstance()->log("Meta strategy: %s", strategyId.c_str());

    if (strategyId == "epsilon-greedy") {
        return new EpsilonGreedy();
    }
	else if (strategyId == "probabilistic") {
		return new Probabilistic();
	}
    else {	//otherwise, logs an error because meta-strategy was not found 
		Logging::getInstance()->log("Error: unrecognized meta-strategy '%s'. Defaulting to EpsilonGreedy!", strategyId.c_str());
    }
	return new EpsilonGreedy();	//failsafe default...
}

