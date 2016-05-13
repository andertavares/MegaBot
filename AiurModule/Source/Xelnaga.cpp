#include "Xelnaga.h"
#include "../src/Util.h"
using namespace BWAPI;
using namespace XelnagaBot;

//int dd21 = 0;
//파일 입출력
#include <stdlib.h>  

void Xelnaga::onStart()
{

	Broodwar->setLocalSpeed(0);
	//setLocalSpeed(0);
	//BWAPI::Game::setLocalSpeed(0);
	//입력하기
	for(int i=0;i<1024;i++)
	{
		pre_word1[i] = 0;
		pre_word2[i] = 0;
		last_word[i] = 0;
	}
	strcpy(pre_word1,"bwapi-data\\read\\Xelnaga_");
	strcpy(pre_word2,"bwapi-data\\write\\Xelnaga_");
	strcpy(last_word,".txt");
	
	//char pre_word1[1024] = "bwapi-data\\read\\Xelnaga_";
	
	//char pre_word2[1024] = "bwapi-data\\write\\Xelnaga_";
	//char last_word[1024] = ".txt";

	strcat(pre_word1,Broodwar->enemy()->getName().c_str());
	strcat(pre_word1,last_word);
	
	strcat(pre_word2,Broodwar->enemy()->getName().c_str());
	strcat(pre_word2,last_word);
	

	

	
	fp1=fopen(pre_word1,"r");
	fp2=fopen(pre_word2,"r");

	lost_strategy_num=-1;
	
	if(fp1!=NULL)
	{
		fscanf(fp1,"%d",&lost_strategy_num);
		fclose(fp1);
	}
	else if(fp2!=NULL)
	{
		fscanf(fp2,"%d",&lost_strategy_num);
		fclose(fp2);
	}




	


	//프로토스인경우 전략 3개(빠른다크,확장,리버)
	if(Broodwar->enemy()->getRace() == Races::Protoss)
	{
		//지난번 진 전략+1번 전략을 택함.
		lost_strategy_num++;
		//3번 이후에는 다시 0번으로 순환
		if(lost_strategy_num >= 4)
			lost_strategy_num = 0;

//lost_strategy_num = 1;//없애주기!!
	
	
	if(lost_strategy_num == 0)
	{
		this->protoss_FD       = new Protoss_FD();
		this->protoss_FD->onStart();
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi       = new Protoss_Multi();
		this->protoss_Multi->onStart();		
	}
	
	else if(lost_strategy_num == 2)
		{
		this->protoss_ZR       = new Protoss_ZR();
		this->protoss_ZR->onStart();	
		}


	else if(lost_strategy_num == 3)
		{
		this->protoss_FZ       = new Protoss_FZ();
		this->protoss_FZ->onStart();	
		}

	
	}

	//테란인경우 전략 2개(빠른다크,확장) 
	if(Broodwar->enemy()->getRace() == Races::Terran)
	{
		//지난번 진 전략+1번 전략을 택함.
		lost_strategy_num++;
		//1번 이후에는 다시 0번으로 순환
		if(lost_strategy_num >= 2)
			lost_strategy_num = 0;
		
		
//lost_strategy_num = 1;//없애주기!!
	
	if(lost_strategy_num == 0)
	{
	this->protoss_FD       = new Protoss_FD();
	this->protoss_FD->onStart();

	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi       = new Protoss_Multi();
		this->protoss_Multi->onStart();		
	}
	
	/*
	else if(lost_strategy_num == 2)
		{
		this->terran_ZR_Drop       = new Terran_ZR_Drop();
		this->terran_ZR_Drop->onStart();
		}
	*/	

	
	}
	
	//저그인경우 1개
	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
		this->protoss_Multi       = new Protoss_Multi();
		this->protoss_Multi->onStart();	
	}
	


	
	


	
	return ;


}

