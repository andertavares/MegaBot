#include "SpottedObject.h"
#include "../utils/Logging.h"

//using namespace BWAPI;

SpottedObject::SpottedObject(){
	type = NULL; //BWAPI::UnitTypes::None;
	x = y = -1 ; //BWAPI::Positions::None;
	unitID = -1;
	lastSeenFrame = -1;
}

SpottedObject::SpottedObject(BWAPI::Unit* mUnit) {
	type = new BWAPI::UnitType(mUnit->getType());
	x = mUnit->getPosition().x();	//x and y coords refer to the center of the unit
	y = mUnit->getPosition().y();
	//position = new BWAPI::Position(mUnit->getPosition());
	//tilePosition = new BWAPI::TilePosition(mUnit->getTilePosition());
    unitID = mUnit->getID();
	lastSeenFrame = BWAPI::Broodwar->getFrameCount();
}

void SpottedObject::update(BWAPI::Unit* mUnit) {
	
    if (unitID != -1 && unitID != mUnit->getID()) { //we allow updating when unitID is -1 because now it will have valid values
        Logging::getInstance()->log(
			"Warning, attempted updating units with diff IDs (expected=%d, received=%d). Ignoring...", 
			unitID, mUnit->getID()
		);
        return;
    }
	unitID = mUnit->getID();	//necessary when unitID was initialized with -1
	typeName = mUnit->getType().getName();
	x = mUnit->getPosition().x();
	y = mUnit->getPosition().y();
    //position = mUnit->getPosition();
    //tilePosition = mUnit->getTilePosition();
	lastSeenFrame = BWAPI::Broodwar->getFrameCount();
}

int SpottedObject::getUnitID() {
    return unitID;
}

BWAPI::UnitType SpottedObject::getType() {
	return BWAPI::UnitTypes::getUnitType(typeName);
}

int SpottedObject::getX(){
	return x;
}

int SpottedObject::getY(){
	return y;
}


int SpottedObject::getLastSeenFrame(){
	return lastSeenFrame;
}

void SpottedObject::draw(){

	BWAPI::UnitType myType = getType();
	/*
	Logging::getInstance()->log(
		"[%d] Drawing %s at %d,%d / tile: (%d,%d)", 
		BWAPI::Broodwar->getFrameCount(),
		myType.getName().c_str(), 
		x, y, x / 32, y / 32
	);
	*/
	int x1 = x - myType.dimensionLeft();
	int y1 = y - myType.dimensionUp();
	int x2 = x + myType.dimensionRight();// - myType.dimensionLeft();
	int y2 = y + myType.dimensionDown();// -  myType.dimensionUp();

	
	/*	
	Logging::getInstance()->log(
		"Rect coords: (%d,%d), (%d,%d)", x1, y1, x2, y2
	);
	*/

	BWAPI::Broodwar->drawBoxMap(x1, y1, x2, y2, BWAPI::Colors::Red, false);
	BWAPI::Broodwar->drawTextMap(
		x1, y,
		myType.c_str()
	);
}


