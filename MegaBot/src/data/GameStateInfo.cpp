#include "GameStateInfo.h"


#include <string>
#include <BWAPI.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>

#include "Configuration.h"
#include "../strategy/MetaStrategy.h"

using namespace std;

GameStateInfo* GameStateInfo::instance = NULL;

GameStateInfo* GameStateInfo::getInstance() {
    if (instance == NULL) {
        instance = new GameStateInfo();
    }
    return instance;
}

GameStateInfo::GameStateInfo() {}

GameStateInfo::~GameStateInfo() {}

int GameStateInfo::numBases(string name, BWAPI::Race race) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;

    if (race == BWAPI::Races::Protoss) {
        for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
            if (unit->getType() == BWAPI::UnitTypes::Protoss_Nexus && enemyName != name.c_str() && 
            !unit->getPlayer()->isEnemy(Broodwar->self())) {
                num++;
            }
            else if (unit->getType() == BWAPI::UnitTypes::Protoss_Nexus && enemyName == name.c_str() 
            && unit->getPlayer()->isEnemy(Broodwar->self())) {
                num++;
            }
        }
    }
    else if (race == BWAPI::Races::Terran) {
        for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
            if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center && enemyName != name.c_str() 
            && !unit->getPlayer()->isEnemy(Broodwar->self())) {
                num++;
            }
            else if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center && enemyName == name.c_str() 
            && unit->getPlayer()->isEnemy(Broodwar->self())) {
                num++;
            }
        }
    }
    else if (race == BWAPI::Races::Zerg) {
        for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
            if (unit->getType() == BWAPI::UnitTypes::Zerg_Hatchery && enemyName != name.c_str() &&
             !unit->getPlayer()->isEnemy(Broodwar->self())) {
                num++;
            }
            else if (unit->getType() == BWAPI::UnitTypes::Zerg_Hatchery && enemyName == name.c_str() 
            && unit->getPlayer()->isEnemy(Broodwar->self())) {
                num++;
            }
        }
    }
    return num;
}

int GameStateInfo::terrestrialSmallUnits(string name) { 
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (!unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Small && 
        enemyName != name.c_str() && !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (!unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Small
        && enemyName == name.c_str() && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}

int GameStateInfo::terrestrialMediumUnits(string name) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (!unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Medium && 
        enemyName != name.c_str() && !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (!unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Medium 
        && enemyName == name.c_str() && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}

int GameStateInfo::terrestrialLargeUnits(string name) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (!unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Large && 
        enemyName != name.c_str() && !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (!unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Large
        && enemyName == name.c_str() && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}

int GameStateInfo::flyingSmallUnits(string name) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Small && 
        enemyName != name.c_str() && !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Small 
        && enemyName == name.c_str() && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}

int GameStateInfo::flyingMediumUnits(string name) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Medium && 
        enemyName != name.c_str() && !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Medium 
        && enemyName == name.c_str() && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}

int GameStateInfo::flyingLargeUnits(string name) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Large && 
        enemyName != name.c_str() && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (unit->getType().isFlyer() && unit->getType().size() == BWAPI::UnitSizeTypes::Large 
        && enemyName == name.c_str() && !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}

int GameStateInfo::unknownUnits(string name) {
    string enemyName = BWAPI::Broodwar->enemy()->getName().c_str();
    int num = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (unit->getType().size() == BWAPI::UnitSizeTypes::Unknown && enemyName != name.c_str() && 
        !unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
        else if (unit->getType().size() == BWAPI::UnitSizeTypes::Unknown && enemyName == name.c_str() 
        && unit->getPlayer()->isEnemy(Broodwar->self())) {
            num++;
        }
    }
    return num;
}
