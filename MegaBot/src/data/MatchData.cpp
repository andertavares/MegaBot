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
    using namespace tinyxml2;

    string bot_name = myBehaviorName;

    /* DEBUG
    ofstream outFile;
    outFile.open("bwapi-data/write/dbg.txt", ios::out | ios::app);
    */

    XMLElement* rootNode;
    XMLElement* myBehvNode;
    XMLElement* queryNode;

    string inputFile = Configuration::getInstance()->enemyInformationInputFile();
    string outputFile = Configuration::getInstance()->enemyInformationOutputFile();

    //const char* filename = Configuration::getInstance()->readDataFile.c_str();

    XMLDocument doc;
    XMLError input_result = doc.LoadFile(inputFile.c_str());

    // if file was not found, ok, we create a node and fill information in it
    if (input_result == XML_ERROR_FILE_NOT_FOUND) {
        rootNode = doc.NewElement("scores");
        doc.InsertFirstChild(rootNode);
    }
    // if another error occurred, we're in trouble =/
    else if (input_result != XML_NO_ERROR) {
        Broodwar->printf(
            "Error while parsing the configuration file '%s'. Error: '%s'",
            inputFile,
            doc.ErrorName()
            );
        return;
    }
    else { //no error, goes after root node
        rootNode = doc.FirstChildElement("scores");
        if (rootNode == NULL) {
            rootNode = doc.NewElement("scores");
            doc.InsertFirstChild(rootNode);
        }
    }

    //finds information with bot node
    /*botNode = doc.FirstChildElement("results")->FirstChildElement(enemy_name.c_str());
    if (botNode == NULL) {
    botNode = doc.NewElement(enemy_name.c_str());
    doc.InsertFirstChild(botNode);
    }*/

    int result_value = 0;
    //queries wins, losses or draws node according to match result
    if (gameResult == LOSS) {
        result_value = -1;
    }
    else if (gameResult == DRAW) {
        result_value = 0;
    }
    else if (gameResult == WIN) {
        result_value = 1;
    }
    else {
        throw exception("Invalid game result!");
    }

    float value;
    float alpha = 0.2f;
    float score = 0;

    myBehvNode = rootNode->FirstChildElement(myBehaviorName.c_str());
    if (myBehvNode == NULL) {
        score = alpha*result_value;
        myBehvNode = doc.NewElement(myBehaviorName.c_str());
        myBehvNode->SetText(score);
        rootNode->InsertFirstChild(myBehvNode);
    }
    else {
        myBehvNode->QueryFloatText(&value);
        score = (1 - alpha)*value + alpha*result_value;
        myBehvNode->SetText(value + score);
    }

    doc.SaveFile(outputFile.c_str());
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