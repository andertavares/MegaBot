#include <ArmyManager.h>
using namespace BWAPI;
ArmyManager::ArmyManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator)
{
	this->arbitrator = arbitrator;
	lastFrameCheck = 0;
	lastExpandFrame = 0;
	firstAttack = true;
	
	d = true; //한번만 실행되게
	dd = true; // 한번만 실행되게
	ddd=  true;
	dddd = true;
	observer = true;
	//this->spendManager			 = new SpendManager();
}

void ArmyManager::setDefenseManager(DefenseManager* defenseManager)
{
	this->defenseManager = defenseManager;
}

void ArmyManager::setInformationManager(InformationManager* infoManager)
{
	this->informationManager = infoManager;
}

void ArmyManager::setBaseManager(BaseManager *baseManager)
{
	this->baseManager = baseManager;
}

double ArmyManager::enemyDPS()
{
  double total = 0;


	for each (BWAPI::Unit *u in SelectAllEnemy()(canMove)(isCompleted)(maxGroundHits,">",0).not(isWorker))
	{
		total += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
	}

	return total;
}

double ArmyManager::myDPS()
{
  double total = 0;

	for each (BWAPI::Unit *u in SelectAll()(canMove)(isCompleted)(maxGroundHits,">",0).not(isWorker))
	{
		total += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
	}

	return total;
}

