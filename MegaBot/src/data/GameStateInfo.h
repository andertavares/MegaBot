#ifndef __GAMESTATEINFO_H__
#define __GAMESTATEINFO_H__

#include <string>
#include <BWAPI.h>

using namespace std;

class GameStateInfo {

private:

    static GameStateInfo* instance;
    GameStateInfo();

public:
    /** Returns the instance of the class. */
    static GameStateInfo* getInstance();

    /** Destructor */
    ~GameStateInfo();

	/** To be called every frame (although won't act every time) */
	void onFrame();

    int numBases(string name, BWAPI::Race race);
    int terrestrialSmallUnits(string name);
    int terrestrialMediumUnits(string name);
    int terrestrialLargeUnits(string name);
    int flyingSmallUnits(string name);
    int flyingMediumUnits(string name);
    int flyingLargeUnits(string name);
    int unknownUnits(string name);
};

#endif