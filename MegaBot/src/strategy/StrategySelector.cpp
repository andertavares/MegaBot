#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <cfloat>
#include "StrategySelector.h"
#include "../MegaBot.h"
#include "../data/Configuration.h"
#include "../utils/tinyxml2.h"



StrategySelector* StrategySelector::instance = NULL;

StrategySelector::StrategySelector() {
    active = true;
    /*
    loadStats();
    */
}

StrategySelector* StrategySelector::getInstance() {
    if (instance == NULL) {
        instance = new StrategySelector();
    }
    return instance;
}

StrategySelector::~StrategySelector() {
    instance = NULL;
}

void StrategySelector::enable() {
    active = true;
}

void StrategySelector::disable() {
    active = false;
}

void StrategySelector::selectStrategy() {
    using namespace tinyxml2;
    //retrieve what config says about strategy
    string strategyId = Configuration::getInstance()->strategyID;

    //chooses strategy probabilistically
    if (strategyId == "probabilistic") {
        srand(time(NULL));
        float lucky = (rand() % 1000) / 1000.f;
        float epsilon = 0.1f;
        if (lucky < epsilon) {
            Broodwar->printf(
                "Choosing randomly: (%.3f < %.3f)", lucky, epsilon
                );
            currentStrategyId = probabilistic();
        }
        else {
            //file to read is MegaBot-vs-enemy.xml
            string inputFile = Configuration::getInstance()->enemyInformationInputFile();

            tinyxml2::XMLDocument doc;
            XMLError errorInput = doc.LoadFile(inputFile.c_str());

            if (errorInput == XMLError::XML_NO_ERROR) {
                Player* enemy = Broodwar->enemy();
                string enemy_name = enemy->getName();
                XMLElement* rootNode = doc.FirstChildElement("scores");

                if (rootNode != NULL) {
                    XMLElement* candidate = rootNode->FirstChildElement();
                    string best_name;
                    float best_score = -1.0f;

                    map<string, float> scoresMap;
                    scoresMap[MegaBot::NUSBot] = 0;
                    scoresMap[MegaBot::SKYNET] = 0;
                    scoresMap[MegaBot::XELNAGA] = 0;

                    while (candidate != NULL) {
                        float score = -FLT_MAX;
                        candidate->QueryFloatText(&score);

                        scoresMap[candidate->Name()] = score;
                        candidate = candidate->NextSiblingElement();
                    }

                    for (std::map<string, float>::iterator it = scoresMap.begin(); it != scoresMap.end(); ++it) {
                        if (it->second > best_score) {
                            best_name = it->first;
                            best_score = it->second;
                        }
                    }

                    if (best_name.empty()) {
                        Broodwar->printf("Best strategy could not be determined. Choosing prob'ly");
                        currentStrategyId = probabilistic();
                    }
                    else {
                        currentStrategyId = best_name;
                    }
                }
                else {
                    Broodwar->printf("Enemy information not found, choosing strategy randomly");
                    currentStrategyId = probabilistic();
                }
            }
            else { //prints error
                Broodwar->printf(
                    "Error while parsing strategy file '%s'. Error: '%s'",
                    Configuration::getInstance()->strategyFile.c_str(),
                    doc.ErrorName()
                    );
                currentStrategyId = probabilistic();
            }
        }
    }
    else {	//otherwise, use strategy explicitly described in config. 
        currentStrategyId = strategyId;
    }
    return;
}

string StrategySelector::probabilistic() {
    using namespace tinyxml2;

    string defaultBehavior = MegaBot::SKYNET;	//in case something go wrong

    //parses strategy file
    map<string, float> behaviors;

    tinyxml2::XMLDocument doc;
    int result = doc.LoadFile(Configuration::getInstance()->strategyFile.c_str());

    if (result != XML_NO_ERROR) {
        Broodwar->printf(
            "An error has occurred while parsing strategy file '%s'. Error: '%s'",
            Configuration::getInstance()->strategyFile.c_str(),
            doc.ErrorName()
            );
        return defaultBehavior;	//returns a default strategy
    }

    XMLElement* behaviorEntry = doc.FirstChildElement("strategy")->FirstChildElement("behavior");
    for (; behaviorEntry; behaviorEntry = behaviorEntry->NextSiblingElement()) {
        string name = string(behaviorEntry->Attribute("name"));
        float probability = 0;
        behaviorEntry->QueryFloatAttribute("probability", &probability);

        behaviors.insert(make_pair(name, probability));
    }


    //openings loaded, now will select one
    float sum = 0.f; //probabilities should add to 1.0, but this is to guard against abnormal cases
    map<string, float>::iterator behv;
    for (behv = behaviors.begin(); behv != behaviors.end(); ++behv) {
        sum += behv->second;
    }

    /*
    uncomment when c++11 is available
    for (auto behv : behaviors) {
    sum += behv.second;
    }*/

    //generates a pseudo-random number between 0 and sum
    float random = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / sum));

    //traverses the list until we find an opening that matches the random number
    float acc = 0;
    for (behv = behaviors.begin(); behv != behaviors.end(); ++behv) {//for (auto opening : behaviors) {
        if (random < acc + behv->second) {	//found!
            Broodwar->printf(
                "MegaBot chose: %s (random: %.3f, target: %.3f, acc: %.3f, sum: %.3f)",
                behv->first.c_str(), random, (acc + behv->second), acc, sum
                );
            return behv->first;
        }
        acc += behv->second;
    }
    Broodwar->printf(
        "ERROR: behavior was not randomly selected (random: %.3f, acc: %.3f, sum: %.3f). Defaulting to: %s.",
        random, acc, sum, defaultBehavior
        );
    return defaultBehavior;	//something went wrong, opening was not randomly selected =/
}

string StrategySelector::getStrategy() {
    selectStrategy();
    return currentStrategyId;
}

void StrategySelector::printInfo() {
    Broodwar->drawTextScreen(180, 5, "\x0F%s", currentStrategyId.c_str());
}

void StrategySelector::loadStats() {
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

void StrategySelector::addEntry(string line) {
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

int StrategySelector::toInt(string &str) {
    stringstream ss(str);
    int n;
    ss >> n;
    return n;
}

string StrategySelector::getFilename() {
    stringstream ss;
    ss << Configuration::INPUT_DIR; // "bwapi-data\\AI\\";
    //ss << "bwapi-data\\read\\"; //Tournament persistent storage version
    ss << "Strategies_MegaBot.csv";

    return ss.str();
}

string StrategySelector::getWriteFilename() {
    stringstream ss;
    ss << Configuration::OUTPUT_DIR;	// "bwapi-data\\AI\\";
    //ss << "bwapi-data\\write\\"; //Tournament persistent storage version
    ss << "Strategies_MegaBot.csv";

    return ss.str();
}

void StrategySelector::addResult(int win) {
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

void StrategySelector::saveStats() {
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
        Broodwar->printf("Error writing to stats file!\n");
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