//chc수정
void ArmyManager::update()
{
	//total_attack=false;

	BWAPI::Race race = Broodwar->self()->getRace();
	BWAPI::Race enemyRace = Broodwar->enemy()->getRace();

	if(enemyRace == Races::Protoss)  //적이 프로토스일경우
	{
	//chc
	/*

	//프로브테스트
		for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {		
		if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Probe)
        {
			
			std::set<BWAPI::Unit *>::const_iterator Probe = its;
			
			Position *e = new Position(2300,120);
			Position *em = new Position(2100,2100);
			if(d)
			{
			(*Probe)->rightClick(*em);
			d = false;
			}
			if((*Probe)->getPosition().x() < 3000 && (*Probe)->getPosition().x() > 1500 && (*Probe)->getPosition().y() < 3000 && (*Probe)->getPosition().y() > 1500)
				(*Probe)->rightClick(*e);		


		}
		}*/


		//다크컨트롤



///////////////////////////////////////////////////
	


//캐논+넥서스 1.4
			/*
			if(ddd && dddd)
			{
					 for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
    {		
	
		if ((*its2)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {

			std::set<BWAPI::Unit *>::const_iterator dark = its2;

				
				for (std::set<BWAPI::Unit *>::const_iterator n = Broodwar->enemy()->getUnits().begin(); n != BWAPI::Broodwar->enemy()->getUnits().end(); ++n)
			{
				//if((*n)->getType().isBuilding() ==  BWAPI::UnitTypes::Protoss_Photon_Cannon)
				if((*n)->getType() ==  BWAPI::UnitTypes::Protoss_Photon_Cannon)
				{

					if((*n)->getType().isBuilding())
					(*dark)->attack(*n);
				}  
				if((*n)->getType() == BWAPI::UnitTypes::Protoss_Nexus)
				{

		 for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
		 {		
	
		if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {
			std::set<BWAPI::Unit *>::const_iterator dark = its;

					(*dark)->attack(*n);
				
		}
			}
				}
				}
		}
				}
				dddd = false;	
			
			}
			*/






	}



	//chc
	

	if (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 12)
	{
		lastFrameCheck = BWAPI::Broodwar->getFrameCount();




		
	//테란일경우
	if(enemyRace == Races::Terran && false)  //임시로 막아놓음
	{
		if ( BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 3)
		{
					total_attack = true;
			
			


			if (!firstAttack)
			{
				firstAttack = true;
			}

			std::set<BWAPI::Unit*> idleDefenders = defenseManager->getIdleDefenders();
			//int count = 0;
			if (!idleDefenders.empty())
			{
				for (std::set<BWAPI::Unit*>::iterator it = idleDefenders.begin(); it != idleDefenders.end(); it++)
				{
					arbitrator->setBid(this, *it, 70);
					//++count;
				}
			}
		
   
		}


	}
	

		//if ((myDPS() > 4) || (myDPS() >= 1.8 && !firstAttack))

		//프로토스일경우
	if(observer) 
	{
		if(enemyRace == Races::Protoss && false)  //임시로 막아놓음
		{
			if ((myDPS() > 6.5) || (myDPS() >= 6.5 && !firstAttack))
		{

			total_attack = true;
		
			if (!firstAttack)
			{
				firstAttack = true;
			}

			std::set<BWAPI::Unit*> idleDefenders = defenseManager->getIdleDefenders();
			//int count = 0;
			if (!idleDefenders.empty())
			{
				for (std::set<BWAPI::Unit*>::iterator it = idleDefenders.begin(); it != idleDefenders.end(); it++)
				{
					arbitrator->setBid(this, *it, 70);
					//++count;
				}
			}
		}
		}
	}

	if(!observer) 
	{
		if(enemyRace == Races::Protoss && false) //임시로 막아놓음


		{
				if (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observer) >= 2)

		{
				total_attack = true;
			
				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
	{
		if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
		{
			(*its)->attack(this->informationManager->getEnemyStartLocation()->getPosition());
		}
				}


			if (!firstAttack)
			{
				firstAttack = true;
			}

			std::set<BWAPI::Unit*> idleDefenders = defenseManager->getIdleDefenders();
			//int count = 0;
			if (!idleDefenders.empty())
			{
				for (std::set<BWAPI::Unit*>::iterator it = idleDefenders.begin(); it != idleDefenders.end(); it++)
				{
					arbitrator->setBid(this, *it, 70);
					//++count;
				}
			}
		}
		}
	}








		//저그일경우

			if(enemyRace == Races::Zerg && false)  //임시로 막아놓음
			{
			if ((myDPS() > 3.94) || (myDPS() >= 3.94 && !firstAttack))

		{

				//total_attack = true;
	
			if (!firstAttack)
			{
				firstAttack = true;
			}

			std::set<BWAPI::Unit*> idleDefenders = defenseManager->getIdleDefenders();
			//int count = 0;
			if (!idleDefenders.empty())
			{
				for (std::set<BWAPI::Unit*>::iterator it = idleDefenders.begin(); it != idleDefenders.end(); it++)
				{
					arbitrator->setBid(this, *it, 70);
					//++count;
				}
			}
		}
			}

		std::map<BWAPI::Unit*, InformationManager::UnitData> savedData = informationManager->getSavedData();
		enemyBuildings.clear();
		for (std::map<BWAPI::Unit*, InformationManager::UnitData>::const_iterator sd = savedData.begin(); sd != savedData.end(); sd++)
		{
			if ((sd->second.exists) && (sd->second.player == BWAPI::Broodwar->enemy()) && (sd->second.type.isBuilding()) && (!sd->second.type.isInvincible()))
			{
				enemyBuildings.push_back(sd->second.position);
			}
		}
		round = (int)enemyBuildings.size() - 1;

		//for each (BWAPI::Unit *u in company)
		for(std::map<BWAPI::Unit*,ArmyManager::ArmyData>::iterator it = company.begin(); it != company.end(); it++)
		{
			if (it->first->isIdle())
			{
				it->second.mode = ArmyManager::ArmyData::Idle;
			}
			if (it->second.mode == ArmyManager::ArmyData::Idle)
			{
				BWAPI::Position pos = informationManager->getEnemyStartLocation()->getPosition(); // Enemy start location by default

				if (!informationManager->getEnemyBases().empty())
				{
					BWTA::BaseLocation *bl = *(informationManager->getEnemyBases().rbegin());
					it->second.target = bl;
					pos = it->second.target->getPosition(); 

					//chc
					/*if ((BWAPI::Broodwar->getFrameCount() > lastExpandFrame + 2000) && (myDPS() > 5))
					{
						lastExpandFrame = BWAPI::Broodwar->getFrameCount();
						baseManager->expand(90);
					}*/
				}
				else
				{
					
					pos = enemyBuildings[round];
					--round;
					if (round < 0)
						round = (int)enemyBuildings.size() - 1;
				}

				it->first->attack(pos);
				it->second.mode = ArmyManager::ArmyData::Attacking;
			}

			//std::set<BWAPI::Unit*> aroundMe = it->first->getUnitsInRadius(it->first->getType().sightRange()); //u->getUnitsInWeaponRange(u->getType().groundWeapon());
			//for each (BWAPI::Unit *am in aroundMe)
			//{
				/* attack workers IF there is no danger!!! */
				//if (am->getType().isWorker() && am->getPlayer() == BWAPI::Broodwar->enemy())
				//it->first->attack(am);
			//}
		}
	}
}

std::string ArmyManager::getName() const
{
	return "Army Manager";
}

std::string ArmyManager::getShortName() const
{
	return "Arm";
}

void ArmyManager::onOffer(std::set<BWAPI::Unit*> units)
{
	for each (BWAPI::Unit *u in units)
	{
		if (company.find(u) == company.end())
		{
			arbitrator->accept(this, u);
			ArmyManager::ArmyData	ad;
			ad.mode = ArmyManager::ArmyData::Idle;
			company[u] = ad;
		}
	}
}

void ArmyManager::onRevoke(BWAPI::Unit *u, double bid)
{
	onRemoveUnit(u);
}

void ArmyManager::onRemoveUnit(BWAPI::Unit* u)
{
	company.erase(u);
}
