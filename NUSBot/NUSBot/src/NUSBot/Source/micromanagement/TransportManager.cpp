/*	Transport Manager
 *
 *	Transport manager handles the building of transport units,
 *	loading, movement to dropoff location, and unloading.
 *
 *	Author: Jinson Xu
 *	Date: 30th July 2014
 */


#include "Common.h"
#include "TransportManager.h"



/*	Pseudo Code - Overview of BASIC implementation first
 *	1. IPBManager will decide which game phase is suitable for drops and then initialize TransportManager
 *	
 *	2. TransportManager will check environment for all transport related info and commence required building if no transport is available. 
 *
 *	3. IPBManager will invoke public function TransportManager::sendUnits(std::set<BWAPI::Unit *> & unitsToLoad, BWAPI::Position loadPosition, BWAPI::Position unloadPosition)
 *
 *	4. TransportManager will direct closest transport unit to loadPosition, load the units, move transport to unloadPosition, unload units, move transport back to loadPosition.
 */


// constructor
TransportManager::TransportManager()  { 
	//nothing to initialize in protected constructor for now
}


//get instance of the singleton
TransportManager & TransportManager::Instance() {
	static TransportManager instance;
	return instance;
}



void TransportManager::loadTransportState() {

}

void TransportManager::update() {

 if (  BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Shuttle ) == 1 )
 {
	 //train shuttle
	 BWAPI::Broodwar->drawTextScreen(250, 350, "NUSBot: Training Shuttle now.");
	
 
 }

}


//BWAPI::Unit * TransportManager::getClosestTransport(BWAPI::Position loadPosition) {
void TransportManager::getClosestTransport(BWAPI::Position loadPosition) {
   
}



void TransportManager::sendUnits(std::set<BWAPI::Unit *> & unitsToLoad, BWAPI::Position loadPosition, BWAPI::Position unloadPosition) {

}


void TransportManager::executeMicro(const UnitVector & targets) 
{
	const UnitVector & transportUnits = getUnits();

	BWAPI::Broodwar->drawTextScreen(250,350, "NUSBot Transport on the move!");
	if (transportUnits.empty())
	{
		return;
	}
}