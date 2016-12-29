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
	set<BWAPI::Unit*> enemyUnits = Broodwar->enemy()->getUnits();
	set<BWAPI::Unit*>::iterator it;
	
	//for (auto &unit : Broodwar->enemy()->getUnits()){
	for(it = enemyUnits.begin(); it != enemyUnits.end(); it++){
		//if (unit->isVisible()){
			enemyObjects.push_back(SpottedObject(*it));
			//addSpottedUnit(unit);
		//}
	}

	//adds all my units as spotted objects
	set<BWAPI::Unit*> myUnits = Broodwar->self()->getUnits();
	//set<BWAPI::Unit*>::iterator it;
	//for (auto &unit : Broodwar->self()->getUnits()){
	for(it = myUnits.begin(); it != myUnits.end(); it++){
		myObjects.push_back(SpottedObject(*it));
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
