#ifndef __EXPLORATIONMANAGER_H__
#define __EXPLORATIONMANAGER_H__

#include "../gamedata/SpottedObject.h"
#include <BWTA.h>

using namespace BWAPI;
using namespace BWTA;
using namespace std;

class SpottedObjectSet {
public:
    /** Counts the number of units of a given type present in this set */
    int SpottedObjectSet::countUnitsOfType(UnitType type);
};


/** The ExplorationManager handles all tasks involving exploration of the game world. It issue orders to a number of units
 * that is used as explorers, keep track of areas recently explored, and keep track of spotted resources or enemy buildings.
 *
 * The ExplorationManager is implemented as a singleton class. Each class that needs to access ExplorationManager can request an instance,
 * and all classes shares the same ExplorationManager instance.
 *
 * Author: Johan Hagelback (johan.hagelback@gmail.com)
 */
class ExplorationManager {

private:
	SpottedObjectSet enemyBuildings;
	SpottedObjectSet enemyUnits;
	
	ExplorationManager();
	static ExplorationManager* instance;

public:
	/** Destructor */
	~ExplorationManager();

	/** Returns the instance of the class. */
	static ExplorationManager* getInstance();

	/** Returns the spotted enemy units (excluding buildings) */
	SpottedObjectSet& getSpottedUnits();

	/** Return spotted enemy buildings (excluding units) */
	SpottedObjectSet& getSpottedBuildings();

	/** Returns true if an enemy is Protoss. */
	static bool enemyIsProtoss();

	/** Returns true if an enemy is Zerg. */
	static bool enemyIsZerg();

	/** Returns true if an enemy is Terran. */
	static bool enemyIsTerran();

	/** All enemy races are currently unknown. */
	static bool enemyIsUnknown();
};

#endif
