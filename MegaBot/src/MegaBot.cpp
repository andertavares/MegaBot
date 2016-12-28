#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "MegaBot.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"
#include "strategy/MetaStrategy.h"
#include "strategy/MetaStrategyFactory.h"
#include "data/Configuration.h"
#include "data/MatchData.h"
#include "data/GameStateInfo.h"
#include "utils/Logging.h"
#include <cstdlib>
#include <ctime>

using namespace BWAPI;

MegaBot::MegaBot() : acknowledged(false) {
    
	logger = Logging::getInstance();

    //Configuration::getInstance()->parseConfig(); (moved to onStart)

    MatchData::getInstance()->registerEnemyBehaviorName("Unknown");
    enemyBehaviorName = "Unknown";

	//srand (static_cast <unsigned> (time(0)));
}

void MegaBot::onStart() {
    // Uncomment to enable complete map information
    //Broodwar->enableFlag(Flag::CompleteMapInformation);

    MatchData::getInstance()->registerMatchBegin();
    Configuration::getInstance()->parseConfig();

	//initializes meta strategy (strategy selector)
	metaStrategy = MetaStrategyFactory::getMetaStrategy();
	metaStrategy->onStart();
	
	currentStrategy = metaStrategy->getCurrentStrategy();

    //Broodwar->sendText("%s on!", myBehaviorName.c_str());		//sends behavior communication message
	logger->log("Game started with %s !", metaStrategy->getCurrentStrategyName().c_str());

    MatchData::getInstance()->registerMyBehaviorName(metaStrategy->getCurrentStrategyName());
	
    //currentBehavior->onStart();

    MatchData::getInstance()->writeToCrashFile();

    //sets user input, speed and GUI 
    Broodwar->enableFlag(Flag::UserInput);

    int speed = Configuration::getInstance()->speed;
	logger->log("Setting speed to %d.", speed);
    Broodwar->setLocalSpeed(0);

    bool gui = Configuration::getInstance()->enableGUI;
	logger->log("Setting GUI to %s.", gui ? "enabled" : "disabled");
    Broodwar->setGUI(gui);
}

void MegaBot::onEnd(bool isWinner) {
	string strategyName = metaStrategy->getCurrentStrategyName();

    logger->log("Game ended well with %s.", strategyName.c_str());
    
	int result;

    //if (Broodwar->elapsedTime() / 60 >= 80) result = MatchData::DRAW;
    //tournament rules for draw are 86400 frames, but we reduce 100 to ensure counting
    if (Broodwar->getFrameCount() >= 86300) {
		result = MatchData::DRAW;
		logger->log("Draw. Finishing behavior: %s.", strategyName.c_str());
	}
	else if (isWinner) {
        result = MatchData::WIN;
        logger->log("Victory! Winner behavior: %s.", strategyName.c_str());
    }
	else {
		result = MatchData::LOSS;
		logger->log("Defeat :( Finishing behavior: %s.", strategyName.c_str());
	}

    //MetaStrategy::getInstance()->addResult(win);
    //MetaStrategy::getInstance()->saveStats();
    //Statistics::getInstance()->saveResult(win);

    MatchData::getInstance()->registerMatchFinish(result);
    MatchData::getInstance()->writeSummary();
    MatchData::getInstance()->writeDetailedResult();
    MatchData::getInstance()->updateCrashFile();

	currentStrategy->onEnd(isWinner);
	logger->log("Finished.");
}

void MegaBot::onFrame() {
    /*string strategyId = Configuration::getInstance()->strategyID;
    if (strategyId == "epsilon-greedy") {
        MetaStrategy::getInstance()->OnFrame();
    }
    else if (strategyId == "fixed-intervals") {
        Method1::getInstance()->OnFrame();
        }*/
	if (Broodwar->elapsedTime() / 60 >= 81) {	//leave stalled game
        Broodwar->leaveGame();
        return;
    }

	metaStrategy->onFrame();	//might switch strategy so I update currentStrategy below

	currentStrategy = metaStrategy->getCurrentStrategy();
	currentStrategy->onFrame();

	 //draws some text
    Broodwar->drawTextScreen(240, 20, "\x0F MegaBot v1.0.2");
	Broodwar->drawTextScreen(240, 35, "\x0F Meta strategy: %s", metaStrategy->getName().c_str());
	//Broodwar->drawTextScreen(240, 35, "\x0F Meta-Strategy: %s", metaStrategy->getCurrentStrategyName().c_str());
	Broodwar->drawTextScreen(240, 50, "\x0F Strategy: %s", metaStrategy->getCurrentStrategyName().c_str());
    //Broodwar->drawTextScreen(5, 25, "\x0F Enemy behavior: %s", enemyBehaviorName.c_str());
    Broodwar->drawTextScreen(240, 65, "\x0F Enemy: %s", Broodwar->enemy()->getName().c_str());
	Broodwar->drawTextScreen(240, 80, "Frame count %d.", Broodwar->getFrameCount());
    Broodwar->drawTextScreen(240, 95, "Seconds: %d.", Broodwar->elapsedTime());
}

void MegaBot::onSendText(std::string text) {
	//tests for some commands:
	
	//logger->log("substr(0,6) of text is %s", text.substr(0, 6).c_str());
	if (text.substr(0, 6) == "switch"){
		logger->log("Will attempt a strategy switch to %s", text.substr(7, text.size()).c_str());
		metaStrategy->forceStrategy(text.substr(7, text.size()));
	}

    currentStrategy->onSendText(text);
}

void MegaBot::onReceiveText(BWAPI::Player* player, std::string text) {
    currentStrategy->onReceiveText(player, text);
	//handshake(text);
}

void MegaBot::onPlayerLeft(BWAPI::Player* player) {
    currentStrategy->onPlayerLeft(player);
}

void MegaBot::onNukeDetect(BWAPI::Position target) {
    currentStrategy->onNukeDetect(target);
}

void MegaBot::onUnitDiscover(BWAPI::Unit* unit) {
    currentStrategy->onUnitDiscover(unit);
}

void MegaBot::onUnitEvade(BWAPI::Unit* unit) {
    currentStrategy->onUnitEvade(unit);
}

void MegaBot::onUnitShow(BWAPI::Unit* unit) {
    currentStrategy->onUnitShow(unit);
}

void MegaBot::onUnitHide(BWAPI::Unit* unit) {
    currentStrategy->onUnitHide(unit);
}

void MegaBot::onUnitCreate(BWAPI::Unit* unit) {
    currentStrategy->onUnitCreate(unit);
}

void MegaBot::onUnitDestroy(BWAPI::Unit* unit) {
    currentStrategy->onUnitDestroy(unit);
}

void MegaBot::onUnitMorph(BWAPI::Unit* unit) {
    currentStrategy->onUnitMorph(unit);
}

void MegaBot::onUnitRenegade(BWAPI::Unit* unit) {
    currentStrategy->onUnitRenegade(unit);
}

void MegaBot::onSaveGame(std::string gameName) {
    currentStrategy->onSaveGame(gameName);
}

void MegaBot::onUnitComplete(BWAPI::Unit *unit) {
    currentStrategy->onUnitComplete(unit);
}

/*
string MegaBot::myBehavior() {
    return myBehaviorName;
}

string MegaBot::enemyBehavior() {
    return enemyBehaviorName;
}
*/
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
