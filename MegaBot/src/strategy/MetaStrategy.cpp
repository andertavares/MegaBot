#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cfloat>
#include "BWAPI.h"
#include "MetaStrategy.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"
#include "../MegaBot.h"
#include "../data/Configuration.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "../data/MatchData.h"
#include "EpsilonGreedy.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>


//initializes consts
const string MetaStrategy::SKYNET = "Skynet";
const string MetaStrategy::XELNAGA = "Xelnaga";
const string MetaStrategy::NUSBot = "NUSBot";

MetaStrategy::MetaStrategy() : rng(std::time(0)) {
	name = "none";

	//initalizes behaviors
    portfolio.insert(make_pair(SKYNET, new Skynet()));
    portfolio.insert(make_pair(XELNAGA, new Xelnaga()));
    portfolio.insert(make_pair(NUSBot, new NUSBotModule()));

    //initializes reverse map
	map<string, BWAPI::AIModule*>::iterator behv;
	for(behv = portfolio.begin(); behv != portfolio.end(); behv++){
		strategyNames.insert(make_pair((*behv).second, (*behv).first));
		//Logging::getInstance()->log("Added %s to reverse map", (*behv).first.c_str() );
    }

	
}

MetaStrategy::~MetaStrategy() {
	
}

BWAPI::AIModule* MetaStrategy::getCurrentStrategy(){
	return currentStrategy;
}

std::string MetaStrategy::getCurrentStrategyName(){
	return strategyNames[currentStrategy];
}

void MetaStrategy::onStart() {
	map<string, BWAPI::AIModule*>::iterator behv;

	for(behv = portfolio.begin(); behv != portfolio.end(); behv++){
		Logging::getInstance()->log("%s: onStart()", (*behv).first.c_str());
		(*behv).second->onStart();
	}
}

string MetaStrategy::getName(){
	return name;
}

void MetaStrategy::forceStrategy(string strategyName){

	Logging::getInstance()->log("Forced strategy switch to '%s'", strategyName.c_str());
	string oldStrategyName = getCurrentStrategyName();

	if (strategyName == "random"){
		currentStrategy = randomUniform();
	}
	else if ( portfolio.find(strategyName) != portfolio.end() ){ //found strategyName in map
		currentStrategy = portfolio[strategyName];
	}
	else { //strategyName not found in map
		Logging::getInstance()->log(
			"Attempted to switch to invalid strategy '%s'. Will not switch.",
			strategyName.c_str()
		);
		return;
	}
		
		
		/*{
		try {
			currentStrategy = portfolio.at(strategyName);
			//exclusions.at("the");
		}
		catch(std::out_of_range e) {
			Logging::getInstance()->log(
				"Attempted to switch to invalid strategy '%s'. Will not switch.",
				strategyName.c_str()
			);
			return;
		}
	}*/

	//undoes switches in case of some error:
	if (currentStrategy == NULL){
		Logging::getInstance()->log(
			"Attempted to switch to invalid strategy '%s'. Reverting switch...",
			strategyName.c_str()
		);
		currentStrategy = portfolio[oldStrategyName];
		return;
	}

	Logging::getInstance()->log(
		"Switching %s -> %s", oldStrategyName.c_str(), getCurrentStrategyName().c_str()
	);

}

AIModule* MetaStrategy::randomUniform() {
	Logging::getInstance()->log("Random uniform strategy selection...");
	boost::random::uniform_int_distribution<> unifInt(0, portfolio.size() - 1);
	int index = unifInt(rng);

	//code partly from: http://stackoverflow.com/a/158865
	map<string,AIModule*>::iterator iter = portfolio.begin();
	std::advance(iter, index);

	//just testing the randomness of the generator below :P
	//Logging::getInstance()->log("Random sequence: %d %d %d %d %d", dist(gen), dist(gen), dist(gen), dist(gen), dist(gen));
	Logging::getInstance()->log("Random index: %d", index);
	Logging::getInstance()->log("Selected: %s", (*iter).first.c_str());
	return (*iter).second;
}

