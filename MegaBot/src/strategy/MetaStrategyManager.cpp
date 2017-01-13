#include "MetaStrategyManager.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "MetaStrategy.h"
#include "EpsilonGreedy.h"
#include "Probabilistic.h"
#include "RandomSwitch.h"
#include "SingleChoice.h"

MetaStrategy* MetaStrategyManager::getMetaStrategy(){
	//using namespace tinyxml2;

    //retrieve what config says about strategy
	string metaStrategyName = Configuration::getInstance()->metaStrategyID;

	Logging::getInstance()->log("Meta strategy: %s", metaStrategyName.c_str());

    if (metaStrategyName == "epsilon-greedy") {
        return new EpsilonGreedy();
    }
	else if (metaStrategyName == "probabilistic") {
		return new Probabilistic();
	}
	else if (metaStrategyName == "random-switch") {
		return new RandomSwitch();
	}
	
    else {	//otherwise, go to Single Choice using 'metaStrategyName' as the chosen behavior
		Logging::getInstance()->log("Using SingleChoice with strategy: %s", metaStrategyName.c_str());
		return new SingleChoice(metaStrategyName);
    }
	//return new EpsilonGreedy();	//failsafe default...
}

