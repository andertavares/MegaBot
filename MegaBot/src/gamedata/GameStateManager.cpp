#include "GameStateManager.h"
#include <BWAPI.h>
#include "GameState.h"
#include "../data/Configuration.h"
#include "../utils/Logging.h"

GameStateManager* GameStateManager::instance = NULL;

GameStateManager::GameStateManager(void) : frequency(500) {
	//TODO write files with increasing number, just like logging
	stateDumpFile = Configuration::OUTPUT_DIR + 
		"states_MegaBot-vs-" + 
		BWAPI::Broodwar->enemy()->getName();

}


GameStateManager::~GameStateManager(void) {

}

GameStateManager* GameStateManager::getInstance(){
	if (instance == NULL) {
		instance = new GameStateManager();
	}
	return instance;
}

void GameStateManager::onFrame(){
	
	//draws information of observed units
	map<int, SpottedObject>& spottedEnemies = GameState::getSpottedEnemyUnits();
	map<int, SpottedObject>::iterator it; 
	
	BWAPI::Broodwar->printf("I spotted %d enemies", spottedEnemies.size());

	//for (auto enemyUnit : spottedEnemies) {
	for(it = spottedEnemies.begin(); it != spottedEnemies.end(); it++){
		//if (enemyBldg-> isActive()){
		SpottedObject& enemyUnit = (*it).second;

		int x1 = enemyUnit.getTilePosition().x() * 32;
		int y1 = enemyUnit.getTilePosition().y() * 32;
		int x2 = x1 + enemyUnit.getType().dimensionLeft() + enemyUnit.getType().dimensionRight();
		int y2 = y1 + enemyUnit.getType().dimensionUp() + enemyUnit.getType().dimensionDown();
		
		
		BWAPI::Broodwar->drawBoxMap(x1, y1, x2, y2, BWAPI::Colors::Red, false);
		BWAPI::Broodwar->drawTextMap(
			x1 + enemyUnit.getType().dimensionRight(), 
			y1 + enemyUnit.getType().dimensionDown(), 
			enemyUnit.getType().c_str()
		);
		//}
	}

	/*
	for (auto enemyUnit : enemyUnits) {
		//if (enemyBldg-> isActive()){
		int x1 = enemyUnit->getTilePosition().x * 32;
		int y1 = enemyUnit->getTilePosition().y * 32;
		int x2 = x1 + enemyUnit->getType().dimensionRight() * 2;
		int y2 = y1 + enemyUnit->getType().dimensionDown() * 2;

		Broodwar->drawBoxMap(x1, y1, x2, y2, Colors::Orange, false);
		Broodwar->drawTextMap(x1 + enemyUnit->getType().dimensionRight(), y1 + enemyUnit->getType().dimensionDown(), UnitString::shortName(enemyUnit->getType()).c_str());
		//}
	}
	*/
	
	//skips first frame as well as the interval configured in frequency
	int thisFrame = BWAPI::Broodwar->getFrameCount();
	
	if(thisFrame % frequency != 0 || thisFrame == 0){
		return;
	}

	recordState();
}

void GameStateManager::recordState(){
	int thisFrame = BWAPI::Broodwar->getFrameCount();

	if ( gameStates.find(thisFrame) != gameStates.end() ){
		//already stored this game state, will skip
		Logging::getInstance()->log(
			"Already recorded state at frame %d. Skipping...", thisFrame
		);
		return;
	}
	
	Logging::getInstance()->log("Recording game state.");
	gameStates.insert(make_pair(thisFrame, GameState()));
	
}
