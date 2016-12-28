#include "GameState.h"
#include <BWAPI.h>
#include "../strategy/MetaStrategy.h"

using namespace BWAPI;
/*
BWAPI::Player* GameState::enemy = NULL;
string GameState::mapName = "unknown";
*/
GameState::GameState(void) {
	/*
	enemy = Broodwar->enemy();
	mapName = Broodwar->mapFileName();
	currentStrategyName = "none";	//TODO: MegaBot::strategyName()
	*/

	frame = Broodwar->getFrameCount();

	supplyUsed = Broodwar->self()->supplyUsed();
	supplyAvailable = Broodwar->self()->supplyTotal();

	currentMinerals = Broodwar->self()->minerals();
	currentGas = Broodwar->self()->gas();

	//adds all enemy units as spotted objects
	for (auto &unit : Broodwar->enemy()->getUnits()){
		//if (unit->isVisible()){
			enemyObjects.push_back(SpottedObject(unit));
			//addSpottedUnit(unit);
		//}
	}

	//adds all my units as spotted objects
	for (auto &unit : Broodwar->self()->getUnits()){
		myObjects.push_back(SpottedObject(unit));
	}
	
	//populates score data
	myUnitScore = Broodwar->self()->getUnitScore();
	myKillScore = Broodwar->self()->getKillScore();
	myBuildingScore = Broodwar->self()->getBuildingScore();
	myRazingScore = Broodwar->self()->getRazingScore();
	myGatheredMinerals = Broodwar->self()->gatheredMinerals();
	myGatheredGas = Broodwar->self()->gatheredGas();
	
}


GameState::~GameState(void) {
	
}
