#include "Common.h"
#include "IPBManager.h"

using namespace NUSBot;


IPBManager::IPBManager() : numWorkerScouts(0), currentScout(NULL)
{
	

}

void IPBManager::update()
{
	timerManager.startTimer(TimerManager::All);
	
	BWAPI::Broodwar->drawTextMap(200,330, "IPB_MANAGER_UPDATE" );

	//IPB 1st Stage - Describe Battlefiled
	describeBattleField();
	
	//Build Threat Model here based on Own units/buildings against Enemy Units and buildings
	enableThreatModel();

	developCourseOfAction();
	
		
	timerManager.stopTimer(TimerManager::All);

	drawDebugInterface();

	
}


void IPBManager::drawDebugInterface()
{
	timerManager.displayTimers(490, 225);
	
	StarcraftBuildOrderSearchManager::Instance().drawSearchInformation(10, 240);
	//BuildingManager::Instance().drawBuildingInformation(200,50);
	ProductionManager::Instance().drawProductionInformation(30, 50);
	InformationManager::Instance().drawUnitInformation(450,25);

	combatCommander.drawSquadInformation(200, 30);

	// draw position of mouse cursor
	if (Options::Debug::DRAW_NUSBOT_DEBUG)
	{
		BWAPI::Broodwar->drawTextScreen(20, 20, "Frame: %7d\nTime: %4dm %3ds", BWAPI::Broodwar->getFrameCount(), BWAPI::Broodwar->getFrameCount()/(24*60), (BWAPI::Broodwar->getFrameCount()/24)%60);
		int mouseX = BWAPI::Broodwar->getMousePosition().x() + BWAPI::Broodwar->getScreenPosition().x();
		int mouseY = BWAPI::Broodwar->getMousePosition().y() + BWAPI::Broodwar->getScreenPosition().y();
		BWAPI::Broodwar->drawTextMap(mouseX + 20, mouseY, " %d %d", mouseX, mouseY);
	}
}

// assigns units to various managers
void IPBManager::populateUnitVectors()
{
	// filter our units for those which are valid and usable
	setValidUnits();

	// set each type of unit
	setScoutUnits();
	setCombatUnits();
	//FLIWA
	setTransporUnits();
	setWorkerUnits();
}

const bool IPBManager::isAssigned(BWAPI::Unit * unit) const
{
	return assignedUnits.find(unit) != assignedUnits.end();
}

// validates units as usable for distribution to various managers
void IPBManager::setValidUnits()
{
	validUnits.clear();
	assignedUnits.clear();

	// make sure the unit is completed and alive and usable
	BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits())
	{
		if (isValidUnit(unit))
		{	
			validUnits.insert(unit);
		}
	}
}

// selects which units will be scouting
// currently only selects the worker scout after first pylon built
// this does NOT take that worker away from worker manager, but it still works
// TODO: take this worker away from worker manager in a clever way
void IPBManager::setScoutUnits()
{
	// if we have just built our first suply provider, set the worker to a scout
	if (numWorkerScouts == 0)
	{
		// get the first supply provider we come across in our units, this should be the first one we make
		BWAPI::Unit * supplyProvider = getFirstSupplyProvider();

		// if it exists
		if (supplyProvider)
		{
			// grab the closest worker to the supply provider to send to scout
			BWAPI::Unit * workerScout = getClosestWorkerToTarget(supplyProvider->getPosition());

			// if we find a worker (which we should) add it to the scout vector
			if (workerScout)
			{
				numWorkerScouts++;
				scoutUnits.insert(workerScout);
				assignedUnits.insert(workerScout);
				WorkerManager::Instance().setScoutWorker(workerScout);
			}
		}
	}
}

// sets combat units to be passed to CombatCommander
void IPBManager::setCombatUnits()
{
	combatUnits.clear();

	BOOST_FOREACH (BWAPI::Unit * unit, validUnits)
	{
		if (!isAssigned(unit) && isCombatUnit(unit))		
		{	
			combatUnits.insert(unit);
			assignedUnits.insert(unit);
		}
	}

	// emergency situation, enemy is in our base and we have no combat units
	// add our workers to the combat force
    int workersToDefend = StrategyManager::Instance().defendWithWorkers();
	if (combatUnits.empty() && (workersToDefend > 0))
	{
		BOOST_FOREACH (BWAPI::Unit * unit, validUnits)
		{
			// if it's a worker
			if (!isAssigned(unit) && unit->getType().isWorker())			
			{	
				// only assign it to combat if it's not building something
				if (!WorkerManager::Instance().isBuilder(unit))
				{
					combatUnits.insert(unit);
					assignedUnits.insert(unit);
                    workersToDefend--;
				}

                if (workersToDefend <= 0)
                {
                    break;
                }
			}
		}
	}
}

