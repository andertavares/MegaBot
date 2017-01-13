#include "Configuration.h"
#include "../utils/tinyxml2.h"
#include "../utils/Logging.h"
#include "../MegaBot.h"
#include <sstream>
#include <algorithm> 
#include <fstream>
#include <BWAPI.h>

//BEGIN: change these if you need to read/write in different folders
const string Configuration::INPUT_DIR = "bwapi-data/AI/";
string Configuration::OUTPUT_DIR = "bwapi-data/write/";
string Configuration::READ_DIR = "bwapi-data/read/";
//END: change these if you need to read/write in different folders

//Unless you're programming MegaBot, don't change consts below
const string Configuration::CONFIG_FILE = Configuration::INPUT_DIR + "megabot_config.xml";

//xml field names
const string Configuration::FIELD_META_STRATEGY_ID = "meta-strategy";
const string Configuration::FIELD_MATCH_DATA_FILE = "match-output";
const string Configuration::FIELD_STRATEGY_FILE = "strategy-file";
const string Configuration::FIELD_READ_DIR = "read-dir";
const string Configuration::FIELD_WRITE_DIR = "write-dir";
const string Configuration::FIELD_SPEED = "speed";
const string Configuration::FIELD_ENABLE_GUI = "gui";
const string Configuration::FIELD_ALPHA = "alpha";
const string Configuration::FIELD_EPSILON = "epsilon";

const string Configuration::WIN_TABLE_FILE = "win-table"; 

Configuration* Configuration::instance = NULL;

Configuration::Configuration() {
	//sets up default values
	matchDataFile = OUTPUT_DIR + "output.xml";
    //readDataFile = READ_DIR + "output.xml";
    strategyFile = INPUT_DIR + "megabot_protoss-uniform.xml";

	enemyInformationPrefix = "MegaBot-vs-";
    crashInformationPrefix = "crash_MegaBot-vs-";

	metaStrategyID = "epsilon-greedy"; 
	speed = 0;
	enableGUI = true;

	alpha = 0.2f;
	epsilon = 0.15f;
}

Configuration* Configuration::getInstance() {
	if (instance == NULL) {
		instance = new Configuration();
	}
	return instance;
}


Configuration::~Configuration() {
}


string Configuration::enemyInformationInputFile(){
	return READ_DIR + enemyInformationPrefix + _enemyName() + ".xml";
}

string Configuration::enemyInformationOutputFile(){
	return OUTPUT_DIR + enemyInformationPrefix + _enemyName() + ".xml";
}

string Configuration::crashInformationInputFile() {
    return READ_DIR + crashInformationPrefix + _enemyName() + ".xml";
}

string Configuration::crashInformationOutputFile() {
    return OUTPUT_DIR + crashInformationPrefix + _enemyName() + ".xml";
}

bool _isSpace(char caracter) {
    return caracter == ' ';
}

string _enemyName(){
	string enemyName =  BWAPI::Broodwar->enemy()->getName();
    std::replace_if(enemyName.begin(), enemyName.end(), _isSpace, '_');
	return enemyName;
}

void Configuration::parseConfig() {
	using namespace tinyxml2;
	using namespace BWAPI;

	tinyxml2::XMLDocument doc;	//namespace explicit to avoid ambiguity
	int result = doc.LoadFile(CONFIG_FILE.c_str());

	if (result != XML_NO_ERROR) {
		Logging::getInstance()->log(
			"An error has occurred while parsing the configuration file '%s'. Error: '%s'", 
			CONFIG_FILE.c_str(), 
			doc.ErrorName()
		);
		return;
	}
	Logging::getInstance()->log("Config file '%s' found. Parsing...", CONFIG_FILE.c_str());
	XMLElement* element;

	//sets meta-strategy
	element = doc.FirstChildElement("config")->FirstChildElement(FIELD_META_STRATEGY_ID.c_str());
	if (element) {
		metaStrategyID = string(element->Attribute("value"));
	}

	//sets speed
	element = doc.FirstChildElement("config")->FirstChildElement(FIELD_SPEED.c_str());
	if (element) {
		element->QueryIntAttribute("value", &speed);
	}

	//sets gui
	element = doc.FirstChildElement("config")->FirstChildElement(FIELD_ENABLE_GUI.c_str());
	if (element) {
		element->QueryBoolAttribute("value", &enableGUI);
	}

	//strategy file
	element = doc.FirstChildElement("config")->FirstChildElement(FIELD_STRATEGY_FILE.c_str());
	if (element) {
		strategyFile = Configuration::INPUT_DIR + string(element->Attribute("value"));
	}

    //read directory
    element = doc.FirstChildElement("config")->FirstChildElement(FIELD_READ_DIR.c_str());
    if (element) {
        Configuration::READ_DIR = string(element->Attribute("value"));
    }

    //write directoy
    element = doc.FirstChildElement("config")->FirstChildElement(FIELD_WRITE_DIR.c_str());
    if (element) {
        Configuration::OUTPUT_DIR = string(element->Attribute("value"));
    }

	//victory scorechart
	element = doc.FirstChildElement("config")->FirstChildElement(WIN_TABLE_FILE.c_str());
	if (element) {
		winTableFile = Configuration::INPUT_DIR + string(element->Attribute("value"));
	}
	
	//alpha
	element = doc.FirstChildElement("config")->FirstChildElement(FIELD_ALPHA.c_str());
	if (element) {
		element->QueryFloatAttribute("value", &alpha);
	}

	//epsilon
	element = doc.FirstChildElement("config")->FirstChildElement(FIELD_EPSILON.c_str());
	if (element) {
		Broodwar->printf("reading epsilon!");
		element->QueryFloatAttribute("value", &epsilon);
	}

	//traverses the XML looking for configurations
	/*
	for (XMLNode* data = doc.FirstChild()->FirstChild(); data; data = data->NextSibling()) {
		if (data->ToText())
	}*/

	//if matchDataFile <<
	//if buildOrderID <<


}