Xelnaga::~Xelnaga()
{
	if(Broodwar->enemy()->getRace() == Races::Protoss)
	{

			
	if(lost_strategy_num == 0)
	{
	delete this->protoss_FD->buildManager;
	delete this->protoss_FD->techManager;
	delete this->protoss_FD->upgradeManager;
	delete this->protoss_FD->scoutManager;
	delete this->protoss_FD->spyManager;
	delete this->protoss_FD->workerManager;
	delete this->protoss_FD->supplyManager;
	delete this->protoss_FD->spendManager;
	delete this->protoss_FD->buildOrderManager;
	delete this->protoss_FD->baseManager;
	delete this->protoss_FD->defenseManager;
	delete this->protoss_FD->underAttackManager;
	delete this->protoss_FD->armyManager;
	delete this->protoss_FD->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_FD->enhancedUI;

	}
	else if(lost_strategy_num == 1)
	{

	delete this->protoss_Multi->buildManager;
	delete this->protoss_Multi->techManager;
	delete this->protoss_Multi->upgradeManager;
	delete this->protoss_Multi->scoutManager;
	delete this->protoss_Multi->spyManager;
	delete this->protoss_Multi->workerManager;
	delete this->protoss_Multi->supplyManager;
	delete this->protoss_Multi->spendManager;
	delete this->protoss_Multi->buildOrderManager;
	delete this->protoss_Multi->baseManager;
	delete this->protoss_Multi->defenseManager;
	delete this->protoss_Multi->underAttackManager;
	delete this->protoss_Multi->armyManager;
	delete this->protoss_Multi->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_Multi->enhancedUI;
	
	}
	
	else if(lost_strategy_num == 2)
		{
		delete this->protoss_ZR->buildManager;
	delete this->protoss_ZR->techManager;
	delete this->protoss_ZR->upgradeManager;
	delete this->protoss_ZR->scoutManager;
	delete this->protoss_ZR->spyManager;
	delete this->protoss_ZR->workerManager;
	delete this->protoss_ZR->supplyManager;
	delete this->protoss_ZR->spendManager;
	delete this->protoss_ZR->buildOrderManager;
	delete this->protoss_ZR->baseManager;
	delete this->protoss_ZR->defenseManager;
	delete this->protoss_ZR->underAttackManager;
	delete this->protoss_ZR->armyManager;
	delete this->protoss_ZR->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_ZR->enhancedUI;
		}


	else if(lost_strategy_num == 3)
		{
	delete this->protoss_FZ->buildManager;
	delete this->protoss_FZ->techManager;
	delete this->protoss_FZ->upgradeManager;
	delete this->protoss_FZ->scoutManager;
	delete this->protoss_FZ->spyManager;
	delete this->protoss_FZ->workerManager;
	delete this->protoss_FZ->supplyManager;
	delete this->protoss_FZ->spendManager;
	delete this->protoss_FZ->buildOrderManager;
	delete this->protoss_FZ->baseManager;
	delete this->protoss_FZ->defenseManager;
	delete this->protoss_FZ->underAttackManager;
	delete this->protoss_FZ->armyManager;
	delete this->protoss_FZ->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_FZ->enhancedUI;
	}
	


	}

	if(Broodwar->enemy()->getRace() == Races::Terran)
	{
		if(lost_strategy_num == 0)
	{
	delete this->protoss_FD->buildManager;
	delete this->protoss_FD->techManager;
	delete this->protoss_FD->upgradeManager;
	delete this->protoss_FD->scoutManager;
	delete this->protoss_FD->spyManager;
	delete this->protoss_FD->workerManager;
	delete this->protoss_FD->supplyManager;
	delete this->protoss_FD->spendManager;
	delete this->protoss_FD->buildOrderManager;
	delete this->protoss_FD->baseManager;
	delete this->protoss_FD->defenseManager;
	delete this->protoss_FD->underAttackManager;
	delete this->protoss_FD->armyManager;
	delete this->protoss_FD->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_FD->enhancedUI;

	}
	else if(lost_strategy_num == 1)
	{
			delete this->protoss_Multi->buildManager;
	delete this->protoss_Multi->techManager;
	delete this->protoss_Multi->upgradeManager;
	delete this->protoss_Multi->scoutManager;
	delete this->protoss_Multi->spyManager;
	delete this->protoss_Multi->workerManager;
	delete this->protoss_Multi->supplyManager;
	delete this->protoss_Multi->spendManager;
	delete this->protoss_Multi->buildOrderManager;
	delete this->protoss_Multi->baseManager;
	delete this->protoss_Multi->defenseManager;
	delete this->protoss_Multi->underAttackManager;
	delete this->protoss_Multi->armyManager;
	delete this->protoss_Multi->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_Multi->enhancedUI;
	}

	}

	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
		delete this->protoss_Multi->buildManager;
	delete this->protoss_Multi->techManager;
	delete this->protoss_Multi->upgradeManager;
	delete this->protoss_Multi->scoutManager;
	delete this->protoss_Multi->spyManager;
	delete this->protoss_Multi->workerManager;
	delete this->protoss_Multi->supplyManager;
	delete this->protoss_Multi->spendManager;
	delete this->protoss_Multi->buildOrderManager;
	delete this->protoss_Multi->baseManager;
	delete this->protoss_Multi->defenseManager;
	delete this->protoss_Multi->underAttackManager;
	delete this->protoss_Multi->armyManager;
	delete this->protoss_Multi->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->protoss_Multi->enhancedUI;
	}
	
	
}

