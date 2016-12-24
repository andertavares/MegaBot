#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "MegaBot.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"
#include "strategy/StrategySelector.h"
#include "data/Configuration.h"
#include "data/MatchData.h"
#include "data/GameStateInfo.h"
#include "utils/Logging.h"

using namespace BWAPI;

//initializes consts
const string MegaBot::SKYNET = "Skynet";
const string MegaBot::XELNAGA = "Xelnaga";
const string MegaBot::NUSBot = "NUSBot";

MegaBot::MegaBot()
    : acknowledged(false) {

    //initalizes behaviors
    behaviors.insert(make_pair(SKYNET, new Skynet()));
    behaviors.insert(make_pair(XELNAGA, new Xelnaga()));
    behaviors.insert(make_pair(NUSBot, new NUSBotModule()));

    //initializes reverse map
	 map<string, BWAPI::AIModule*>::iterator behv;
	for(behv = behaviors.begin(); behv != behaviors.end(); behv++){
    	behaviorNames.insert(make_pair((*behv).second, (*behv).first));
    }

	logger = Logging::getInstance();

    //Configuration::getInstance()->parseConfig(); (moved to onStart)

    MatchData::getInstance()->registerEnemyBehaviorName("Unknown");
   
    enemyBehaviorName = "Unknown";
}

void MegaBot::onStart() {
    // Uncomment to enable complete map information
    //Broodwar->enableFlag(Flag::CompleteMapInformation);

    MatchData::getInstance()->registerMatchBegin();
    Configuration::getInstance()->parseConfig();

    //myBehaviorName = MegaBot::NUSBot;

	map<string, BWAPI::AIModule*>::iterator behv;
	for(behv = behaviors.begin(); behv != behaviors.end(); behv++){
		logger->log("Initializing %s !", behaviorNames[(*behv).second].c_str());
		(*behv).second->onStart();
	}
	
    myBehaviorName = StrategySelector::getInstance()->getStrategy();

    //Broodwar->sendText("%s on!", myBehaviorName.c_str());		//sends behavior communication message
    logger->log("Game started with %s !", myBehaviorName.c_str());

    MatchData::getInstance()->registerMyBehaviorName(myBehaviorName);
    currentBehavior = behaviors[myBehaviorName];
    currentBehavior->onStart();

    MatchData::getInstance()->writeToCrashFile();

    //overrides user input, speed and GUI set by currentBehavior
    Broodwar->enableFlag(Flag::UserInput);

    int speed = Configuration::getInstance()->speed;
	logger->log("Setting speed to %d.", speed);
    Broodwar->setLocalSpeed(0);

    bool gui = Configuration::getInstance()->enableGUI;
	logger->log("Setting GUI to %s.", gui ? "enabled" : "disabled");
    Broodwar->setGUI(gui);

	logger->log(
        "Parameters: alpha=%.2f; epsilon=%.2f.",
        Configuration::getInstance()->alpha,
        Configuration::getInstance()->epsilon
    );
}

void MegaBot::onEnd(bool isWinner) {
    logger->log("Game ended well with %s !", myBehaviorName.c_str());
    int result = MatchData::LOSS;

    //if (Broodwar->elapsedTime() / 60 >= 80) result = MatchData::DRAW;
    //tournament rules for draw are 86400 frames, but we reduce 100 to ensure counting
    if (Broodwar->getFrameCount() >= 86300) {
		result = MatchData::DRAW;
		logger->log("Draw. Finishing behavior: %s.", myBehaviorName.c_str());
	}
	else if (isWinner) {
        result = MatchData::WIN;
        logger->log("Victory! Winner behavior: %s.", myBehaviorName.c_str());
    }
	else {
		logger->log("Defeat :( Finishing behavior: %s.", myBehaviorName.c_str());
	}

    //StrategySelector::getInstance()->addResult(win);
    //StrategySelector::getInstance()->saveStats();
    //Statistics::getInstance()->saveResult(win);

    MatchData::getInstance()->registerMatchFinish(result);
    MatchData::getInstance()->writeSummary();
    MatchData::getInstance()->writeDetailedResult();
    MatchData::getInstance()->updateCrashFile();

    currentBehavior->onEnd(isWinner);
	logger->log("Finished.");
}

void MegaBot::onFrame() {
    string strategyId = Configuration::getInstance()->strategyID;
    if (strategyId == "epsilon-greedy") {
        StrategySelector::getInstance()->OnFrame();
    }
    else if (strategyId == "fixed-intervals") {
        Method1::getInstance()->OnFrame();
        }
        }

void MegaBot::onSendText(std::string text) {
    currentBehavior->onSendText(text);
}

void MegaBot::onReceiveText(BWAPI::Player* player, std::string text) {
    currentBehavior->onReceiveText(player, text);
	//handshake(text);
}

void MegaBot::onPlayerLeft(BWAPI::Player* player) {
    currentBehavior->onPlayerLeft(player);
}

void MegaBot::onNukeDetect(BWAPI::Position target) {
    currentBehavior->onNukeDetect(target);
}

void MegaBot::onUnitDiscover(BWAPI::Unit* unit) {
    currentBehavior->onUnitDiscover(unit);
}

void MegaBot::onUnitEvade(BWAPI::Unit* unit) {
    currentBehavior->onUnitEvade(unit);
}

void MegaBot::onUnitShow(BWAPI::Unit* unit) {
    currentBehavior->onUnitShow(unit);
}

void MegaBot::onUnitHide(BWAPI::Unit* unit) {
    currentBehavior->onUnitHide(unit);
}

void MegaBot::onUnitCreate(BWAPI::Unit* unit) {
    currentBehavior->onUnitCreate(unit);
}

void MegaBot::onUnitDestroy(BWAPI::Unit* unit) {
    currentBehavior->onUnitDestroy(unit);
}

void MegaBot::onUnitMorph(BWAPI::Unit* unit) {
    currentBehavior->onUnitMorph(unit);
}

void MegaBot::onUnitRenegade(BWAPI::Unit* unit) {
    currentBehavior->onUnitRenegade(unit);
}

void MegaBot::onSaveGame(std::string gameName) {
    currentBehavior->onSaveGame(gameName);
}

void MegaBot::onUnitComplete(BWAPI::Unit *unit) {
    currentBehavior->onUnitComplete(unit);
}

string MegaBot::myBehavior() {
    return myBehaviorName;
}

string MegaBot::enemyBehavior() {
    return enemyBehaviorName;
}

void MegaBot::handshake(string text){
	//behavior message recognition: checks whether text ends with 'on!'
    if (enemyBehaviorName == "Unknown" && text.substr(max(3, int(text.size())) - 3) == string("on!")) {
        //splits text in spaces and gets 1st part: this is enemy's name
        istringstream iss(text);
        vector<string> tokens;
        copy(
            istream_iterator<string>(iss),
            istream_iterator<string>(),
            back_inserter(tokens)
        );
        //the 'magic' above is from: http://stackoverflow.com/a/237280/1251716

        enemyBehaviorName = tokens[0];

        Broodwar->printf(">>>>> Enemy recognized: %s <<<<<", enemyBehaviorName.c_str());
        MatchData::getInstance()->registerEnemyBehaviorName(enemyBehaviorName);

        //sends text back to enemy to acknowledge recognition
        Broodwar->sendText("ACKNOWLEDGED!");
    }
    //recognizes that enemy has acknowledged my strategy
    else if (text == "ACKNOWLEDGED!") {
        acknowledged = true;
    }
}
