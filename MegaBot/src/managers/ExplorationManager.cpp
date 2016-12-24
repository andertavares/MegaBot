#include "BuildingPlacer.h"
#include "ExplorationManager.h"

ExplorationManager* ExplorationManager::instance = NULL;

int SpottedObjectSet::countUnitsOfType(UnitType type) {
    int count = 0;
    for each (BWAPI::Unit * unit in BWAPI::Broodwar->getAllUnits()) {
        if (unit->getType() == type) {
            count++;
        }
    }
    return count;
}

ExplorationManager::ExplorationManager()
{
	
}

ExplorationManager::~ExplorationManager()
{
	instance = NULL;
}

ExplorationManager* ExplorationManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new ExplorationManager();
	}
	return instance;
}

SpottedObjectSet& ExplorationManager::getSpottedBuildings(){
	return enemyBuildings;
}

SpottedObjectSet& ExplorationManager::getSpottedUnits(){
	return enemyUnits;
}

bool ExplorationManager::enemyIsProtoss()
{
	for each (Player* u in Broodwar->getPlayers())
	{
		if (u->isEnemy(Broodwar->self()))
		{
			if (u->getRace().getID() == Races::Protoss.getID())
			{
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsZerg()
{
    for each (Player* u in Broodwar->getPlayers())
	{
		if (u->isEnemy(Broodwar->self()))
		{
			if (u->getRace().getID() == Races::Zerg.getID())
			{
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsTerran()
{
    for each (Player* u in Broodwar->getPlayers())
	{
		if (u->isEnemy(Broodwar->self()))
		{
			if (u->getRace().getID() == Races::Terran.getID())
			{
				return true;
			}
		}
	}
	return false;
}

bool ExplorationManager::enemyIsUnknown()
{
	if (!enemyIsTerran() && !enemyIsProtoss() && !enemyIsZerg())
	{
		return true;
	}
	return false;
}

