#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>

using namespace std;

class Configuration {
	//the singleton instance of this class
	static Configuration* instance;

	Configuration();

public:
	//identifies the strategy the bot will use
	string strategyID;

	//path to write the file with match details
	string matchDataFile;

	//path to xml file with strategy definition
	string strategyFile;

	//path to the xml file with the win percentages table by bot
	string winTableFile;

	//initial match speed
	int speed;

	//enable GUI (false makes match go much faster)
	bool enableGUI;

	//Returns the singleton instance of this class
	static Configuration* getInstance();

	~Configuration();

	void parseConfig();

	//Directory to read input from
	static const string INPUT_DIR;

	//Directory to write output to
	static const string OUTPUT_DIR;

	//File to read configs from
	static const string CONFIG_FILE;

	//directory that stores openings (build orders)
	static const string OPENINGS_DIR;

	//xml field that contains the build order ID
	static const string FIELD_STRATEGY_ID;

	//xml field that contains the match data file
	static const string FIELD_MATCH_DATA_FILE;

	//xml field that contains the match data file
	static const string FIELD_STRATEGY_FILE;

	//xml field that contains the initial match speed
	static const string FIELD_SPEED;

	//xml field that contains 'enable/disable' GUI
	static const string FIELD_ENABLE_GUI;

	//xml field that contains the path to the win table file
	static const string WIN_TABLE_FILE;
};

#endif