void Xelnaga::onEnd(bool isWinner)
{
	
	
	//졌다면 진 전략 기록
	if(isWinner == false)
	{		
		fp3=fopen(pre_word1,"w");
		fp4=fopen(pre_word2,"w");
		fprintf(fp3,"%d",lost_strategy_num);
		fprintf(fp4,"%d",lost_strategy_num);
		fclose(fp3);
		fclose(fp4);
	}
	
	
	//log("onEnd(%d)\n",isWinner); //이기는사람체크 Protoss_FD로 안해서 문제될수도?	
}




void Xelnaga::onFrame()
{
	
	
	
	
		if(Broodwar->mapName().compare(1,9,"Andromeda") == 0)
							{
								
								if(lost_strategy_num == 1 || Broodwar->enemy()->getRace() == Races::Zerg)
									this->protoss_Multi->aaab =true;							
							}
						


		Broodwar->drawTextScreen(10,10,"System Number (Stretegy) : %d",lost_strategy_num);


if(Broodwar->enemy()->getRace() == Races::Protoss)
{
	if(lost_strategy_num == 0)
	{
		this->protoss_FD->onFrame();
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->onFrame();
	}

	else if(lost_strategy_num == 2)
		{
			
				this->protoss_ZR->onFrame();
		}

	
	else if(lost_strategy_num == 3)
		{
			
				this->protoss_FZ->onFrame();
		}
}


if(Broodwar->enemy()->getRace() == Races::Terran)
{
	if(lost_strategy_num == 0)
	{
		this->protoss_FD->onFrame();
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->onFrame();
	}

	else if(lost_strategy_num == 2)
		{			
				this->terran_ZR_Drop->onFrame();
		}
}



	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{

		this->protoss_Multi->onFrame();
	}
	
	


	
	
	return ; 

}


