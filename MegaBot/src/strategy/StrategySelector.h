#ifndef __STRATEGYSELECTOR_H__
#define __STRATEGYSELECTOR_H__

#include <BWAPI.h>
#include "../utils/Logging.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"

using namespace BWAPI;
using namespace std;
/*
struct StrategyStats {
    string mapHash;
    string mapName;
    string strategyId;
    string ownRace;
    string opponentRace;
    int won;
    int lost;
    int draw;
    int total;

    StrategyStats() {
        won = 0;
        lost = 0;
        draw = 0;
        total = 0;
    }

    int getTotal() {
        if (total == 0) return 1; //To avoid division by zero.
        return total;
    }

    bool matches() {
        string mMapHash = Broodwar->mapHash();
        string mOwnRace = Broodwar->self()->getRace().getName();
        if (mMapHash == mapHash && mOwnRace == ownRace) {
            Race oRace = Broodwar->enemy()->getRace();
            if (oRace.getID() != Races::Unknown.getID()) {
                //Opponent race is known. Match race as well.
                if (oRace.getName() == opponentRace) {
                    return true;
                }
                else {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

struct Strategy {
    Race race;
    string strategyId;

    Strategy(Race mRace, string mId) {
        race = mRace;
        strategyId = mId;
    }
};
*/

class StrategySelector {

private:   


protected:
	//name of behavior in previous frame
    //string oldBehaviorName;

	//name of current behavior
	//string strategyName;
    //string currentStrategy;
	boost::mt19937 rng; //();	//mersenne twister random number generator
	//boost::random::uniform_int_distribution<> unifInt; //(0, portfolio.size()); //uniform integer distribution

	BWAPI::AIModule* currentStrategy;

	//the name of this meta strategy
	string name;

	//maps the behaviors to their respective names
    std::map<BWAPI::AIModule*, string> strategyNames;

    //maps behavior names to their AIModules
    std::map<string, BWAPI::AIModule*> portfolio;

	/** Returns a strategy uniformly at random */
	BWAPI::AIModule* randomUniform(); 


public:

	StrategySelector();
    ~StrategySelector();

	static const string SKYNET;		//"Skynet"
	static const string XELNAGA;	//"Xelnaga"
	static const string NUSBot;		//"NUSBot"

	//returns the meta strategy name
	string getName();

	//returns the active strategy
    BWAPI::AIModule* getCurrentStrategy();

	//returns active behavior name
	string getCurrentStrategyName();

	//acts every frame (may switch strategy or not)
	virtual void onFrame() {}

	//initializes the portfolio of behaviors
	virtual void onStart();

	//forces the adoption of a strategy
	virtual void forceStrategy(string strategyName);

    /** Prints debug info to the screen. */
    void printInfo();

};

#endif
