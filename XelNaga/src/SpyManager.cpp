#include <BWTA.h>
#include <SpyManager.h>
#include <UnitGroupManager.h>
BWTA::BaseLocation* findFarestBaseLocation(std::set<BWTA::BaseLocation *> allBaseLocations, const BWTA::BaseLocation *enemyStartLocation)
{
	BWTA::BaseLocation *toReturn;
	double maxDistance = -1;
	double distance;
	
	for each (BWTA::BaseLocation *bl in allBaseLocations)
	{
		distance = enemyStartLocation->getAirDistance(bl);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			toReturn = bl;
		}
	}
	return toReturn;
}

SpyManager::SpyManager(Arbitrator::Arbitrator<BWAPI::Unit*,double> *arbitrator)
{
	this->arbitrator = arbitrator;
	this->debugMode=false;
	lastFrameCheck = 0;
	randomDodge = new Random(21);
	initialized = false;
}

SpyManager::~SpyManager()
{
	delete randomDodge;
}

void SpyManager::onOffer(std::set<BWAPI::Unit*> units)
{
	for each (BWAPI::Unit *unit in units)
	{
		if (spies.find(unit) == spies.end())
		{
			arbitrator->accept(this, unit);
			SpyData temp;
			spies.insert(std::make_pair(unit,temp));
		}
		else
			arbitrator->decline(this, unit, 0);
	}
}

void SpyManager::onRevoke(BWAPI::Unit *unit, double bid)
{
	onRemoveUnit(unit);
}

void SpyManager::buildObserver()
{
	//if we don't have any observatories, first build one - and quickly!
	/* chc 옵저버 만들기 지움
	if (BWAPI::Broodwar->self()->visibleUnitCount(BWAPI::UnitTypes::Protoss_Observatory) == 0)
		buildOrderManager->build(1, BWAPI::UnitTypes::Protoss_Observatory, 75);

	//build an observer
	int observerCount = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observer);
	buildOrderManager->build(observerCount+1, BWAPI::UnitTypes::Protoss_Observer, 75);
	*/
}

void SpyManager::update()
{
	/*if (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 6)
	{
		lastFrameCheck = BWAPI::Broodwar->getFrameCount();

		std::set<BWAPI::Unit*> w = SelectAll()(isCompleted)(Protoss_Observer);
		for each(BWAPI::Unit *u in w)
			arbitrator->setBid(this, u, 40);


		if (!baseLocationsToSpy.empty())
		{
			std::map<BWAPI::Unit*, SpyData>::iterator it = spies.begin();
			for ( ; it != spies.end(); it++)
			{
				if (it->second.mode == SpyData::Idle)
				{
					it->second.target = baseLocationsToSpy.top();
					it->second.mode = SpyData::Spying;
					it->second.lastFrameDodging = 0;
					baseLocationsSpied.insert(baseLocationsToSpy.top());
					baseLocationsToSpy.pop();
					break;
				}
				if (it->second.mode == SpyData::Spying)
				{
					if ((it->second.lastFrameDodging == 0) || (BWAPI::Broodwar->getFrameCount() > it->second.lastFrameDodging + 300))
					{
						it->second.lastFrameDodging = BWAPI::Broodwar->getFrameCount();
						it->first->move(it->second.target->getPosition());
					}
					// Look if there are detectors around us. If yes, dodge it!
					std::set<BWAPI::Unit*> surroundingUnits = it->first->getUnitsInRadius(it->first->getType().sightRange());
					UnitGroup surroundingUnitGroup = UnitGroup::getUnitGroup(surroundingUnits);
					surroundingUnitGroup = surroundingUnitGroup(BWAPI::Broodwar->enemy())(isDetector);
					if (!surroundingUnitGroup.empty())
					{
						int x = it->first->getPosition().x();
						int	y = it->first->getPosition().y();
						for each (BWAPI::Unit *u in surroundingUnitGroup)
						{
							x -= (u->getPosition().x() - it->first->getPosition().x())/2;
							y -= (u->getPosition().y() - it->first->getPosition().y())/2;						
						}

						x += (randomDodge->nextAnotherInt() - 10);
						y += (randomDodge->nextAnotherInt() - 10);

						BWAPI::Position pos(x,y);
						//it->first->move(pos.makeValid()); chc 옵저버 베이스로 이동하는거 지움.
					}
				}
			}

			if (it == spies.end())
				if (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Protoss_Observer) == 0)
					buildObserver();
		}

		if (debugMode)
		{
			drawAssignments();
		}
	}*/
}

void SpyManager::setInformationManager(InformationManager* infoManager)
{
	this->informationManager = infoManager;
}

void SpyManager::setBuildOrderManager(BuildOrderManager *boManager)
{
	this->buildOrderManager = boManager;
}

std::string SpyManager::getName() const
{
	return "Spy Manager";
}

std::string SpyManager::getShortName() const
{
	return "Spy";
}

void SpyManager::onRemoveUnit(BWAPI::Unit* unit)
{
	if (spies.find(unit) != spies.end())
	{
		BWTA::BaseLocation* lostTarget = spies[unit].target;
		baseLocationsSpied.erase(lostTarget);

		if (baseLocationsSpied.find(lostTarget) == baseLocationsSpied.end())
		{
			baseLocationsToSpy.push(lostTarget);
			if (debugMode)
			{
				BWAPI::Broodwar->printf("Reassigning (%d,%d)", lostTarget->getPosition().x(), lostTarget->getPosition().y());
			}
		}
		spies.erase(unit);
	}
}

void SpyManager::setSpyCount(int count)
{
	this->numberBasesToSpy=count;
}

void SpyManager::setDebugMode(bool debugMode)
{
	this->debugMode = debugMode;
}

void SpyManager::initialize()
{
  //We spy all base locations but our starting one.
  numberBasesToSpy = BWTA::getBaseLocations().size() - 1;
  
  myStartLocation = BWTA::getStartLocation(BWAPI::Broodwar->self());
  //At this point, we should have only one base in infoManager::enemyLocations
	enemyStartLocation = *(informationManager->getEnemyBases().begin());

  std::set<BWTA::BaseLocation *> allBaseLocations = BWTA::getBaseLocations();
  allBaseLocations.erase(myStartLocation);
  
  while ((int)baseLocationsToSpy.size() < numberBasesToSpy)
    {
      BWTA::BaseLocation *farBase = findFarestBaseLocation(allBaseLocations, enemyStartLocation);
      allBaseLocations.erase(farBase);
      baseLocationsToSpy.push(farBase);
    }
  
  initialized = true;
}

bool SpyManager::isInitialized()
{
  return initialized;
}

void SpyManager::drawAssignments()
{
	//draw target vector for each Spy
	for (std::map<BWAPI::Unit*,SpyData>::iterator s = spies.begin(); s != spies.end(); s++)
	{
		if ((*s).second.mode != SpyData::Idle)
		{
			BWAPI::Position SpyPos = (*s).first->getPosition();
			BWAPI::Position targetPos = (*s).second.target->getPosition();
			BWAPI::Broodwar->drawLineMap(SpyPos.x(), SpyPos.y(), targetPos.x(), targetPos.y(), BWAPI::Colors::Yellow);
			BWAPI::Broodwar->drawCircleMap(SpyPos.x(), SpyPos.y(), 6, BWAPI::Colors::Yellow);
			BWAPI::Broodwar->drawCircleMap(targetPos.x(), targetPos.y(), (*s).first->getType().sightRange(), BWAPI::Colors::Yellow);
		}
	}
}
