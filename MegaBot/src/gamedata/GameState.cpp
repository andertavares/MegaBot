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
		//updates only if I can see unit or CompleteMapInformation is enabled
		if((*it)->isVisible() || BWAPI::Broodwar->isFlagEnabled(BWAPI::Flag::CompleteMapInformation)){
			//code below works even if SpottedObject is not present at the map (a default one is created)
			currentEnemyObjects[(*it)->getID()].update(*it); 
			//persistentEnemyObjects[(*it)->getID()].update(*it); //this one is updated in GameStateManager::onFrame
		}
	
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

void GameState::onUnitDiscover(BWAPI::Unit* unit) {
	//creates or updates information about enemy unit
	if(unit->getPlayer()->getID() == BWAPI::Broodwar->enemy()->getID()){
		Logging::getInstance()->log(
			"Discovered %s at %d, %d", unit->getType().c_str(), unit->getPosition().x(), unit->getPosition().y()
		);
		persistentEnemyObjects[unit->getID()].update(unit);
	}
}

void GameState::onUnitEvade(BWAPI::Unit* unit) {
    //currentStrategy->onUnitEvade(unit);
}

void GameState::onUnitShow(BWAPI::Unit* unit) {
	//creates or updates information about enemy unit
	if(unit->getPlayer()->getID() == BWAPI::Broodwar->enemy()->getID()){
		Logging::getInstance()->log(
			"Shown %s at %d, %d", unit->getType().c_str(), unit->getPosition().x(), unit->getPosition().y()
		);
		persistentEnemyObjects[unit->getID()].update(unit);
	}
}

void GameState::onUnitHide(BWAPI::Unit* unit) {
    //currentStrategy->onUnitHide(unit);
}

void GameState::onUnitCreate(BWAPI::Unit* unit){
	//creates or updates information about enemy unit
	if(unit->getPlayer()->getID() == BWAPI::Broodwar->enemy()->getID()){
		Logging::getInstance()->log(
			"Created %s at %d, %d", unit->getType().c_str(), unit->getPosition().x(), unit->getPosition().y()
		);

		persistentEnemyObjects[unit->getID()].update(unit);
	}
	
}

void GameState::onUnitDestroy(BWAPI::Unit* unit){
	Logging::getInstance()->log(
		"Removing unit %s w/ ID=%d from persistentEnemyObjects",
		unit->getType().getName().c_str(), unit->getID()
	);

	persistentEnemyObjects.erase(unit->getID());
}

void GameState::onUnitMorph(BWAPI::Unit* unit) {
	//updates information of enemy unit that morphed
    if(unit->getPlayer()->getID() == BWAPI::Broodwar->enemy()->getID()){
		Logging::getInstance()->log(
			"Morphed %s at %d, %d", unit->getType().c_str(), unit->getPosition().x(), unit->getPosition().y()
		);

		persistentEnemyObjects[unit->getID()].update(unit);
	}
}

void GameState::onUnitRenegade(BWAPI::Unit* unit) {
	Logging::getInstance()->log(
		"Renegaded %s at %d, %d", unit->getType().c_str(), unit->getPosition().x(), unit->getPosition().y()
	);

	if(unit->getPlayer()->getID() == BWAPI::Broodwar->enemy()->getID()){
		//enemy has got control of an unit, update it
		persistentEnemyObjects[unit->getID()].update(unit);
	}
	else if(unit->getPlayer()->getID() == BWAPI::Broodwar->self()->getID()) {
		//I took control of an unit, erase it from enemy units
		persistentEnemyObjects.erase(unit->getID());
	}
}

map<int, SpottedObject>& GameState::getSpottedEnemyUnits(){
	return persistentEnemyObjects;
}
