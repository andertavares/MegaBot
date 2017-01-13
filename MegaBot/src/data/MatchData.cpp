#include "MatchData.h"
#include <ctime>
#include <BWAPI.h>
#include <fstream>
#include <sstream>
#include <algorithm> 

#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"

#include "Configuration.h"

using namespace BWAPI;
using namespace std;

MatchData* MatchData::instance = NULL;

MatchData::MatchData() : metaStrategyName("unknown") {
	logger = Logging::getInstance();
}


MatchData::~MatchData() {}

void MatchData::registerMatchBegin() {
    startTime = currentDateTime();
	logger->log("Match started at: %s", startTime.c_str());
	logger->log("Map is: %s", Broodwar->mapFileName().c_str());
	logger->log("Enemy is: %s", Broodwar->enemy()->getName().c_str() );

}

void MatchData::registerMatchFinish(int result) {
    gameResult = result;

    //attempts to retrieve enemy score information (not working, currently BWAPI doesn't allow retrieval of enemy info even at match end)
    Broodwar->enableFlag(Flag::CompleteMapInformation);

    //registers scores of both players
    Player* me = Broodwar->self();
    Player* enemy = Broodwar->enemy();

	logger->log("Match finished at %s", currentDateTime().c_str());
}

void MatchData::registerMetaStrategy(string name) {
	metaStrategyName = name;
}

string MatchData::getMetaStrategyName() {
	return metaStrategyName;
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

    tinyxml2::XMLDocument doc;
    XMLError input_result = doc.LoadFile(inputFile.c_str());

    // if file was not found, ok, we create a node and fill information in it
    if (input_result == XML_ERROR_FILE_NOT_FOUND) {
        rootNode = doc.NewElement("scores");
        doc.InsertFirstChild(rootNode);
    }
    // if another error occurred, we're in trouble =/
    else if (input_result != XML_NO_ERROR) {
		
        /*Broodwar->printf(
            "Error while parsing the configuration file '%s'. Error: '%s'",
            inputFile,
            doc.ErrorName()
        );*/
		logger->log(
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
		logger->log("Invalid game result! %s", gameResult);
        throw exception("Invalid game result!");
    }

    float oldScore;
    float score = 0;
    float alpha = Configuration::getInstance()->alpha; //alias for easy reading


    myBehvNode = rootNode->FirstChildElement(myBehaviorName.c_str());
    if (myBehvNode == NULL) {
        score = alpha*result_value;
        myBehvNode = doc.NewElement(myBehaviorName.c_str());
        myBehvNode->SetText(score);
        rootNode->InsertFirstChild(myBehvNode);
    }
    else {
        myBehvNode->QueryFloatText(&oldScore);
        score = (1 - alpha)*oldScore + alpha*result_value;
        myBehvNode->SetText(score);
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
        //Broodwar->printf("Invalid game result %d!", result);
		logger->log("Invalid game result %d!", result);
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
    //ss << MetaStrategy::getInstance()->getStrategyID() << ";";
    ss << Broodwar->enemy()->getRace().getName() << ";";
	ss << Broodwar->enemy()->getName() << ";";
    ss << enemyBehaviorName << ";";
    ss << Broodwar->mapFileName() << ";";
    if (gameResult == WIN) ss << "Won";
    if (gameResult == LOSS) ss << "Lost";
    if (gameResult == DRAW) ss << "Draw";
    ss << ";";
    ss << Broodwar->self()->getUnitScore() << ";";
	ss << Broodwar->self()->getKillScore() << ";";
    ss << Broodwar->self()->getBuildingScore() << ";";
	ss << Broodwar->self()->getRazingScore() << ";";
    
    ss << Broodwar->enemy()->getUnitScore() << ";";
	ss << Broodwar->enemy()->getKillScore();
    ss << Broodwar->enemy()->getBuildingScore() << ";";
	ss << Broodwar->enemy()->getRazingScore() << ";";
    

	logger->log(ss.str().c_str());

	ss << "\n";	//adds newline to write in summary file (logger doesn't need it)

    //Save the file
    string filename = getSummaryFilename();

    ofstream outFile;
    outFile.open(filename.c_str(), ios::out | ios::app);
    if (!outFile) {
		logger->log("Error writing to stats file!");
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

void MatchData::writeToCrashFile() {
    using namespace tinyxml2;

    string bot_name = myBehaviorName;

    XMLElement* rootNode;
    XMLElement* myBehvNode;
    XMLElement* queryNode;

    string inputFile = Configuration::getInstance()->crashInformationInputFile();
    string outputFile = Configuration::getInstance()->crashInformationOutputFile();

    tinyxml2::XMLDocument doc;
    XMLError input_result = doc.LoadFile(inputFile.c_str());

    // if file was not found, ok, we create a node and fill information in it
    if (input_result == XML_ERROR_FILE_NOT_FOUND) {
        rootNode = doc.NewElement("crashes");
        doc.InsertFirstChild(rootNode);
    }
    // if another error occurred, we're in trouble =/
    else if (input_result != XML_NO_ERROR) {
		logger->log(
            "Error while parsing the crash file '%s'. Error: '%s'",
            inputFile,
            doc.ErrorName()
        );
        return;
    }
    else { //no error, goes after root node
        rootNode = doc.FirstChildElement("crashes");
        if (rootNode == NULL) {
            rootNode = doc.NewElement("crashes");
            doc.InsertFirstChild(rootNode);
        }
    }

    float oldScore;

    myBehvNode = rootNode->FirstChildElement(myBehaviorName.c_str());
    if (myBehvNode == NULL) {
        myBehvNode = doc.NewElement(myBehaviorName.c_str());
        myBehvNode->SetText(0);
        rootNode->InsertFirstChild(myBehvNode);
    }
    else {
        myBehvNode->QueryFloatText(&oldScore);
        myBehvNode->SetText(oldScore + 1);
    }

    doc.SaveFile(outputFile.c_str());
}

void MatchData::updateCrashFile() {
    using namespace tinyxml2;

    string bot_name = myBehaviorName;

    XMLElement* rootNode;
    XMLElement* myBehvNode;

    string outputFile = Configuration::getInstance()->crashInformationOutputFile();

    tinyxml2::XMLDocument doc;
    XMLError input_result = doc.LoadFile(outputFile.c_str());

    // if another error occurr, we're in trouble =/
    if (input_result != XML_NO_ERROR) {
		logger->log(
            "Error while parsing the crash file '%s'. Error: '%s'",
            outputFile,
            doc.ErrorName()
        );
        return;
    }
    else { //no error, goes after root node
        rootNode = doc.FirstChildElement("crashes");
        if (rootNode != NULL) {
            myBehvNode = rootNode->FirstChildElement(myBehaviorName.c_str());
            if (myBehvNode != NULL) {
                myBehvNode->SetText(0);
            }
            doc.SaveFile(outputFile.c_str());
        }
    }
}
