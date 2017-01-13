#ifndef __MetaStrategy_H__
#define __MetaStrategy_H__

#include <BWAPI.h>
#include "../utils/Logging.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"

using namespace BWAPI;
using namespace std;

/** Base class for meta strategies */
class MetaStrategy {

private:   


protected:
	//name of behavior in previous frame
    //string oldBehaviorName;

	boost::mt19937 rng; //mersenne twister random number generator

	/** Active behavior (corresponds to a bot) */
	BWAPI::AIModule* currentStrategy;

	/** Name of this meta strategy */
	string name;

	/** Maps behaviors to their respective names */
    std::map<BWAPI::AIModule*, string> strategyNames;

    /** Maps behavior names to their respectives AIModules */
    std::map<string, BWAPI::AIModule*> portfolio;

	/** Returns a strategy uniformly at random */
	BWAPI::AIModule* randomUniform(); 


public:

	MetaStrategy();
    ~MetaStrategy();

	static const string SKYNET;		//"Skynet"
	static const string XELNAGA;	//"Xelnaga"
	static const string NUSBot;		//"NUSBot"

	/** Returns the meta strategy name */
	string getName();

	/** Returns the active strategy */
    BWAPI::AIModule* getCurrentStrategy();

	/** Returns active behavior name */
	string getCurrentStrategyName();

	/** Acts every frame (may switch strategy or not) */
	virtual void onFrame() {}

	/** Initializes the portfolio of behaviors */
	virtual void onStart();

	/** Forces the adoption of a strategy */
	virtual void forceStrategy(string strategyName);

    /** Prints debug info to the screen. */
    void printInfo();

};

#endif
