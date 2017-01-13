#include "MetaStrategyManager.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "MetaStrategy.h"
#include "EpsilonGreedy.h"
#include "Probabilistic.h"
#include "RandomSwitch.h"
#include "SingleChoice.h"

MetaStrategy* MetaStrategyManager::activeMetaStrategy = NULL;

MetaStrategy* MetaStrategyManager::getMetaStrategy(){
	//initializes activeMetaStrategy if it has not been initialized before
	if(activeMetaStrategy == NULL){
		
		//retrieve what config says about strategy
		string metaStrategyName = Configuration::getInstance()->metaStrategyID;

		Logging::getInstance()->log("Meta strategy: %s", metaStrategyName.c_str());

		if (metaStrategyName == "epsilon-greedy") {
			activeMetaStrategy = new EpsilonGreedy();
		}
		else if (metaStrategyName == "probabilistic") {
			activeMetaStrategy = new Probabilistic();
		}
		else if (metaStrategyName == "random-switch") {
			activeMetaStrategy = new RandomSwitch();
		}
	
		else {	//otherwise, go to Single Choice using 'metaStrategyName' as the chosen behavior
			Logging::getInstance()->log("Using SingleChoice with strategy: %s", metaStrategyName.c_str());
			activeMetaStrategy = new SingleChoice(metaStrategyName);
		}
		//return new EpsilonGreedy();	//failsafe default...
	}

	return activeMetaStrategy;
}

