#include "MilitaryEvaluator.h"
#include "../managers/ExplorationManager.h"

MilitaryEvaluator* MilitaryEvaluator::instance = NULL;

MilitaryEvaluator::MilitaryEvaluator() {
    explorationManager = ExplorationManager::getInstance();
}

MilitaryEvaluator::~MilitaryEvaluator() {
    instance = NULL;
}

MilitaryEvaluator* MilitaryEvaluator::getInstance() {
    if (instance == NULL) {
        instance = new MilitaryEvaluator();
    }
    return instance;
}



MilitaryForce MilitaryEvaluator::evaluateEnemyAir() {
    int numHeavyUnits = 0;	//accounts for Battlecruisers, Carriers, Guardians
    int numLightUnits = 0;	//accounts for Mutalisks, Wraiths, Scouts
    SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();

    numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Battlecruiser)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Carrier)
        + enemyUnits.countUnitsOfType(UnitTypes::Zerg_Guardian)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Arbiter);

    numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Zerg_Mutalisk)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Wraith)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Scout)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Corsair);

    if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some air force, won't return NONE
        if (numHeavyUnits > 5 && numLightUnits > 10) {
            return MIXED_MANY;
        }
        else if (numHeavyUnits < 5 && numLightUnits < 10) {
            return MIXED_FEW;
        }
        else if (numHeavyUnits > 5 && numLightUnits < 10) {
            return HEAVY_MANY;
        }
        else if (numHeavyUnits < 5 && numLightUnits < 3) {
            return HEAVY_FEW;
        }
        else if (numHeavyUnits < 5 && numLightUnits > 10) {
            return LIGHT_MANY;
        }
        else if (numHeavyUnits < 2 && numLightUnits < 10) {
            return LIGHT_FEW;
        }
        /*//won't bother returning MIXED_MANY because HEAVY_MANY is already troublesome
        if (numHeavyUnits > 5) {
        return HEAVY_MANY;
        }
        else if (numLightUnits > 10 && numHeavyUnits < 3){
        return LIGHT_MANY;
        }
        else if (numLightUnits < 10 && numHeavyUnits < 3){
        return LIGHT_FEW;
        }
        else if (numLightUnits > 0 && numHeavyUnits > 0){
        return MIXED_FEW;
        }
        else return HEAVY_FEW;*/
    }

    return NONE;
}

MilitaryForce MilitaryEvaluator::evaluateEnemyMech() {
    int numHeavyUnits = 0;	//accounts for Battlecruisers, Carriers, Guardians
    int numLightUnits = 0;	//accounts for Mutalisks, Wraiths, Scouts
    SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();

    numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Battlecruiser)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Siege_Mode)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Carrier)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Reaver)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Arbiter);

    numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Vulture)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Science_Vessel)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Wraith)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Goliath)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Scout)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Corsair);

    if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some mech, won't return NONE

        //won't bother returning MIXED_MANY because HEAVY_MANY is already troublesome
        if (numHeavyUnits > 5) {
            return HEAVY_MANY;
        }
        else if (numLightUnits > 10 && numHeavyUnits < 3) {
            return LIGHT_MANY;
        }
        else if (numLightUnits < 10 && numHeavyUnits < 3) {
            return LIGHT_FEW;
        }
        else if (numLightUnits > 0 && numHeavyUnits > 0) {
            return MIXED_FEW;
        }
        else return HEAVY_FEW;
    }

    return NONE;
}

MilitaryForce MilitaryEvaluator::evaluateEnemyLand() {
    int numHeavyUnits = 0;	//accounts for Infantry, Lings, Hydras, Zealots, Goons, High Templars, DTs
    int numLightUnits = 0;	//accounts for Tanks, Ultralisks, Reavers, Archon, defensive buildings
    SpottedObjectSet& enemyUnits = explorationManager->getSpottedUnits();
    SpottedObjectSet& enemyBldgs = explorationManager->getSpottedBuildings();

    numLightUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Marine)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Firebat)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Ghost)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Goliath)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Vulture)
        + enemyUnits.countUnitsOfType(UnitTypes::Zerg_Zergling)
        + enemyUnits.countUnitsOfType(UnitTypes::Zerg_Hydralisk)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Zealot)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Dragoon)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Dark_Templar)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_High_Templar);

    numHeavyUnits = enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Siege_Mode)
        + enemyUnits.countUnitsOfType(UnitTypes::Terran_Siege_Tank_Tank_Mode)
        + enemyUnits.countUnitsOfType(UnitTypes::Zerg_Ultralisk)
        + enemyUnits.countUnitsOfType(UnitTypes::Zerg_Lurker)
        + enemyUnits.countUnitsOfType(UnitTypes::Zerg_Lurker_Egg)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Reaver)
        + enemyUnits.countUnitsOfType(UnitTypes::Protoss_Archon)
        + 2 * enemyUnits.countUnitsOfType(UnitTypes::Zerg_Sunken_Colony)		//defensive buildings count more
        + 2 * enemyUnits.countUnitsOfType(UnitTypes::Protoss_Photon_Cannon)
        + 2 * enemyUnits.countUnitsOfType(UnitTypes::Terran_Bunker);

    if (numLightUnits > 0 || numHeavyUnits > 0) {	//enemy has some land force, won't return NONE

        if (numHeavyUnits > 5 && numLightUnits > 10) {
            return MIXED_MANY;
        }
        else if (numHeavyUnits < 5 && numLightUnits < 10) {
            return MIXED_FEW;
        }
        else if (numHeavyUnits > 5 && numLightUnits < 10) {
            return HEAVY_MANY;
        }
        else if (numHeavyUnits < 5 && numLightUnits < 3) {
            return HEAVY_FEW;
        }
        else if (numHeavyUnits < 5 && numLightUnits > 10) {
            return LIGHT_MANY;
        }
        else if (numHeavyUnits < 2 && numLightUnits < 10) {
            return LIGHT_FEW;
        }

    }

    return NONE;
}

bool MilitaryEvaluator::needAntiAir() {
    MilitaryForce enemyAir = evaluateEnemyAir();

    return (
        enemyAir == LIGHT_MANY || enemyAir == MIXED_FEW
        || enemyAir == MIXED_MANY || enemyAir == HEAVY_FEW || enemyAir == HEAVY_MANY
        );
}

string MilitaryEvaluator::stringOf(MilitaryForce force) {
    if (force == NONE) return "None";
    if (force == LIGHT_FEW) return "Light, few";
    if (force == LIGHT_MANY) return "Light, many";
    if (force == HEAVY_FEW) return "Heavy, few";
    if (force == HEAVY_MANY) return "Heavy, many";
    if (force == MIXED_FEW) return "Mixed, few";
    if (force == MIXED_MANY) return "Mixed, many";
    else return "";
}