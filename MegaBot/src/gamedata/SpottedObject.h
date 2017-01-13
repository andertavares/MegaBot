#ifndef __SPOTTEDOBJECT_H__
#define __SPOTTEDOBJECT_H__

#include <BWAPI.h>

//using namespace BWAPI;
using namespace std;

/** The SpottedObject class is a help class for the ExplorationManager. It contains all details about a spotted
* enemy unit or neutral resource.
*
* Author: Johan Hagelback (johan.hagelback@gmail.com)
*/
class SpottedObject {

private:
    BWAPI::UnitType* type;

    /** x coordinate, in pixels */
	int x;

	/** y coordinate, in pixels */
	int y;

	/** Stores the unit type name */
	string typeName;

    int unitID;
	int lastSeenFrame;

public:
	/** Default constructor, initializes 'invalid' values */
	SpottedObject();

	/** Draws a box around the spotted object is believed to be */
	void draw();

    /** Creates an object from a unit reference. */
    SpottedObject(BWAPI::Unit* mUnit);

    /** Incorporate updated information from the recently seen unit */
    void update(BWAPI::Unit* mUnit);

    /** Returns the unique id of the spotted unit. */
    int getUnitID();

    /** Returns the type of the spotted unit. */
    BWAPI::UnitType getType();

    /** Returns the x coordinate, in pixels, where the unit is believed to be. */
    int getX();

	/** Returns the y coordinate, in pixels, where the unit is believed to be. */
	int getY();

	/** Returns the frame in which object was seen for last time*/
	int getLastSeenFrame();
};


#endif