/*
void Method1::OnFrame() {
    int thisFrame = Broodwar->getFrameCount();
    //Logging::getInstance()->log("Frame count %d.", thisFrame);

    if (thisFrame % 100 == 0 && thisFrame > 0) {
		Logging::getInstance()->log("Frame count %d.", thisFrame);
    }

    if (thisFrame % 5000 == 0 && thisFrame > 0) {  //behavior switch
        /*int playerBases = GameStateInfo::getInstance()->numBases(myBehaviorName.c_str(), BWAPI::Races::Protoss);
        Logging::getInstance()->log("Number of player's bases %d.", playerBases);

        BWAPI::Race enemyRace = BWAPI::Broodwar->enemy()->getRace();
        string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
        int enemyBases = GameStateInfo::getInstance()->numBases(enemyName, enemyRace);
        Logging::getInstance()->log("Number of enemy's bases %d.", enemyBases);

        int playerSmallUnits = GameStateInfo::getInstance()->terrestrialSmallUnits(myBehaviorName.c_str());
        Logging::getInstance()->log("Number of player's small units %d.", playerSmallUnits);

        string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
        int enemySmallUnits = GameStateInfo::getInstance()->terrestrialSmallUnits(enemyName);
        Logging::getInstance()->log("Number of enemy's small units %d.", enemySmallUnits);
		* <- REMEMBER TO CLOSE COMMENT HERE IF METHOD GETS UNCOMMENTED

        string oldBehaviorName = myBehaviorName;
        Logging::getInstance()->log("Frame count %d.", thisFrame);
        double lucky = (rand() / (double)(RAND_MAX + 1));

        if (lucky < 0.33) {
			myBehaviorName = MetaStrategy::NUSBot;
        }
        else if (lucky < 0.66) {
            myBehaviorName = MetaStrategy::SKYNET;
        }
        else {
            myBehaviorName = MetaStrategy::XELNAGA;
        }
        Logging::getInstance()->log("Switching: %s -> %s", oldBehaviorName.c_str(), myBehaviorName.c_str());
        MatchData::getInstance()->registerMyBehaviorName(myBehaviorName);
        currentBehavior = behaviors[myBehaviorName];
        currentBehavior->onFrame();
        Logging::getInstance()->log("%s on!", myBehaviorName.c_str());
    }

    

    //sends behavior communication message every 200 frames
    /*if (!acknowledged && (BWAPI::Broodwar->getFrameCount() % 200) == 0) {
    Broodwar->sendText("%s on!", myBehaviorName.c_str());
    }
	* <- REMEMBER TO CLOSE COMMENT HERE IF METHOD GETS UNCOMMENTED

   
}
*/

/*
void MetaStrategy::printInfo() {
    Broodwar->drawTextScreen(180, 5, "\x0F%s", currentStrategyId.c_str());
}
*/

/*
void MetaStrategy::loadStats() {
    string filename = getFilename();

    ifstream inFile;
    inFile.open(filename.c_str());
    if (!inFile) {
        //No file found.
        return;
    }
    else {
        string line;
        char buffer[256];
        while (!inFile.eof()) {
            inFile.getline(buffer, 256);
            if (buffer[0] != ';') {
                stringstream ss;
                ss << buffer;
                line = ss.str();
                addEntry(line);
            }
        }
        inFile.close();
    }
}
*/

/*
void MetaStrategy::addEntry(string line) {
    if (line == "") return;

    StrategyStats s = StrategyStats();
    int i;
    string t;

    i = line.find(";");
    t = line.substr(0, i);
    s.strategyId = t;
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.ownRace = t;
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.opponentRace = t;
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.won = toInt(t);
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.lost = toInt(t);
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.draw = toInt(t);
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.total = toInt(t);
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.mapName = t;
    line = line.substr(i + 1, line.length());

    i = line.find(";");
    t = line.substr(0, i);
    s.mapHash = t;
    line = line.substr(i + 1, line.length());

    stats.push_back(s);
}
*/