bool IPBManager::isCombatUnit(BWAPI::Unit * unit) const
{
	assert(unit != NULL);

	// no workers or buildings allowed
	if (unit && unit->getType().isWorker() || unit->getType().isBuilding())
	{
		return false;
	}

	// check for various types of combat units
	if (unit->getType().canAttack() || 
		unit->getType() == BWAPI::UnitTypes::Terran_Medic ||
		unit->getType() == BWAPI::UnitTypes::Protoss_High_Templar ||
		unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
	{
		return true;
	}
		
	return false;
}


bool IPBManager::isTransportUnit(BWAPI::Unit * unit) const
{
	assert(unit != NULL);

	// no workers or buildings allowed
	if (unit && unit->getType().isWorker() || unit->getType().isBuilding())
	{
		return false;
	}


	// check for various types of transport units
	if (unit->getType().canAttack() || unit->getType().canMove() ||
		unit->getType() == BWAPI::UnitTypes::Protoss_Shuttle ||
		unit->getType() == BWAPI::UnitTypes::Terran_Dropship ||
		unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
	{
		return true;
	}
		
	return false;
}

// sets Transport Unit
void IPBManager::setTransporUnits()
{
	transportUnits.clear();

	BOOST_FOREACH (BWAPI::Unit * unit, validUnits)
	{
		if (!isAssigned(unit) && isTransportUnit(unit))		
		{	
			transportUnits.insert(unit);
			assignedUnits.insert(unit);
		}
	}

}

BWAPI::Unit * IPBManager::getFirstSupplyProvider()
{
	BWAPI::Unit * supplyProvider = NULL;

	if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg)
	{
		BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == BWAPI::UnitTypes::Zerg_Spawning_Pool)
			{
				supplyProvider = unit;
			}
		}
	}
	else
	{
		
		BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == BWAPI::Broodwar->self()->getRace().getSupplyProvider())
			{
				supplyProvider = unit;
			}
		}
	}

	return supplyProvider;
}

void IPBManager::setWorkerUnits()
{
	workerUnits.clear();

	BOOST_FOREACH (BWAPI::Unit * unit, validUnits)
	{
		if (!isAssigned(unit) && unit->getType().isWorker())			
		{	
			workerUnits.insert(unit);
			assignedUnits.insert(unit);
		}
	}

}