void Xelnaga::onUnitDestroy(BWAPI::Unit* unit)
{	
	if (Broodwar->isReplay()) return;


	

if(Broodwar->enemy()->getRace() == Races::Protoss)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->arbitrator.onRemoveObject(unit);
	this->protoss_FD->buildManager->onRemoveUnit(unit);
	this->protoss_FD->techManager->onRemoveUnit(unit);
	this->protoss_FD->upgradeManager->onRemoveUnit(unit);
	this->protoss_FD->workerManager->onRemoveUnit(unit);  
	this->protoss_FD->scoutManager->onRemoveUnit(unit);
	this->protoss_FD->spyManager->onRemoveUnit(unit);
	this->protoss_FD->defenseManager->onRemoveUnit(unit);
	this->protoss_FD->informationManager->onUnitDestroy(unit);
	this->protoss_FD->baseManager->onRemoveUnit(unit);
	this->protoss_FD->underAttackManager->onRemoveUnit(unit);
	this->protoss_FD->armyManager->onRemoveUnit(unit);
	
	}
	else if(lost_strategy_num == 1)
	{

	this->protoss_Multi->arbitrator.onRemoveObject(unit);
	this->protoss_Multi->buildManager->onRemoveUnit(unit);
	this->protoss_Multi->techManager->onRemoveUnit(unit);
	this->protoss_Multi->upgradeManager->onRemoveUnit(unit);
	this->protoss_Multi->workerManager->onRemoveUnit(unit);  
	this->protoss_Multi->scoutManager->onRemoveUnit(unit);
	this->protoss_Multi->spyManager->onRemoveUnit(unit);
	this->protoss_Multi->defenseManager->onRemoveUnit(unit);
	this->protoss_Multi->informationManager->onUnitDestroy(unit);
	this->protoss_Multi->baseManager->onRemoveUnit(unit);
	this->protoss_Multi->underAttackManager->onRemoveUnit(unit);
	this->protoss_Multi->armyManager->onRemoveUnit(unit);




	}

	else if(lost_strategy_num == 2)
		{
			
					this->protoss_ZR->arbitrator.onRemoveObject(unit);
	this->protoss_ZR->buildManager->onRemoveUnit(unit);
	this->protoss_ZR->techManager->onRemoveUnit(unit);
	this->protoss_ZR->upgradeManager->onRemoveUnit(unit);
	this->protoss_ZR->workerManager->onRemoveUnit(unit);  
	this->protoss_ZR->scoutManager->onRemoveUnit(unit);
	this->protoss_ZR->spyManager->onRemoveUnit(unit);
	this->protoss_ZR->defenseManager->onRemoveUnit(unit);
	this->protoss_ZR->informationManager->onUnitDestroy(unit);
	this->protoss_ZR->baseManager->onRemoveUnit(unit);
	this->protoss_ZR->underAttackManager->onRemoveUnit(unit);
	this->protoss_ZR->armyManager->onRemoveUnit(unit);
		}


	else if(lost_strategy_num == 3)
		{			
	this->protoss_FZ->arbitrator.onRemoveObject(unit);
	this->protoss_FZ->buildManager->onRemoveUnit(unit);
	this->protoss_FZ->techManager->onRemoveUnit(unit);
	this->protoss_FZ->upgradeManager->onRemoveUnit(unit);
	this->protoss_FZ->workerManager->onRemoveUnit(unit);  
	this->protoss_FZ->scoutManager->onRemoveUnit(unit);
	this->protoss_FZ->spyManager->onRemoveUnit(unit);
	this->protoss_FZ->defenseManager->onRemoveUnit(unit);
	this->protoss_FZ->informationManager->onUnitDestroy(unit);
	this->protoss_FZ->baseManager->onRemoveUnit(unit);
	this->protoss_FZ->underAttackManager->onRemoveUnit(unit);
	this->protoss_FZ->armyManager->onRemoveUnit(unit);
		}
}


if(Broodwar->enemy()->getRace() == Races::Terran)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->arbitrator.onRemoveObject(unit);
	this->protoss_FD->buildManager->onRemoveUnit(unit);
	this->protoss_FD->techManager->onRemoveUnit(unit);
	this->protoss_FD->upgradeManager->onRemoveUnit(unit);
	this->protoss_FD->workerManager->onRemoveUnit(unit);  
	this->protoss_FD->scoutManager->onRemoveUnit(unit);
	this->protoss_FD->spyManager->onRemoveUnit(unit);
	this->protoss_FD->defenseManager->onRemoveUnit(unit);
	this->protoss_FD->informationManager->onUnitDestroy(unit);
	this->protoss_FD->baseManager->onRemoveUnit(unit);
	this->protoss_FD->underAttackManager->onRemoveUnit(unit);
	this->protoss_FD->armyManager->onRemoveUnit(unit);
	}
	else if(lost_strategy_num == 1)
	{
			this->protoss_Multi->arbitrator.onRemoveObject(unit);
	this->protoss_Multi->buildManager->onRemoveUnit(unit);
	this->protoss_Multi->techManager->onRemoveUnit(unit);
	this->protoss_Multi->upgradeManager->onRemoveUnit(unit);
	this->protoss_Multi->workerManager->onRemoveUnit(unit);  
	this->protoss_Multi->scoutManager->onRemoveUnit(unit);
	this->protoss_Multi->spyManager->onRemoveUnit(unit);
	this->protoss_Multi->defenseManager->onRemoveUnit(unit);
	this->protoss_Multi->informationManager->onUnitDestroy(unit);
	this->protoss_Multi->baseManager->onRemoveUnit(unit);
	this->protoss_Multi->underAttackManager->onRemoveUnit(unit);
	this->protoss_Multi->armyManager->onRemoveUnit(unit);
	}

	else if(lost_strategy_num == 2)
		{			
					this->terran_ZR_Drop->arbitrator.onRemoveObject(unit);
	this->terran_ZR_Drop->buildManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->techManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->upgradeManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->workerManager->onRemoveUnit(unit);  
	this->terran_ZR_Drop->scoutManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->spyManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->defenseManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->informationManager->onUnitDestroy(unit);
	this->terran_ZR_Drop->baseManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->underAttackManager->onRemoveUnit(unit);
	this->terran_ZR_Drop->armyManager->onRemoveUnit(unit);
		}
}



	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
			this->protoss_Multi->arbitrator.onRemoveObject(unit);
	this->protoss_Multi->buildManager->onRemoveUnit(unit);
	this->protoss_Multi->techManager->onRemoveUnit(unit);
	this->protoss_Multi->upgradeManager->onRemoveUnit(unit);
	this->protoss_Multi->workerManager->onRemoveUnit(unit);  
	this->protoss_Multi->scoutManager->onRemoveUnit(unit);
	this->protoss_Multi->spyManager->onRemoveUnit(unit);
	this->protoss_Multi->defenseManager->onRemoveUnit(unit);
	this->protoss_Multi->informationManager->onUnitDestroy(unit);
	this->protoss_Multi->baseManager->onRemoveUnit(unit);
	this->protoss_Multi->underAttackManager->onRemoveUnit(unit);
	this->protoss_Multi->armyManager->onRemoveUnit(unit);
	}
	







	
}