/*
int MetaStrategy::toInt(string &str) {
    stringstream ss(str);
    int n;
    ss >> n;
    return n;
}
*/

/*
string MetaStrategy::getFilename() {
    stringstream ss;
    ss << Configuration::INPUT_DIR; // "bwapi-data\\AI\\";
    //ss << "bwapi-data\\read\\"; //Tournament persistent storage version
    ss << "Strategies_MegaBot.csv";

    return ss.str();
}
*/


/*
string MetaStrategy::getWriteFilename() {
    stringstream ss;
    ss << Configuration::OUTPUT_DIR;	// "bwapi-data\\AI\\";
    //ss << "bwapi-data\\write\\"; //Tournament persistent storage version
    ss << "Strategies_MegaBot.csv";

    return ss.str();
}
*/

/*
void MetaStrategy::addResult(int win) {
    if (!active) return;

    string opponentRace = Broodwar->enemy()->getRace().getName();
    string mapHash = Broodwar->mapHash();

    //Check if we have the entry already
    for (int i = 0; i < (int)stats.size(); i++) {
        if (mapHash == stats.at(i).mapHash && opponentRace == stats.at(i).opponentRace && currentStrategyId == stats.at(i).strategyId) {
            stats.at(i).total++;
            if (win == 0) stats.at(i).lost++;
            if (win == 1) stats.at(i).won++;
            if (win == 2) stats.at(i).draw++;
            return;
        }
    }

    StrategyStats s = StrategyStats();
    s.total++;
    if (win == 0) s.lost++;
    if (win == 1) s.won++;
    if (win == 2) s.draw++;
    s.strategyId = currentStrategyId;
    s.mapHash = mapHash;
    s.mapName = Broodwar->mapFileName();
    s.ownRace = Broodwar->self()->getRace().getName();
    s.opponentRace = opponentRace;
    stats.push_back(s);
}
*/

/*
void MetaStrategy::saveStats() {
    if (!active) return;

    //Fill entries in stats file for combinations that have
    //not yet been played.
    string mapHash = Broodwar->mapHash();
    string opponentRace = Broodwar->enemy()->getRace().getName();
    string ownRace = Broodwar->self()->getRace().getName();

    for (int i = 0; i < (int)strategies.size(); i++) {
        bool found = false;
        for (int s = 0; s < (int)stats.size(); s++) {
            if (strategies.at(i).strategyId == stats.at(s).strategyId && mapHash == stats.at(s).mapHash && opponentRace == stats.at(s).opponentRace) {
                //Matches
                found = true;
                break;
            }
        }

        if (!found) {
            //Only fill in the strategies for
            //the same race
            if (ownRace == strategies.at(i).race.getName()) {
                //Add entry
                StrategyStats s = StrategyStats();
                s.mapHash = mapHash;
                s.mapName = Broodwar->mapFileName();
                s.opponentRace = opponentRace;
                s.ownRace = strategies.at(i).race.getName();
                s.strategyId = strategies.at(i).strategyId;

                stats.push_back(s);
            }
        }
    }

    //Save the file
    string filename = getWriteFilename();

    ofstream outFile;
    outFile.open(filename.c_str());
    if (!outFile) {
        Logging::getInstance()->log("Error writing to stats file!\n");
    }
    else {
        for (int i = 0; i < (int)stats.size(); i++) {
            stringstream s2;
            s2 << stats.at(i).strategyId;
            s2 << ";";
            s2 << stats.at(i).ownRace;
            s2 << ";";
            s2 << stats.at(i).opponentRace;
            s2 << ";";
            s2 << stats.at(i).won;
            s2 << ";";
            s2 << stats.at(i).lost;
            s2 << ";";
            s2 << stats.at(i).draw;
            s2 << ";";
            s2 << stats.at(i).total;
            s2 << ";";
            s2 << stats.at(i).mapName;
            s2 << ";";
            s2 << stats.at(i).mapHash;
            s2 << ";\n";

            outFile << s2.str();
        }
        outFile.close();
    }
}
*/
