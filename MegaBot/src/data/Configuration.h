#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>

using namespace std;


// Returns enemy name with spaces replaced by underscore
string _enemyName();

// Auxiliary to indicate whether a char is a space
bool _isSpace(char);

class Configuration {
	//the singleton instance of this class
	static Configuration* instance;

	Configuration();

public:
	//identifies the strategy the bot will use
	string strategyID;

	//path to write the file with match details
	string matchDataFile;

	//beginning of file name to look for information about enemy
    string enemyInformationPrefix;

	//path to xml file with strategy definition
	string strategyFile;

	//path to the xml file with the win percentages table by bot
	string winTableFile;

	//initial match speed
	int speed;

	//enable GUI (false makes match go much faster)
	bool enableGUI;

	//learning rate for strategy value (score) updates
	float alpha;
	
	//rate of exploration
	float epsilon;

	//Returns the singleton instance of this class
	static Configuration* getInstance();

	~Configuration();

	void parseConfig();

	// path to file to read enemy information
	string enemyInformationInputFile();

	// path to file to write enemy information
	string enemyInformationOutputFile();

	//Directory to read input from
	static const string INPUT_DIR;

	//Directory to write output to
	static string OUTPUT_DIR;

    //Directory to read informations about the tournament
    static string READ_DIR;

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

    //xml field that contains the read directory
    static const string FIELD_READ_DIR;
    
    //xml field that contains the write directory
    static const string FIELD_WRITE_DIR;

	//xml field that contains the initial match speed
	static const string FIELD_SPEED;

	//xml field that contains 'enable/disable' GUI
	static const string FIELD_ENABLE_GUI;

	//xml field that contains the path to the win table file
	static const string WIN_TABLE_FILE;

	//xml field that contains alpha
	static const string FIELD_ALPHA;

	//xml field that contains epsilon
	static const string FIELD_EPSILON;
};

#endif
