#include "GameState.h"
#include <BWAPI.h>
#include "../strategy/MetaStrategy.h"

using namespace BWAPI;
/*
BWAPI::Player* GameState::enemy = NULL;
string GameState::mapName = "unknown";
*/

map<int, SpottedObject> GameState::persistentEnemyObjects;

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
	
	//for (BWAPI::Unit* unit : Broodwar->enemy()->getUnits()){
	for(it = enemyUnits.begin(); it != enemyUnits.end(); it++){
		//if (unit->isVisible()){
		currentEnemyObjects[(*it)->getID()].update(*it); //hope default constructor and init goes ok SpottedObject(*it);
		persistentEnemyObjects[(*it)->getID()].update(*it);
		
		//checks if object already exists in persistent map, updates it if needed
		/*if(persistentEnemyObjects.find((*it)->getID()) != persistentEnemyObjects.end()){
			//found, update
			persistentEnemyObjects[(*it)->getID()].update(*it);
		}*/


			//addSpottedUnit(unit);
		//}
	}

	//adds all my units as spotted objects
	set<BWAPI::Unit*> myUnits = Broodwar->self()->getUnits();
	//set<BWAPI::Unit*>::iterator it;
	//for (BWAPI::Unit* unit : Broodwar->self()->getUnits()){
	for(it = myUnits.begin(); it != myUnits.end(); it++){
		currentMyObjects[(*it)->getID()].update(*it); // SpottedObject(*it);
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

void GameState::unitDestroyed(BWAPI::Unit* unit){
	Logging::getInstance()->log(
		"Removing unit %s w/ ID=%d from persistentEnemyObjects",
		unit->getType().getName().c_str(), unit->getID()
	);
	persistentEnemyObjects.erase(unit->getID());
}

map<int, SpottedObject>& GameState::getSpottedEnemyUnits(){
	return persistentEnemyObjects;
}
