#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <windows.h>
#include <map>
#include <string>
#include "utils/Logging.h"
#include "strategy/MetaStrategy.h"

using namespace std;

class MegaBot : public BWAPI::AIModule {

	//string myBehaviorName, enemyBehaviorName;
	string enemyBehaviorName;

	//signals when enemy has recognized me
	bool acknowledged;

	BWAPI::AIModule* currentStrategy;

	MetaStrategy* metaStrategy;

	Logging* logger;

	//maps the behaviors to their respective names
	std::map<BWAPI::AIModule*, string> behaviorNames;

	//maps behavior names to their AIModules
	std::map<string, BWAPI::AIModule*> behaviors;

	//performs 'handshake': handles acknowledgement of enemy
	void handshake(string text);

public:

	MegaBot();

	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player* player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player* player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit* unit);
	virtual void onUnitEvade(BWAPI::Unit* unit);
	virtual void onUnitShow(BWAPI::Unit* unit);
	virtual void onUnitHide(BWAPI::Unit* unit);
	virtual void onUnitCreate(BWAPI::Unit* unit);
	virtual void onUnitDestroy(BWAPI::Unit* unit);
	virtual void onUnitMorph(BWAPI::Unit* unit);
	virtual void onUnitRenegade(BWAPI::Unit* unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit *unit);

	//below: not part of BWAPI::AIModule

	//returns the name of the behavior the enemy is using
	string enemyBehavior();

    //code for loss
    static const int LOSS = 0;

    // code for victory
    static const int WIN = 1;

    //code for draw
    static const int DRAW = 2;

    int gameResult;
	
};
