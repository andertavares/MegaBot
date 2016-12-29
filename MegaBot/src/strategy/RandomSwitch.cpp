#include "RandomSwitch.h"
#include "../data/MatchData.h"

RandomSwitch::RandomSwitch(void) : MetaStrategy() {
	name = "Random Switch";
}


RandomSwitch::~RandomSwitch(void) {
}

void RandomSwitch::onStart(){
	MetaStrategy::onStart();

	//selects one behavior probabilistically
	currentStrategy = randomUniform();


}

void RandomSwitch::onFrame(){
	int thisFrame = Broodwar->getFrameCount();

	if (thisFrame % 5000 == 0 && thisFrame > 0) {  //behavior switch
        /*int playerBases = GameStateInfo::getInstance()->numBases(myBehaviorName.c_str(), BWAPI::Races::Protoss);
        Logging::getInstance()->log("Number of player's bases %d.", playerBases);

        BWAPI::Race enemyRace = BWAPI::Broodwar->enemy()->getRace();
        string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
        int enemyBases = GameStateInfo::getInstance()->numBases(enemyName, enemyRace);
        Logging::getInstance()->log("Number of enemy's bases %d.", enemyBases);

        int playerSmallUnits = GameStateInfo::getInstance()->terrestrialSmallUnits(myBehaviorName.c_str());
        Logging::getInstance()->log("Number of player's small units %d.", playerSmallUnits);

        string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
        int enemySmallUnits = GameStateInfo::getInstance()->terrestrialSmallUnits(enemyName);
        Logging::getInstance()->log("Number of enemy's small units %d.", enemySmallUnits);
		*/

		string oldBehaviorName = getCurrentStrategyName();
        Logging::getInstance()->log("Will switch strategy in frame %d.", thisFrame);
        /*double lucky = (rand() / (double)(RAND_MAX + 1));

        if (lucky < 0.33) {
			myBehaviorName = MetaStrategy::NUSBot;
        }
        else if (lucky < 0.66) {
            myBehaviorName = MetaStrategy::SKYNET;
        }
        else {
            myBehaviorName = MetaStrategy::XELNAGA;
        }*/

		currentStrategy = randomUniform();

		Logging::getInstance()->log("Switching: %s -> %s", oldBehaviorName.c_str(), getCurrentStrategyName().c_str());
        MatchData::getInstance()->registerMyBehaviorName(getCurrentStrategyName());
        //currentBehavior = behaviors[myBehaviorName];
        //currentBehavior->onFrame();
        //Logging::getInstance()->log("%s on!", myBehaviorName.c_str());
    }

    

    //sends behavior communication message every 200 frames
    /*if (!acknowledged && (BWAPI::Broodwar->getFrameCount() % 200) == 0) {
    Broodwar->sendText("%s on!", myBehaviorName.c_str());
    }
	*/
}