bool IPBManager::isValidUnit(BWAPI::Unit * unit)
{
	if (!unit)
	{
		return false;
	}

	if (unit->isCompleted() 
		&& unit->getHitPoints() > 0 
		&& unit->exists() 
		&& unit->getType() != BWAPI::UnitTypes::Unknown 
		&& unit->getPosition().x() != BWAPI::Positions::Unknown.x() 
		&& unit->getPosition().y() != BWAPI::Positions::Unknown.y()) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

void IPBManager::onUnitShow(BWAPI::Unit * unit)			
{ 
	InformationManager::Instance().onUnitShow(unit); 
	WorkerManager::Instance().onUnitShow(unit);
}

void IPBManager::onUnitHide(BWAPI::Unit * unit)			
{ 
	InformationManager::Instance().onUnitHide(unit); 
}

void IPBManager::onUnitCreate(BWAPI::Unit * unit)		
{ 
	InformationManager::Instance().onUnitCreate(unit); 
}

void IPBManager::onUnitRenegade(BWAPI::Unit * unit)		
{ 
	InformationManager::Instance().onUnitRenegade(unit); 
}

void IPBManager::onUnitDestroy(BWAPI::Unit * unit)		
{ 	
	ProductionManager::Instance().onUnitDestroy(unit);
	WorkerManager::Instance().onUnitDestroy(unit);
	InformationManager::Instance().onUnitDestroy(unit); 
}

void IPBManager::onUnitMorph(BWAPI::Unit * unit)		
{ 
	InformationManager::Instance().onUnitMorph(unit);
	WorkerManager::Instance().onUnitMorph(unit);
}

BWAPI::Unit * IPBManager::getClosestUnitToTarget(BWAPI::UnitType type, BWAPI::Position target)
{
	BWAPI::Unit * closestUnit = NULL;
	double closestDist = 100000;

	BOOST_FOREACH (BWAPI::Unit * unit, validUnits)
	{
		if (unit->getType() == type)
		{
			double dist = unit->getDistance(target);
			if (!closestUnit || dist < closestDist)
			{
				closestUnit = unit;
				closestDist = dist;
			}
		}
	}

	return closestUnit;
}

BWAPI::Unit * IPBManager::getClosestWorkerToTarget(BWAPI::Position target)
{
	BWAPI::Unit * closestUnit = NULL;
	double closestDist = 100000;

	BOOST_FOREACH (BWAPI::Unit * unit, validUnits)
	{
		if (!isAssigned(unit) && unit->getType().isWorker() && WorkerManager::Instance().isFree(unit))
		{
			double dist = unit->getDistance(target);
			if (!closestUnit || dist < closestDist)
			{
				closestUnit = unit;
				closestDist = dist;
			}
		}
	}

	return closestUnit;
}

void IPBManager::setGamePhase(GAME_PHASE phase )			
{ 
	curr_phase = phase;
	next_phase == curr_phase + 1;

	
}

void IPBManager::getMapDetails()			
{ 
	BWTA::readMap();
	BWTA::analyze();

	//We need to know how big the map is and then 
	//compute from the size when will we be the time
	//that we will consider the middle game phase
	//We can then create strategy base on this data
}

//TODO: BattlefieldManager
void IPBManager::describeBattleField()			
{ 
	BWAPI::Broodwar->drawTextMap(250,350, "IPB_MANAGER: Scanning Battlefield..." );
	timerManager.startTimer(TimerManager::MapGrid);

	//TODO: We should be using the Layered Influence Map here
	MapGrid::Instance().update();

	timerManager.stopTimer(TimerManager::MapGrid);

	timerManager.startTimer(TimerManager::MapTools);
	MapTools::Instance().update();					//expansion searching
	timerManager.stopTimer(TimerManager::MapTools);	
}

//TODO: ThreatManager
void IPBManager::enableThreatModel()			
{ 
	BWAPI::Broodwar->drawTextMap(250,350, "IPB_MANAGER: Threat Detection..." );
	// economy and base managers
	timerManager.startTimer(TimerManager::Worker);
	// populate the unit vectors we will pass into various managers
	populateUnitVectors();
	WorkerManager::Instance().update();
	timerManager.stopTimer(TimerManager::Worker);

	timerManager.startTimer(TimerManager::Production);
	ProductionManager::Instance().update();
	timerManager.stopTimer(TimerManager::Production);

	
	
}
//TODO: COAManager
void IPBManager::developCourseOfAction()			
{ 
	
    BWAPI::Broodwar->drawTextMap(250,350, "IPB_MANAGER: Resolving COA...\n" );
	timerManager.startTimer(TimerManager::Building);
	BuildingManager::Instance().update();
	timerManager.stopTimer(TimerManager::Building);

	// combat and scouting managers
	timerManager.startTimer(TimerManager::Combat);
	if (Options::Modules::USING_COMBATCOMMANDER)
	{
		combatCommander.update(combatUnits);
	}
	timerManager.stopTimer(TimerManager::Combat);

	timerManager.startTimer(TimerManager::Scout);
	if (Options::Modules::USING_SCOUTMANAGER)
	{
		scoutManager.update(scoutUnits);
	}
	timerManager.stopTimer(TimerManager::Scout);

	// utility managers
	timerManager.startTimer(TimerManager::InformationManager);
	InformationManager::Instance().update();
	timerManager.stopTimer(TimerManager::InformationManager);

	timerManager.startTimer(TimerManager::MapGrid);
	MapGrid::Instance().update();
	timerManager.stopTimer(TimerManager::MapGrid);

	timerManager.startTimer(TimerManager::MapTools);
	MapTools::Instance().update();
	timerManager.stopTimer(TimerManager::MapTools);

	timerManager.startTimer(TimerManager::Search);
	StarcraftBuildOrderSearchManager::Instance().update(35 - timerManager.getTotalElapsed());
	timerManager.stopTimer(TimerManager::Search);
	
}
