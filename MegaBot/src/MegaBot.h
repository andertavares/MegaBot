#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <windows.h>
#include <map>
#include <string>

extern bool analyzed;
extern bool analysis_just_finished;
extern BWTA::Region* home;
extern BWTA::Region* enemy_base;
DWORD WINAPI AnalyzeThread();

using namespace std;

class MegaBot : public BWAPI::AIModule {

	string myBehaviorName, enemyBehaviorName;

	BWAPI::AIModule* currentBehavior;

	//maps the behaviors to their respective names
	std::map<BWAPI::AIModule*, string> behaviorNames;

	//maps behavior names to their AIModules
	std::map<string, BWAPI::AIModule*> behaviors;

	/*
	BWAPI::AIModule* behavior1;
	BWAPI::AIModule* behavior2;
	BWAPI::AIModule* behavior3;
	*/
public:

	static const string SKYNET; //= "Skynet";
	static const string XELNAGA; //= "Xelnaga";
	static const string NUSBot;// = "NUSBot";

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

	//returns the name of the behavior i'm using
	string myBehavior();

	//returns the name of the behavior the enemy is using
	string enemyBehavior();

	void drawStats(); 
	void drawBullets();
	void drawVisibilityData();
	void drawTerrainData();
	void showPlayers();
	void showForces();
	bool show_bullets;
	bool show_visibility_data;
};
