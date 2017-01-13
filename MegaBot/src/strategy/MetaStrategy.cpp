#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cfloat>
#include "BWAPI.h"
#include "MetaStrategy.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"
#include "../MegaBot.h"
#include "../data/Configuration.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "../data/MatchData.h"
#include "EpsilonGreedy.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>


//initializes consts
const string MetaStrategy::SKYNET = "Skynet";
const string MetaStrategy::XELNAGA = "Xelnaga";
const string MetaStrategy::NUSBot = "NUSBot";

MetaStrategy::MetaStrategy() : rng(std::time(0)) {
	name = "none";

	//initalizes behaviors
    portfolio.insert(make_pair(SKYNET, new Skynet()));
    portfolio.insert(make_pair(XELNAGA, new Xelnaga()));
    portfolio.insert(make_pair(NUSBot, new NUSBotModule()));

    //initializes reverse map
	map<string, BWAPI::AIModule*>::iterator behv;
	for(behv = portfolio.begin(); behv != portfolio.end(); behv++){
		strategyNames.insert(make_pair((*behv).second, (*behv).first));
		//Logging::getInstance()->log("Added %s to reverse map", (*behv).first.c_str() );
    }

	
}

MetaStrategy::~MetaStrategy() {
	
}

BWAPI::AIModule* MetaStrategy::getCurrentStrategy(){
	return currentStrategy;
}

std::string MetaStrategy::getCurrentStrategyName(){
	return strategyNames[currentStrategy];
}

void MetaStrategy::onStart() {
	map<string, BWAPI::AIModule*>::iterator behv;

	for(behv = portfolio.begin(); behv != portfolio.end(); behv++){
		Logging::getInstance()->log("%s: onStart()", (*behv).first.c_str());
		(*behv).second->onStart();
	}
}

string MetaStrategy::getName(){
	return name;
}

void MetaStrategy::forceStrategy(string strategyName){

	Logging::getInstance()->log("Forced strategy switch to '%s'", strategyName.c_str());
	string oldStrategyName = getCurrentStrategyName();

	if (strategyName == "random"){
		currentStrategy = randomUniform();
	}
	else if ( portfolio.find(strategyName) != portfolio.end() ){ //found strategyName in map
		currentStrategy = portfolio[strategyName];
	}
	else { //strategyName not found in map
		Logging::getInstance()->log(
			"Attempted to switch to invalid strategy '%s'. Will not switch.",
			strategyName.c_str()
		);
		return;
	}
	

	//undoes switches in case of some error:
	if (currentStrategy == NULL){
		Logging::getInstance()->log(
			"Attempted to switch to invalid strategy '%s'. Reverting switch...",
			strategyName.c_str()
		);
		currentStrategy = portfolio[oldStrategyName];
		return;
	}

	Logging::getInstance()->log(
		"Switching %s -> %s", oldStrategyName.c_str(), getCurrentStrategyName().c_str()
	);

}

AIModule* MetaStrategy::randomUniform() {
	Logging::getInstance()->log("Random uniform strategy selection...");
	boost::random::uniform_int_distribution<> unifInt(0, portfolio.size() - 1);
	int index = unifInt(rng);

	//code partly from: http://stackoverflow.com/a/158865
	map<string,AIModule*>::iterator iter = portfolio.begin();
	std::advance(iter, index);

	//just testing the randomness of the generator below :P
	//Logging::getInstance()->log("Random sequence: %d %d %d %d %d", dist(gen), dist(gen), dist(gen), dist(gen), dist(gen));
	Logging::getInstance()->log("Random index: %d", index);
	Logging::getInstance()->log("Selected: %s", (*iter).first.c_str());
	return (*iter).second;
}


/*
void MetaStrategy::printInfo() {
    Broodwar->drawTextScreen(180, 5, "\x0F%s", currentStrategyId.c_str());
}
*/

