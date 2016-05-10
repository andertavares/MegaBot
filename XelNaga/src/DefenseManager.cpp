#include <DefenseManager.h>
#include <BorderManager.h>
using namespace BWAPI;
DefenseManager::DefenseManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator)
{
	firsss2 = false; //chc
	very_early_rush = false;
	early_attack_assimi = true;
	very_early_attack_gate = true;
		early_attack_gate = true;
		early_rush =false;
		firsss=false;

	lastFrameCheck = 0;
	this->arbitrator = arbitrator;
}
void DefenseManager::setBorderManager(BorderManager* borderManager)
{
	this->borderManager=borderManager;
}

void DefenseManager::setBaseManager(BaseManager *baseManager)
{
	this->baseManager = baseManager;
}

void DefenseManager::setInformationManager(InformationManager *informationManager)
{
	this->informationManager = informationManager;
}

std::set<BWAPI::Unit*>& DefenseManager::getIdleDefenders()
{
	idleDefenders.clear();

	for (std::map<BWAPI::Unit*, DefenseData>::iterator it = defenders.begin(); it != defenders.end(); it++)
	{
		if ((it->second.mode == DefenseData::Idle) || (it->second.mode == DefenseData::Moving))
			idleDefenders.insert(it->first);
	}

	return idleDefenders;
}

void DefenseManager::onOffer(std::set<BWAPI::Unit*> units)
{
	for(std::set<BWAPI::Unit*>::iterator u = units.begin(); u != units.end(); u++)
	{
		if (defenders.find(*u) == defenders.end())
		{
			arbitrator->accept(this, *u);
			DefenseData temp;
			defenders.insert(std::make_pair(*u, temp));
		}
	}
}

void DefenseManager::onRevoke(BWAPI::Unit* unit, double bid)
{
	defenders.erase(unit);
}

void DefenseManager::onRemoveUnit(BWAPI::Unit* unit)
{
	defenders.erase(unit);
}