void Xelnaga::onUnitDiscover(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	
	
	
if(Broodwar->enemy()->getRace() == Races::Protoss)
{
	if(lost_strategy_num == 0)
	{
		this->protoss_FD->informationManager->onUnitDiscover(unit);
	this->protoss_FD->unitGroupManager->onUnitDiscover(unit);
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->informationManager->onUnitDiscover(unit);
	this->protoss_Multi->unitGroupManager->onUnitDiscover(unit);
	}

	else if(lost_strategy_num == 2)
		{
			
					this->protoss_ZR->informationManager->onUnitDiscover(unit);
	this->protoss_ZR->unitGroupManager->onUnitDiscover(unit);
		}

	else if(lost_strategy_num == 3)
		{
			
	this->protoss_FZ->informationManager->onUnitDiscover(unit);
	this->protoss_FZ->unitGroupManager->onUnitDiscover(unit);
		}
}


if(Broodwar->enemy()->getRace() == Races::Terran)
{
	if(lost_strategy_num == 0)
	{
			this->protoss_FD->informationManager->onUnitDiscover(unit);
	this->protoss_FD->unitGroupManager->onUnitDiscover(unit);
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->informationManager->onUnitDiscover(unit);
	this->protoss_Multi->unitGroupManager->onUnitDiscover(unit);
	}

	else if(lost_strategy_num == 2)
		{			
				this->terran_ZR_Drop->informationManager->onUnitDiscover(unit);
			this->terran_ZR_Drop->unitGroupManager->onUnitDiscover(unit);
		}
}



	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
	this->protoss_Multi->informationManager->onUnitDiscover(unit);
	this->protoss_Multi->unitGroupManager->onUnitDiscover(unit);
	}
	
	
	
	
	
	
	
	
	
	
}
void Xelnaga::onUnitEvade(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;



	
if(Broodwar->enemy()->getRace() == Races::Protoss)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->informationManager->onUnitEvade(unit);
	this->protoss_FD->unitGroupManager->onUnitEvade(unit);
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->informationManager->onUnitEvade(unit);
	this->protoss_Multi->unitGroupManager->onUnitEvade(unit);
	}

	else if(lost_strategy_num == 2)
		{
			
					this->protoss_ZR->informationManager->onUnitEvade(unit);
	this->protoss_ZR->unitGroupManager->onUnitEvade(unit);
		}
	else if(lost_strategy_num ==3)
		{
			
					this->protoss_FZ->informationManager->onUnitEvade(unit);
	this->protoss_FZ->unitGroupManager->onUnitEvade(unit);
		}
}


if(Broodwar->enemy()->getRace() == Races::Terran)
{
	if(lost_strategy_num == 0)
	{
		this->protoss_FD->informationManager->onUnitEvade(unit);
	this->protoss_FD->unitGroupManager->onUnitEvade(unit);
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->informationManager->onUnitEvade(unit);
	this->protoss_Multi->unitGroupManager->onUnitEvade(unit);
	}

	else if(lost_strategy_num == 2)
		{			
				this->terran_ZR_Drop->informationManager->onUnitEvade(unit);
	this->terran_ZR_Drop->unitGroupManager->onUnitEvade(unit);
		}
}



	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
		this->protoss_Multi->informationManager->onUnitEvade(unit);
	this->protoss_Multi->unitGroupManager->onUnitEvade(unit);
	}
	
	




	
}

