#pragma once;

#include <Common.h>
#include "MicroManager.h"

class MicroManager;

class TransportManager : public MicroManager
{

protected:
	void TransportManager::loadTransportState();
	//BWAPI::Unit * TransportManager::getClosestTransport(BWAPI::Position loadPosition);
	void TransportManager::getClosestTransport(BWAPI::Position loadPosition);
	BOOL transportExists;
	BOOL techExists;

public:

	/*	Jinson notes: usually safer to have singleton constructors as private or protected, but somehow Squad.h is using it */
	TransportManager();
	~TransportManager() {}
	static TransportManager &	Instance();
	void TransportManager::sendUnits(std::set<BWAPI::Unit *> & unitsToLoad, BWAPI::Position loadPosition, BWAPI::Position unloadPosition);
	void executeMicro(const UnitVector & targets);
	void update();

};
