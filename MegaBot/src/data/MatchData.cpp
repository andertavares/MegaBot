#include "MatchData.h"
#include <ctime>
#include <BWAPI.h>
#include <fstream>
#include <sstream>
#include <algorithm> 

#include "../utils/tinyxml2.h"

#include "Configuration.h"

using namespace BWAPI;
using namespace std;

MatchData* MatchData::instance = NULL;

MatchData::MatchData() {}


MatchData::~MatchData() {}

void MatchData::registerMatchBegin() {
    startTime = currentDateTime();
}

void MatchData::registerMatchFinish(int result) {
    gameResult = result;

    //attempts to retrieve enemy score information (not working, currently BWAPI doesn't allow retrieval of enemy info even at match end)
    Broodwar->enableFlag(Flag::CompleteMapInformation);

    //registers scores of both players
    Player* me = Broodwar->self();
    Player* enemy = Broodwar->enemy();
}

void MatchData::registerMyBehaviorName(string name) {
    myBehaviorName = name;
}

void MatchData::registerEnemyBehaviorName(string name) {
    enemyBehaviorName = name;
}

bool isSpace(char caracter) {
    if (caracter == ' ')
        return true;
    else
        return false;
}

void MatchData::writeDetailedResult() {
    Player* enemy = Broodwar->enemy();
    string bot_name = myBehaviorName;
    string enemy_name = enemy->getName();

    int value;

    tinyxml2::XMLElement* botNode;
    tinyxml2::XMLElement* myBotNode;
    tinyxml2::XMLElement* queryNode;

    const char* filename = Configuration::getInstance()->readDataFile.c_str();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename);

    std::replace_if(enemy_name.begin(), enemy_name.end(), isSpace, '_');

    botNode = doc.FirstChildElement(enemy_name.c_str());
    if (botNode == NULL) {
        botNode = doc.NewElement(enemy_name.c_str());
        doc.InsertFirstChild(botNode);
    }

    myBotNode = botNode->FirstChildElement(bot_name.c_str());
    if (myBotNode == NULL) {
        myBotNode = doc.NewElement(bot_name.c_str());
        botNode->InsertFirstChild(myBotNode);
    }

    if (resultToString(gameResult) == "loss") {
        queryNode = myBotNode->FirstChildElement("losses");
        const char* query_str = "losses";
        if (queryNode == NULL) {
            queryNode = doc.NewElement(query_str);
            queryNode->SetText(1);
            myBotNode->InsertFirstChild(queryNode);
        }
        else {
            queryNode->QueryIntText(&value);
            queryNode->SetText(value + 1);
        }
    }
    else if (resultToString(gameResult) == "draw") {
        queryNode = myBotNode->FirstChildElement("draws");
        const char* query_str = "draws";
        if (queryNode == NULL) {
            queryNode = doc.NewElement(query_str);
            queryNode->SetText(1);
            myBotNode->InsertFirstChild(queryNode);
        }
        else {
            queryNode->QueryIntText(&value);
            queryNode->SetText(value + 1);
        }
    }
    else if (resultToString(gameResult) == "win") {
        queryNode = myBotNode->FirstChildElement("wins");
        const char* query_str = "wins";
        if (queryNode == NULL) {
            queryNode = doc.NewElement(query_str);
            queryNode->SetText(1);
            myBotNode->InsertFirstChild(queryNode);
        }
        else {
            queryNode->QueryIntText(&value);
            queryNode->SetText(value + 1);
        }
    }
    doc.SaveFile(Configuration::getInstance()->matchDataFile.c_str());
}

MatchData* MatchData::getInstance() {
    if (instance == NULL) {
        instance = new MatchData();
    }
    return instance;
}

string MatchData::resultToString(int result) {
    switch (result) {
    case WIN:
        return "win";

    case LOSS:
        return "loss";

    case DRAW:
        return "draw";

    default:
        Broodwar->printf("Invalid game result %d!", result);
        return "invalid";
    }
}

string MatchData::getSummaryFilename() {
    stringstream ss;
    ss << Configuration::OUTPUT_DIR;	//bwapi-data/AI or /write
    //ss << "bwapi-data\\write\\"; //Tournament persistent storage version
    ss << "megabot_matchsummary.csv";

    return ss.str();
}

void MatchData::writeSummary() {

    stringstream ss;
    ss << Broodwar->self()->getRace().getName() << ";";
    ss << myBehaviorName << ";";
    //ss << StrategySelector::getInstance()->getStrategyID() << ";";
    ss << Broodwar->enemy()->getRace().getName() << ";";
    ss << enemyBehaviorName << ";";
    ss << Broodwar->mapFileName() << ";";
    if (gameResult == WIN) ss << "Won";
    if (gameResult == LOSS) ss << "Lost";
    if (gameResult == DRAW) ss << "Draw";
    ss << ";";
    ss << Broodwar->self()->getUnitScore() << ";";
    ss << Broodwar->self()->getBuildingScore() << ";";
    ss << Broodwar->self()->getKillScore() << ";";
    ss << Broodwar->enemy()->getUnitScore() << ";";
    ss << Broodwar->enemy()->getBuildingScore() << ";";
    ss << Broodwar->enemy()->getKillScore();
    ss << "\n";

    //Save the file
    string filename = getSummaryFilename();

    ofstream outFile;
    outFile.open(filename.c_str(), ios::out | ios::app);
    if (!outFile) {
        Broodwar->printf("Error writing to stats file!\n");
    }
    else {
        outFile << ss.str();
        outFile.close();
    }
}

const string MatchData::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct, &now);//   localtime_s(&tm, &now);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}