void DefenseManager::update()
{


	//chc
					int enemy_assimilator = 0;
					int	enemy_gateway = 0;
					for (std::set<BWAPI::Unit *>::const_iterator en = Broodwar->enemy()->getUnits().begin(); en != BWAPI::Broodwar->enemy()->getUnits().end(); ++en)
					{
						if((*en)->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
							enemy_assimilator++;

						if((*en)->getType() == BWAPI::UnitTypes::Protoss_Gateway)
							enemy_gateway++;
					}

					if(enemy_assimilator == 0)
						early_attack_assimi = false;

					if(enemy_gateway >= 2)
						early_attack_gate = false;

					if(enemy_gateway >=3)
						very_early_attack_gate = false;



					if(!very_early_rush && !firsss2)
				{
					if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core) == 1)
					{
						if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Cybernetics_Core) != 1) // 사이버 네스틱코어가 지어지고 있는 도중이 아니라면
							firsss2 = true; //더 이상 이 if문으로 못들어옴.
						if(!very_early_attack_gate && early_attack_assimi == false)
							very_early_rush = true;		
					}
				}





	if ((lastFrameCheck == 0) || (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 10))
	{
		lastFrameCheck = BWAPI::Broodwar->getFrameCount();

		// Bid on all completed military units
		std::set<BWAPI::Unit*> myPlayerUnits=BWAPI::Broodwar->self()->getUnits();
		for (std::set<BWAPI::Unit*>::iterator u = myPlayerUnits.begin(); u != myPlayerUnits.end(); u++)
		{
			if ((*u)->isCompleted() && 
				!(*u)->getType().isWorker() && 
				!(*u)->getType().isBuilding() &&
				(*u)->getType().canAttack() &&
				(*u)->getType() != BWAPI::UnitTypes::Zerg_Egg &&
				(*u)->getType() != BWAPI::UnitTypes::Zerg_Larva)
			{
				arbitrator->setBid(this, *u, 20);
			}
		}
		bool borderUpdated=false;
		if (myBorder!=borderManager->getMyBorder())
		{
			myBorder=borderManager->getMyBorder();
			borderUpdated=true;
		}

		//search among our bases which ones are in a border region
		std::vector<Base*> borderBases;
		for each (Base *base in baseManager->getAllBases())
		{
			bool isBorderBase = false;

			for each (BWTA::Chokepoint *c in base->getBaseLocation()->getRegion()->getChokepoints())
			{
				if (myBorder.find(c) != myBorder.end())
				{
					isBorderBase = true;
					break;
				}
			}
	

			if (isBorderBase)
				borderBases.push_back(base);
		}

		std::set<BWTA::BaseLocation*> enemyBasesLocation = informationManager->getEnemyBases();
		myBorderVector.clear();

		for(int i=0; i < (int)borderBases.size(); i++)
		{
			//search the nearest enemy base from current borderBases[i]
			BWTA::BaseLocation *nearestEnemyBaseLocation;
			double shortestDistance = 9999999;

			for each (BWTA::BaseLocation *bl in enemyBasesLocation)
			{
				double distance = borderBases[i]->getBaseLocation()->getGroundDistance(bl);
				if (distance < shortestDistance)
				{
					shortestDistance = distance;
					nearestEnemyBaseLocation = bl;
				}
			}

			if (nearestEnemyBaseLocation == NULL)
				nearestEnemyBaseLocation = informationManager->getEnemyStartLocation();

			//now search the nearest choke of borderBases[i] from the enemy base
			BWTA::Chokepoint *bestChoke;
			shortestDistance = 9999999;

			for each(BWTA::Chokepoint *c in borderBases[i]->getBaseLocation()->getRegion()->getChokepoints())
			{
				double distance = nearestEnemyBaseLocation->getPosition().getApproxDistance(c->getCenter());
				if (distance < shortestDistance)
				{
					shortestDistance = distance;
					bestChoke = c;
				}
			}
			myBorderVector.push_back(bestChoke);
		}

		//Order all units to choke
/*
		
  int i=0;
  if (!myBorder.empty())
  {
    for (std::map<BWAPI::Unit*,DefenseData>::iterator u = defenders.begin(); u != defenders.end(); u++)
    {
      if ((*u).second.mode == DefenseData::Idle || borderUpdated)
      {
        BWAPI::Position chokePosition=myBorderVector[i]->getCenter();
        i++;
        if (i>=(int)myBorderVector.size())
          i=0;
        (*u).first->attack(chokePosition);
        (*u).second.mode = DefenseData::Moving;
      }
    }
  }

  */


		/*
//적 초크포인트테스트		
  if (enemyBorder!=borderManager->getEnemyBorder())
  {
   enemyBorder=borderManager->getEnemyBorder();
    enemyBorderVector.clear();
    for(std::set<BWTA::Chokepoint*>::iterator i=enemyBorder.begin();i!=enemyBorder.end();i++)
      enemyBorderVector.push_back(*i);
    //borderUpdated=true;
  }

  //Order all units to choke
  int i=0;
  if (!enemyBorder.empty())
  {
    for (std::map<BWAPI::Unit*,DefenseData>::iterator u = defenders.begin(); u != defenders.end(); u++)
    {
   
        BWAPI::Position chokePosition2=enemyBorderVector[i]->getCenter();
        i++;
        if (i>=(int)myBorderVector.size())
          i=0;
   //     (*u).first->attackMove(chokePosition);
    //    (*u).second.mode = DefenseData::Moving;
      }    }
*/
//////////////////

//프로토스이면


		if((Broodwar->enemy()->getRace() == Races::Protoss || Broodwar->enemy()->getRace() == Races::Zerg || Broodwar->enemy()->getRace() == Races::Terran) && very_early_rush == false) //chc수정  
		{
		round = (int)myBorderVector.size() - 1;
		if (!myBorder.empty())
		{
			for (std::map<BWAPI::Unit*,DefenseData>::iterator u = defenders.begin(); u != defenders.end(); u++)
			{
				if (u->first->isIdle())
				{
					u->second.mode = DefenseData::Idle;
				}

				if (u->second.mode == DefenseData::Idle || borderUpdated)
				{
					BWAPI::Position chokePosition=myBorderVector[round]->getCenter();
					round--;
					if (round < 0)
						round = (int)myBorderVector.size() - 1;

					//wait to the 2/3 between your current position and the center of the choke to protect
					int borderBasesSize = (int)borderBases.size();
					BWAPI::Position lastBase = borderBases[borderBasesSize - round - 1]->getBaseLocation()->getPosition();					
					int x_wait = chokePosition.x() - lastBase.x();
					int y_wait = chokePosition.y() - lastBase.y();
					x_wait = (x_wait / 4) * 3;
					y_wait = (y_wait / 4) * 3;
					x_wait += lastBase.x();
					y_wait += lastBase.y();

					BWAPI::Position waitPosition(x_wait, y_wait); 

					(*u).first->attack(waitPosition);
					(*u).second.mode = DefenseData::Moving;
				}
				if (u->first->isUnderAttack())
				{
					std::set<BWAPI::Unit *> backup = this->getIdleDefenders();
					for each (BWAPI::Unit *bck in backup)
					{
						bck->attack(u->first->getPosition());
						defenders[bck].mode = DefenseData::Defending;
					}
				}
			}
		}
		}



	//	round = (int)myBorderVector.size() - 1;
		else
		{
		int i=0;
		if (!myBorder.empty())
		{
			for (std::map<BWAPI::Unit*,DefenseData>::iterator u = defenders.begin(); u != defenders.end(); u++)
			{
				if (u->first->isIdle())
				{
					u->second.mode = DefenseData::Idle;
				}

				if (u->second.mode == DefenseData::Idle || borderUpdated)
				{
					BWAPI::Position chokePosition=myBorderVector[i]->getCenter();
					i++;
				 if (i>=(int)myBorderVector.size())
                    i=0;

					(*u).first->attack(chokePosition);
					(*u).second.mode = DefenseData::Moving;
				}
				if (u->first->isUnderAttack())
				{
					std::set<BWAPI::Unit *> backup = this->getIdleDefenders();
					for each (BWAPI::Unit *bck in backup)
					{
						bck->attack(u->first->getPosition());
						defenders[bck].mode = DefenseData::Defending;
					}
				}
			}
		}
		

		}	
	}
}

std::string DefenseManager::getName() const
{
	return "Defense Manager";
}

std::string DefenseManager::getShortName() const
{
	return "Def";
}