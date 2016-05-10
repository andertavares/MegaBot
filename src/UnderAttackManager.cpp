#include <UnderAttackManager.h>
#include <UnitGroup.h>

UnderAttackManager::UnderAttackManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator)
{
	this->arbitrator = arbitrator;
	lastFrameCheck = 0;
}

void UnderAttackManager::update()
{
	if (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 20)
	{
		lastFrameCheck = BWAPI::Broodwar->getFrameCount();

		// Spot and free idle backups and workers purchasing enemy
		for (std::map<BWAPI::Unit*, UAData>::iterator it = backUp.begin(); it != backUp.end(); ++it)
		{
			if ( it->first->isIdle() || 
				(it->second.mode == UAData::WorkerDefending && it->first->getTarget() != NULL && !it->first->getTarget()->isAttacking()) )
			{
				it->second.mode = UAData::Idle;
			}

			if (it->second.mode == UAData::Idle)
			{
				arbitrator->removeBid(this,it->first);
			}
		}

		UnitGroup myPrecious = SelectAll()(isBuilding) + SelectAll()(isWorker);
		for each (BWAPI::Unit *u in myPrecious)
		{
			if (u->isUnderAttack())
			{

				// If I am an under attacked worker, myCoWorkers are my buddy workers from the same base
				UnitGroup myCoWorkers;

				std::set<BWAPI::Unit*> idleDef = defenseManager->getIdleDefenders();
				for each (BWAPI::Unit *def in idleDef)
				{
					arbitrator->setBid(this, def, 50);
				}

				if (u->getType().isWorker())
				{
					std::set<BWAPI::Unit*> aroundMe = u->getUnitsInRadius(u->getType().sightRange());
					myCoWorkers = UnitGroup::getUnitGroup(aroundMe);
					myCoWorkers = myCoWorkers(BWAPI::Broodwar->self())(isWorker)(isCompleted);
					for each (BWAPI::Unit *def in myCoWorkers)
					{
						arbitrator->setBid(this, def, 60);
					}
				}

				if (u->isBeingConstructed() && u->getHitPoints() < 80)
				{
					u->cancelConstruction();
				}

				for (std::map<BWAPI::Unit*, UAData>::iterator it = backUp.begin(); it != backUp.end(); it++)
				{
					if (it->second.mode == UAData::Defender)
					{
						it->first->attack(u->getPosition());
						it->second.mode = UAData::Defending;
					}
					if (it->second.mode == UAData::Worker)
					{
						it->first->attack(u->getPosition());
						it->second.mode = UAData::WorkerDefending;
					}
				}
			}
		}

		// Anti gas steal (and anti in-base enemy building)
		UnitGroup intruders = SelectAllEnemy()(isBuilding);

		for each (BWAPI::Unit *u in intruders)
		{
			// if build u is placed inside one of our region, blast it!
			if (baseManager->getMyRegions().find(BWTA::getRegion(u->getTilePosition())) != baseManager->getMyRegions().end())
			{
				std::set<BWAPI::Unit*> idleDef = defenseManager->getIdleDefenders();
				for each (BWAPI::Unit *def in idleDef)
				{
					arbitrator->setBid(this, def, 50);
				}

				for (std::map<BWAPI::Unit*, UAData>::iterator it = backUp.begin(); it != backUp.end(); it++)
				{
					if (it->second.mode == UAData::Defender)
					{
						it->first->attack(u->getPosition());
						it->second.mode = UAData::Defending;
					}
				}								
			}
		}
	}		
}

void UnderAttackManager::setDefenseManager(DefenseManager *defenseManager)
{
	this->defenseManager = defenseManager;
}

void UnderAttackManager::setBaseManager(BaseManager *baseManager)
{
	this->baseManager = baseManager;
}

std::string UnderAttackManager::getName() const
{
	return "UnderAttack Manager";
}

std::string UnderAttackManager::getShortName() const
{
	return "UAM";
}

void UnderAttackManager::onOffer(std::set<BWAPI::Unit*> units)
{
	for each (BWAPI::Unit *u in units)
	{
		if (backUp.find(u) == backUp.end())
		{
			arbitrator->accept(this, u);
			
			UAData temp;
			if (arbitrator->getBid(this, u) == 50)
			{
				temp.mode = UAData::Defender;
			}
			else
			{
				temp.mode = UAData::Worker;
			}

			backUp.insert(std::make_pair(u, temp));
		}
	}
}

void UnderAttackManager::onRevoke(BWAPI::Unit *u, double bid)
{
	onRemoveUnit(u);
}

void UnderAttackManager::onRemoveUnit(BWAPI::Unit* u)
{
	backUp.erase(u);
}
