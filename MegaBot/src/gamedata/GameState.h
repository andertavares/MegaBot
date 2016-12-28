#pragma once
#include <string>
#include <BWAPI.h>
#include <vector>
#include "SpottedObject.h"

using namespace std;
using namespace BWAPI;

/** Stores relevant information of a game state */
class GameState {
	
	//static BWAPI::Player* enemy;
	//static string mapName;

	/** name of current strategy */
	//string currentStrategyName;

	/** current frame */
	int frame;

	vector<SpottedObject> enemyObjects;
	vector<SpottedObject> myObjects;

	//resources
	int currentMinerals, currentGas;

	//supply
	int supplyUsed, supplyAvailable;

	//my score data
	int myUnitScore, myKillScore, 
		myBuildingScore, myRazingScore, 
		myGatheredMinerals, myGatheredGas, myTotalScore;

	// enemy score data
	/*int enemyUnitScore, enemyKillScore, 
		enemyBuildingScore, enemyRazingScore, 
		enemyGatheredMinerals, enemyGatheredGas, enemyTotalScore;*/


public:
	GameState(void);
	~GameState(void);
};