void Xelnaga::onUnitMorph(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	
	
	
	
if(Broodwar->enemy()->getRace() == Races::Protoss)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->unitGroupManager->onUnitMorph(unit);
	}
	else if(lost_strategy_num == 1)
	{
	this->protoss_Multi->unitGroupManager->onUnitMorph(unit);
	}

	else if(lost_strategy_num == 2)
		{
			
				this->protoss_ZR->unitGroupManager->onUnitMorph(unit);
		}
	else if(lost_strategy_num == 3)
		{
			
				this->protoss_FZ->unitGroupManager->onUnitMorph(unit);
		}
}


if(Broodwar->enemy()->getRace() == Races::Terran)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->unitGroupManager->onUnitMorph(unit);
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->unitGroupManager->onUnitMorph(unit);
	}

	else if(lost_strategy_num == 2)
		{			
				this->terran_ZR_Drop->unitGroupManager->onUnitMorph(unit);
		}
}



	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
		this->protoss_Multi->unitGroupManager->onUnitMorph(unit);
	}
	
	
	
	
	
	
	
	
}
void Xelnaga::onUnitRenegade(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	
	
	
	
if(Broodwar->enemy()->getRace() == Races::Protoss)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->unitGroupManager->onUnitRenegade(unit);
	}
	else if(lost_strategy_num == 1)
	{
	this->protoss_Multi->unitGroupManager->onUnitRenegade(unit);
	}

	else if(lost_strategy_num == 2)
		{
			
				this->protoss_ZR->unitGroupManager->onUnitRenegade(unit);
		}
	else if(lost_strategy_num == 3)
		{
			
				this->protoss_FZ->unitGroupManager->onUnitRenegade(unit);
		}
}


if(Broodwar->enemy()->getRace() == Races::Terran)
{
	if(lost_strategy_num == 0)
	{
	this->protoss_FD->unitGroupManager->onUnitRenegade(unit);
	}
	else if(lost_strategy_num == 1)
	{
		this->protoss_Multi->unitGroupManager->onUnitRenegade(unit);
	}

	else if(lost_strategy_num == 2)
		{			
				this->terran_ZR_Drop->unitGroupManager->onUnitRenegade(unit);
		}
}



	if(Broodwar->enemy()->getRace() == Races::Zerg)
	{
		this->protoss_Multi->unitGroupManager->onUnitRenegade(unit);
	}
	
	
	
	
	
	
	
	
	
	
	
	
}


//시간 없어서 안바꿔줬음
void Xelnaga::onSendText(std::string text)
{
	if (Broodwar->isReplay())
	{
		Broodwar->sendText("%s",text.c_str());
		return;
	}
	UnitType type=UnitTypes::getUnitType(text);
	if (text=="debug")
	{
		if (this->protoss_FD->showManagerAssignments==false)
		{
			this->protoss_FD->showManagerAssignments=true;
			this->protoss_FD->buildOrderManager->setDebugMode(true);
			this->protoss_FD->scoutManager->setDebugMode(true);
			this->protoss_FD->spyManager->setDebugMode(true);
			debugMode = true;
		}
		else
		{
			this->protoss_FD->showManagerAssignments=false;
			this->protoss_FD->buildOrderManager->setDebugMode(false);
			this->protoss_FD->scoutManager->setDebugMode(true); //chc
			this->protoss_FD->spyManager->setDebugMode(false);
			debugMode = false;
		}
		Broodwar->printf("%s",text.c_str());
		return;
	}
	if (text=="expand")
	{
		this->protoss_FD->baseManager->expand();
		Broodwar->printf("%s",text.c_str());
		return;
	}
	if (type!=UnitTypes::Unknown)
	{
		this->buildOrderManager->buildAdditional(1,type,300);
	}
	else
	{
		TechType type=TechTypes::getTechType(text);
		if (type!=TechTypes::Unknown)
		{
			this->protoss_FD->techManager->research(type);
		}
		else
		{
			UpgradeType type=UpgradeTypes::getUpgradeType(text);
			if (type!=UpgradeTypes::Unknown)
			{
				this->protoss_FD->upgradeManager->upgrade(type);
			}
			else
				Broodwar->printf("You typed '%s'!",text.c_str());
		}
	}
	Broodwar->sendText("%s",text.c_str());
}


