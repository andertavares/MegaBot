#ifndef __MATCHDATA_H__
#define __MATCHDATA_H__

#include <string>
#include "../utils/Logging.h"

using namespace std;

class MatchData {
	//singleton instance of this class
	static MatchData* instance;

	//instance of logger object
	Logging* logger;

	//stores the behavior used in this match
	string myBehaviorName;

	//stores enemy behavior used in this match
	string enemyBehaviorName;
	
	//start and finishing timestamps, stored as strings
	string startTime, endTime; 

	/** adopted metastrategy */
	string metaStrategyName;

	// game duration in frames
	int frameCount;

	// game duration in seconds, according to BWAPI
	int duration;

	// my score data
	int myUnitScore, myKillScore, myBuildingScore, myRazingScore, myGatheredMinerals, myGatheredGas, myTotal;

	// enemy score data
	int enemyUnitScore, enemyKillScore, enemyBuildingScore, enemyRazingScore, enemyGatheredMinerals, enemyGatheredGas, enemyTotal;

	int gameResult;
	
	MatchData();

public:
	
	~MatchData();

	/** Returns the singleton instance of this class */
	static MatchData* getInstance();

	/** Gets current timestamp and registers it as start time of the match*/
	void registerMatchBegin();

	/** Returns the path to the output file containing the summary of matches*/
	string getSummaryFilename();


	/** 
	 * Registers match result, end time and score data
	 */
	void registerMatchFinish(int result);

	/** Registers the chosen meta strategy */
	void registerMetaStrategy(string name);

	/** Returns the name of the registered meta strategy */
	string getMetaStrategyName();

	// registers my behavior used in this match
	void registerMyBehaviorName(string name);

	// registers enemy behavior used in this match
	void registerEnemyBehaviorName(string name);

	/* Writes match result in a file */
	void writeDetailedResult();

    /* Writes info of a crash event in a file */
    void writeToCrashFile();

    //set the chosen strategy counter to zero
    void updateCrashFile();

	// Appends the summary of this match to the respective output file
	void writeSummary();

	/* Returns a string corresponding to the match result. */
	string resultToString(int result);

	// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
	const string currentDateTime();

	//code for loss
	static const int LOSS = 0;

	// code for victory
	static const int WIN = 1;

	//code for draw
	static const int DRAW = 2;

};

#endif