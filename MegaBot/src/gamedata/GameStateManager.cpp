#include "GameStateManager.h"
#include <BWAPI.h>
#include "GameState.h"
#include "../utils/Logging.h"

GameStateManager* GameStateManager::instance = NULL;

GameStateManager::GameStateManager(void) {

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
	int thisFrame = BWAPI::Broodwar->getFrameCount();

	//skips first frame as well as the interval configured in frequency
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
