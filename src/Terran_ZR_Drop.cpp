#include "Terran_ZR_Drop.h"
#include "../src/Util.h"
using namespace BWAPI;

void Terran_ZR_Drop::onStart()
{
	
	d5 = true;
	ddddd=true;
	dddd = true;
	d = true;
	dd= true;
	a=0;
	total_attack = false;
	this->showManagerAssignments=false;
	if (Broodwar->isReplay()) return;

	Broodwar->setTextSize(0);
	Broodwar->sendText("gg hf");

	// Thanks for the trick, Gabriel!
	// Broodwar->setLatCom(false);

	// Enable some cheat flags
	Broodwar->enableFlag(Flag::UserInput);
	// Broodwar->enableFlag(Flag::CompleteMapInformation);

	BWTA::readMap();
	BWTA::analyze();
	this->analyzed=true;
	this->buildManager       = new BuildManager(&this->arbitrator);
	this->techManager        = new TechManager(&this->arbitrator);
	this->upgradeManager     = new UpgradeManager(&this->arbitrator);
	this->spyManager				 = new SpyManager(&this->arbitrator);
	this->workerManager      = new WorkerManager(&this->arbitrator);
	this->supplyManager      = new SupplyManager();
	this->spendManager			 = new SpendManager();
	this->baseManager        = new BaseManager();
	this->buildOrderManager  = new BuildOrderManager(this->buildManager,this->techManager,this->upgradeManager,this->workerManager,this->supplyManager);
	this->defenseManager     = new DefenseManager(&this->arbitrator);
	this->underAttackManager = new UnderAttackManager(&this->arbitrator);
	this->armyManager				 = new ArmyManager(&this->arbitrator);
	this->informationManager = InformationManager::create();
	this->scoutManager       = new ScoutManager(&this->arbitrator, this->informationManager);
	this->borderManager      = BorderManager::create();
	this->unitGroupManager	 = UnitGroupManager::create();
	this->moodManager				 = new MoodManager();
	this->enhancedUI         = new EnhancedUI();

	this->buildOrderManager->enableDependencyResolver();

	this->underAttackManager->setDefenseManager(this->defenseManager);
	this->underAttackManager->setBaseManager(this->baseManager);

	this->supplyManager->setBuildManager(this->buildManager);
	this->supplyManager->setBuildOrderManager(this->buildOrderManager);

	this->spendManager->setBuildManager(this->buildManager);
	this->spendManager->setBuildOrderManager(this->buildOrderManager);
	this->spendManager->setMoodManager(this->moodManager);
	this->spendManager->setArmyManager(this->armyManager);

	this->techManager->setBuildingPlacer(this->buildManager->getBuildingPlacer());

	this->upgradeManager->setBuildingPlacer(this->buildManager->getBuildingPlacer());

	this->workerManager->setBaseManager(this->baseManager);
	this->workerManager->setBuildOrderManager(this->buildOrderManager);

	this->baseManager->setBuildOrderManager(this->buildOrderManager);
	this->baseManager->setBorderManager(this->borderManager);

	this->defenseManager->setBorderManager(this->borderManager);
	this->defenseManager->setBaseManager(this->baseManager);
	this->defenseManager->setInformationManager(this->informationManager);

	this->spyManager->setInformationManager(this->informationManager);
	this->spyManager->setBuildOrderManager(this->buildOrderManager);

	this->armyManager->setDefenseManager(this->defenseManager);
	this->armyManager->setInformationManager(this->informationManager);
	this->armyManager->setBaseManager(this->baseManager);

	this->moodManager->setInformationManager(this->informationManager);

	lastFrameExpand = 0;
	lastScout = 0;
	//chc
	debugMode = false;
	cleaningPath = false;
	destinationMineralSpotted = false;

	BWAPI::Race race = Broodwar->self()->getRace();
	BWAPI::Race enemyRace = Broodwar->enemy()->getRace();
	BWAPI::UnitType workerType=race.getWorker();

	home=BWTA::getStartLocation(Broodwar->self());
	closest = NULL;

	std::string s1 = race.getName();
	std::string s2 = enemyRace.getName();

	this->buildManager->setDebugMode(false);
	this->scoutManager->setDebugMode(false); //chc
	this->spyManager->setDebugMode(false);
	this->buildOrderManager->setDebugMode(false);

	//make the basic production facility

	if(enemyRace == Races::Terran) //
	{


		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Probe,470);//8
		this->buildOrderManager->build(1,UnitTypes::Protoss_Pylon,460);  
		this->buildOrderManager->build(10,UnitTypes::Protoss_Probe,458);  //10
		this->buildOrderManager->build(1,UnitTypes::Protoss_Gateway,456);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,453); //11
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Assimilator,451); 
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Probe,450); //13
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Cybernetics_Core,449);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,447);  //14
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,446); //16
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,445); //17
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,444); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,440); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,437); //19
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Gateway,434);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,431); //21
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Support_Bay,428); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Shuttle,426);  //23
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,424);  //25
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,421); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Reaver,419); //29
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,415); //31
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,412); //33
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,410); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,408); //35


		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Shuttle,405); // 37
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,403); //39
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,395); 

		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Shuttle,390); //41
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,388); //43
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,385); //45
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,383); 
		// 위에 나온거까지 다 해서 공격


		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,380);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,375);  //49
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Gateway,370);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,365); 

		this->buildOrderManager->buildAdditional(3,UnitTypes::Protoss_Zealot,360);
		this->buildOrderManager->buildAdditional(3,UnitTypes::Protoss_Zealot,355);

		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,350); 
		this->buildOrderManager->buildAdditional(3,UnitTypes::Protoss_Zealot,349);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,345); 
		this->buildOrderManager->buildAdditional(101,UnitTypes::Protoss_Zealot,330); 















	}
	
	if(enemyRace == Races::Zerg) // 
	{
	/*
			
	std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); 
			
		Position *e = new Position(2300,120);
		(*it)->rightClick(*e);
	if(Broodwar->getFrameCount()>100)
	{
	TilePosition q = (*it)->getTilePosition();
	(*it)->build(BWAPI::Broodwar->self()->getStartLocation(),UnitTypes::Protoss_Pylon);
	}
		
		this->buildOrderManager->build(5,UnitTypes::Protoss_Probe,470);
	
		
		UnitGroup::build
*/		
		
		//this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,90);
		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Probe,470);//8
		this->buildOrderManager->build(1,UnitTypes::Protoss_Pylon,460);  
		this->buildOrderManager->build(10,UnitTypes::Protoss_Probe,450);  
		this->buildOrderManager->build(1,UnitTypes::Protoss_Gateway,440);
		this->buildOrderManager->build(13,UnitTypes::Protoss_Probe,430);
		this->buildOrderManager->build(2,UnitTypes::Protoss_Gateway,420);
		this->buildOrderManager->build(1,UnitTypes::Protoss_Zealot,410);
		this->buildOrderManager->build(14,UnitTypes::Protoss_Probe,400);
		this->buildOrderManager->build(2,UnitTypes::Protoss_Pylon,390);

		this->buildOrderManager->build(15,UnitTypes::Protoss_Probe,380);
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,370);
		this->buildOrderManager->build(4,UnitTypes::Protoss_Gateway,340);
		this->buildOrderManager->build(3,UnitTypes::Protoss_Pylon,330);
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,320);
		this->buildOrderManager->build(4,UnitTypes::Protoss_Pylon,310);
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,300);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,289);


		//병력 모이면 공격
		for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
    {			
		if ((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
        {
			std::set<BWAPI::Unit *>::const_iterator zealot = it;
			BWAPI::Position pos = informationManager->getEnemyStartLocation()->getPosition();



for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
	{
			

			if(spendManager->getUnitCount()->zealot >= 11)
			(*zealot)->attack(pos);
		
		}
	}
}
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,250);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,249);

		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,248);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,247);
     
		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,246);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,245);

		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,243);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,242);

		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,240);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,225);

		this->buildOrderManager->buildAdditional(100,UnitTypes::Protoss_Zealot,200);





	}

	if(enemyRace == Races::Protoss) // 
	{
/*	
		//test
	//병력 모이면 공격
		for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
    {			
		if ((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
        {
			std::set<BWAPI::Unit *>::const_iterator zealot = it;
			BWAPI::Position pos = informationManager->getEnemyStartLocation()->getPosition();



	{
			

			if(spendManager->getUnitCount()->zealot >= 11)
			(*zealot)->attack(pos);
		
		}
	}
}


	

	
							if(spendManager->getUnitCount()->zealot >= 1)
							{
		for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {			
		
	
		if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Probe)
        {
			std::set<BWAPI::Unit *>::const_iterator Probe = its;
			Position *e = new Position(2300,120);
			Position *em = new Position(2100,2100);
			(*Probe)->rightClick(*em,true);
			(*Probe)->rightClick(*e,true);

			
					
		}
		}
							}



*/

		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Probe,470); //8
		this->buildOrderManager->build(1,UnitTypes::Protoss_Pylon,460);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,450); //9
		this->buildOrderManager->build(1,UnitTypes::Protoss_Gateway,440);
    	this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,430); //10
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Assimilator,420); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,410); //11
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,409); //12
		this->buildOrderManager->build(1,UnitTypes::Protoss_Zealot,409); //14
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Cybernetics_Core,408);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,407); //15
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,406); //16
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,406);  
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,405); //17
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,404); // 18
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Dragoon,404); // 20
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Citadel_of_Adun,402); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,401); //21
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Gateway,400);
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Probe,399);//23
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Templar_Archives,398);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,397);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Dragoon,396);//25
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,395);//26
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,395); //27
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Dark_Templar,395); //31
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Gateway,392);
		this->buildOrderManager->upgrade(1, UpgradeTypes::Leg_Enhancements, 391);

		
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,390);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,388);
		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,381);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,380);

		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,379);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,376);

		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,375);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,374);

		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,373);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,372);

		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,371);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,370);

		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,368);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,367);

		
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Zealot,365);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,363);

		this->buildOrderManager->buildAdditional(101,UnitTypes::Protoss_Zealot,300);

		

	

		//다크컨트롤
/*
   for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {			
		if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {
			std::set<BWAPI::Unit *>::const_iterator dark = its;
			
			BWAPI::Position pos = this->informationManager->getEnemyStartLocation()->getPosition();
			(*dark)->rightClick(pos);


			
			if((*dark)->isUnderAttack())
			{
				Position *poss = new Position(2100,2100);				
				(*dark)->rightClick(*poss);
				
			}





		}
   }
		*/
	
	/*잠시막아놓음 chc
	this->workerManager->enableAutoBuild();
	this->workerManager->setAutoBuildPriority(100);
	this->baseManager->setRefineryBuildPriority(30);
	*/


}
}


Terran_ZR_Drop::~Terran_ZR_Drop()
{
	delete this->buildManager;
	delete this->techManager;
	delete this->upgradeManager;
	delete this->scoutManager;
	delete this->spyManager;
	delete this->workerManager;
	delete this->supplyManager;
	delete this->spendManager;
	delete this->buildOrderManager;
	delete this->baseManager;
	delete this->defenseManager;
	delete this->underAttackManager;
	delete this->armyManager;
	delete this->moodManager;
	InformationManager::destroy();
	BorderManager::destroy();
	UnitGroupManager::destroy();
	delete this->enhancedUI;
}

void Terran_ZR_Drop::onEnd(bool isWinner)
{
	log("onEnd(%d)\n",isWinner);
}

void Terran_ZR_Drop::onFrame()
{


	//온프레임일때 테스트
//	this->buildOrderManager->upgrade(1, UpgradeTypes::Leg_Enhancements, 391);

	//적이 프로토스
	if(Broodwar->enemy()->getRace() == Races::Protoss) 
	{



//적 다크일때
for (std::set<BWAPI::Unit *>::const_iterator t = Broodwar->enemy()->getUnits().begin(); t != BWAPI::Broodwar->enemy()->getUnits().end(); ++t)
			{
				
				if((*t)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun || (*t)->getType() == BWAPI::UnitTypes::Protoss_Templar_Archives)   
				{

						this->buildOrderManager->build(1,UnitTypes::Protoss_Robotics_Facility,500);
						this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,500);
						this->buildOrderManager->build(2,UnitTypes::Protoss_Observer,500);
						this->buildOrderManager->build(1,UnitTypes::Protoss_Pylon,379);


						 for (std::set<BWAPI::Unit *>::const_iterator its5 = BWAPI::Broodwar->self()->getUnits().begin(); its5 != BWAPI::Broodwar->self()->getUnits().end(); ++its5)
    {		
	
		if ((*its5)->getType() == BWAPI::UnitTypes::Protoss_Observer)
        {
			 for (std::set<BWAPI::Unit *>::const_iterator its6 = BWAPI::Broodwar->self()->getUnits().begin(); its6 != BWAPI::Broodwar->self()->getUnits().end(); ++its6)
    {		

		if ((*its6)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
        {
			

			(*its5)->rightClick(*its6,true);

			}
			 }
		}
						 }
				
				
						
				
				}

		if((*t)->getType() == BWAPI::UnitTypes::Protoss_Templar_Archives)   
				{

                      //  this->armyManager->observer = false;
						// armyManager->observer = false;
		}



}
	
				



						

	





	//다크길 열어주기
	
/*	if(dddd && spendManager->getUnitCount()->darkTemplar >= 2 )
	{
	 for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {		
		std::set<BWAPI::Unit *>::const_iterator all = its;

	//	BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
		
BWAPI::Position poss = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
			(*all)->rightClick(poss);

	 }
	 dddd = false;


	}

*/


	
		//다크적진으로 가기
		if(d)
	{
   for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {		
	
		if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {
			std::set<BWAPI::Unit *>::const_iterator dark = its;
			
			
			BWAPI::Position pos = this->informationManager->getEnemyStartLocation()->getPosition();
			(*dark)->rightClick(pos);
			//d = false;



			if((*dark)->getPosition().x() < pos.x() + 200 && (*dark)->getPosition().x() > pos.x() - 200 && (*dark)->getPosition().y() < pos.y() + 200 && (*dark)->getPosition().y() > pos.y() - 200)
			d = false;
			
			}
   
   }
		
			
		}


		//넥서스부터 공격
		if(d5)
		{
for (std::set<BWAPI::Unit *>::const_iterator pn = Broodwar->enemy()->getUnits().begin(); pn != BWAPI::Broodwar->enemy()->getUnits().end(); ++pn)
			{
				
				if((*pn)->getType() == BWAPI::UnitTypes::Protoss_Nexus)
				{

				 for (std::set<BWAPI::Unit *>::const_iterator itss = BWAPI::Broodwar->self()->getUnits().begin(); itss != BWAPI::Broodwar->self()->getUnits().end(); ++itss)
    {		
	
		if ((*itss)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {
			(*itss)->rightClick(*pn);
		}
				 }
				}
}
		}










	//	 if(armyManager->myDPS() > 6) // 본진으로 못돌아오기 6보다 크면(에러와 공격멤버들이 본진으로 돌아오는 것 때문)
	//		 dd = false;
	//	 if(this->armyManager->myDPS() > 6) // 본진으로 못돌아오기 6보다 크면(에러와 공격멤버들이 본진으로 돌아오는 것 때문)
	//		 dd = false;

			//본진으로 돌아오기
			if(dd)
			{

				 for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
    {		
	
		if ((*its2)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {

			std::set<BWAPI::Unit *>::const_iterator dark2 = its2;
			if((*dark2)->isUnderAttack())
			{
				d = false;
				d5 = false;
				 int i=0;

				  for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {		
		std::set<BWAPI::Unit *>::const_iterator all = its;

	//	BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
		
BWAPI::Position poss = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
			(*all)->rightClick(poss);

				  }
             //   BWAPI::Position chokePosition=myBorderVector[i]->getCenter();
				//Position *poss = new Position(2100,2100);	

				//본진 초크포인트로 돌아오기

				   
			//		BWAPI::Position chokePosition=myBorderVector[i]->getCenter();
			//		i++;
			//	 if (i>=(int)myBorderVector.size())

              //      i=0;


 for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
    {		
	
		if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
		{
			BWAPI::Position posss = (*its)->getPosition();
				 	
					(*dark2)->rightClick(posss);
					//dd = false;
				//	ddd = false;
					
		}
			}	
		}
		}
				 }
			//	 dd = false;
			//		ddd = false;
			}
	
/*
	// 프로토스일때
if(Broodwar->enemy()->getRace() == Races::Protoss)  
		{
	
			
//병력 모이면 공격
		for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
    {			
		if ((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
        {
			std::set<BWAPI::Unit *>::const_iterator zealot = it;
			BWAPI::Position pos = this->informationManager->getEnemyStartLocation()->getPosition();
		if ((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot || (*it)->getType() == BWAPI::UnitTypes::Protoss_Dragoon || (*it)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
		{                   std::set<BWAPI::Unit *>::const_iterator alot = it;


for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
	{
			

			if(spendManager->getUnitCount()->zealot >= 10)
				(*alot)->attack(pos);
    }
		}
		}
		}

}
*/

	//
}
//적 테란일때

			if(Broodwar->enemy()->getRace() == Races::Terran)
			{
			//	if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Shuttle >= 3)
				
		

			/*		for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
					{		
						a = 0;

						if((*it)->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
		{
			
			a++;
		}
					
					}*/

				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) < 3)
				{
//질럿태우기
					if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Shuttle) >= 3)
					{

					for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
					{		
		if((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
		{
			
			for (std::set<BWAPI::Unit *>::const_iterator it2 = BWAPI::Broodwar->self()->getUnits().begin(); it2 != BWAPI::Broodwar->self()->getUnits().end(); ++it2)
    {		
		
			
		if ((*it2)->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
        {

			(*it)->load(*it2);
		}
			}
		}
					}
					}

//리버태우기

							if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Shuttle) >= 3)
					{

					for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
					{		
						if((*it)->getType() == BWAPI::UnitTypes::Protoss_Reaver)
		{
			
			for (std::set<BWAPI::Unit *>::const_iterator it2 = BWAPI::Broodwar->self()->getUnits().begin(); it2 != BWAPI::Broodwar->self()->getUnits().end(); ++it2)
    {		
		
			
		if ((*it2)->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
        {

			(*it)->load(*it2);
		}
			}
		}
					}
					}
				}


				//파일런 6개 이상일 때 리버 스크랩 다 채우기
				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) >= 5)
				this->buildOrderManager->build(100,UnitTypes::Protoss_Scarab,800);

			
				//리버공격
/*
	if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 3) 
				{
						for (std::set<BWAPI::Unit *>::const_iterator it7 = BWAPI::Broodwar->self()->getUnits().begin(); it7 != BWAPI::Broodwar->self()->getUnits().end(); ++it7)
    {	

			if ((*it7)->getType() == BWAPI::UnitTypes::Protoss_Reaver)
        {
			BWAPI::Position pos11 = informationManager->getEnemyStartLocation()->getPosition();
			(*it7)->attack(pos11);
			}
						}
	}

*/

			
			
//게이트 3개 이상일때 공격
				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 3) 
				{
					//BWAPI::Position pos[99999];
					int al = 0;
					
						for (std::set<BWAPI::Unit *>::const_iterator it3 = BWAPI::Broodwar->self()->getUnits().begin(); it3 != BWAPI::Broodwar->self()->getUnits().end(); ++it3)
    {	

			if ((*it3)->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
        {
			if(ddddd)
			{

			BWAPI::Position pos10 = informationManager->getEnemyStartLocation()->getPosition();
			
		//	this->defenseManager->update().chokepoint2;
			(*it3)->rightClick(BWTA::getNearestChokepoint(pos10)->getCenter(),true);
			(*it3)->rightClick(pos10,true);
			

			}
			}
						}
						ddddd=false;

		//	if((*it3)->isMoving())
			//	ddddd = false;



			
			
				for (std::set<BWAPI::Unit *>::const_iterator it3 = BWAPI::Broodwar->self()->getUnits().begin(); it3 != BWAPI::Broodwar->self()->getUnits().end(); ++it3)

    {	
			if ((*it3)->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
        {

				for (std::set<BWAPI::Unit *>::const_iterator n = Broodwar->enemy()->getUnits().begin(); n != BWAPI::Broodwar->enemy()->getUnits().end(); ++n)
			{
				
				if((*n)->getType() == BWAPI::UnitTypes::Terran_Bunker || (*n)->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode || (*n)->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || (*n)->getType() == BWAPI::UnitTypes::Terran_Missile_Turret || (*n)->getType() == BWAPI::UnitTypes::Terran_Command_Center)
				//if((*n)->getType() == BWAPI::UnitTypes::Terran_SCV)
				{
					
			//BWAPI::Position pos10 = informationManager->getEnemyStartLocation()->getPosition();
		
						BWAPI::Position pos10 = informationManager->getEnemyStartLocation()->getPosition();
					//pos[al] = (*n)->getPosition();
					//al++;
					BWAPI::Position pos = (*n)->getPosition();
					//ddddd=false;

	if((*it3)->getPosition().x() > BWTA::getNearestChokepoint(pos10)->getCenter().x() - 900 && (*it3)->getPosition().x() < BWTA::getNearestChokepoint(pos10)->getCenter().x() + 900 && (*it3)->getPosition().y() > BWTA::getNearestChokepoint(pos10)->getCenter().y() - 900 && (*it3)->getPosition().y() < BWTA::getNearestChokepoint(pos10)->getCenter().y() + 900)
	{
		if((*it3)->getDistance(*n)<180)
					{

						
							for (std::set<BWAPI::Unit *>::const_iterator it4 = BWAPI::Broodwar->self()->getUnits().begin(); it4 != BWAPI::Broodwar->self()->getUnits().end(); ++it4)

    {	
		if ((*it4)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
        {

					
					(*it3)->unload(*it4);
					total_attack = true;
					
		}

			if ((*it4)->getType() == BWAPI::UnitTypes::Protoss_Reaver)
        {

					
					(*it3)->unload(*it4);
					
		}
					//(*it3)->getDistance(*n);
				//	Broodwar->drawTextScreen(5,30,"I have %d units:", (*it3)->getDistance(*n));
							}							
				
		}
				}
				

//				std::set<BWAPI::Unit *>::const_iterator dark = its;

				}
				
			}
						

			
						}
				}


				
				}

				}

				
				
				Position en_start = this->informationManager->getEnemyStartLocation()->getPosition();
	Position my_start = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
		Position my_choke = BWTA::getNearestChokepoint(my_start)->getCenter();
		Position my_ex1 =  BWTA::getNearestBaseLocation(my_choke)->getPosition();
				
		//AIIDE2014 공격
		static int frametest_defens2 = Broodwar->getFrameCount();
		if(frametest_defens2 == Broodwar->getFrameCount())
		{
			frametest_defens2 = frametest_defens2 + 24*5;
			if(total_attack)
			{
				for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
		{
			if((double)(*allMyUnit)->getType().groundWeapon().damageAmount() > 6 || (*allMyUnit)->BWAPI::UnitTypes::Protoss_Reaver)
							{
								(*allMyUnit)->attack(en_start);
								
								




				}

			}
				
			}
		}

				
			

				/////AIIDE2014방어
		double distEnemyNexus = 0;
		double distMyNexus = 0;
		BWAPI::Position *myNexusPosition = new Position(0,0);

		// double distEnemyGround = 0;
		int enemyi = 0;

		bool defense_ing = false; //방어중인지?
	

	


		//////////////////////////////방어  변수들만 여기에




		/////방어시작

		for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
		{
			if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Nexus) 
			{
				*myNexusPosition = (*allMyUnit)->getPosition();
			}

		}



		static int frametest_defens = Broodwar->getFrameCount();
		if(frametest_defens == Broodwar->getFrameCount())
		{
			frametest_defens = frametest_defens + 20;


			if(!total_attack)  // 총공격이 아닐 때만 방어 시스템 작동 
			{

				//AIIDE2014 적일꾼 방어하기위함.
				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
				{
					for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
						{
							if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe) 
								if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_SCV ||  (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe)
								{
									if((*allEnemyUnit)->getDistance(my_start) < 550)
					{
									(*allMyUnit)->attack(*allEnemyUnit);	
										break;
								}
								}

					}

				}






				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
				{
					//if((*allEnemyUnit)->BWAPI::UnitTypes::Zerg_Overlord)
					//	continue;  //적이 오버로드이면 그냥 넘어감.

					int j = -1;
					int i = 0;
					bool k = false;
					//enemyi = 0;

					BWAPI::Position *enemyPosition = new Position(0,0);
					BWAPI::Position *myPosition = new Position(0,0);

					*enemyPosition = (*allEnemyUnit)->getPosition();

					//double EnemyPower=0;
					//EnemyPower = ((double)allEnemyUnit->getType().groundWeapon().damageAmount())/allEnemyUnit->getType().groundWeapon().damageCooldown()


					//본진
					/*
					for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
					{
					if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Nexus) 
					{
					distEnemyNexus = (*allEnemyUnit)->getDistance(*allMyUnit);
					//*myNexusPosition = (*allMyUnit)->getPosition();
					}

					}
					*/

					//앞마당
					for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
					{
						if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Nexus) 
						{
							distEnemyNexus = (*allEnemyUnit)->getDistance(*allMyUnit);
							break;
							//*myNexusPosition = (*allMyUnit)->getPosition();
						}

					}





					BWAPI::Position pos1241 = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();

					if((*allEnemyUnit)->getDistance(pos1241) < 550 || distEnemyNexus < 340 || (*allEnemyUnit)->getDistance(my_ex1) < 340)
					{
						for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
						{




							//cig2013  -- 초반에 질럿vs 질럿 전투이면서 적과 120거리 미만인에들은 방어시스템 적용 x 어짜피, 질럿vs 질럿에서 공격해줌.  -- 시작 --

							
							/*
								bool right_combat_cig=false;
								for(int ii=0;ii<20;ii++)
								{
									if((*allMyUnit)->getID() == Start_Combat_GetId[ii])
										right_combat_cig=true;
								}

								if(right_combat_cig == true)
									continue;
									*/
							

							//cig2013  -- 초반에 질럿vs 질럿 전투이면서 적과 120거리 미만인에들은 방어시스템 적용 x 어짜피, 질럿vs 질럿에서 공격해줌.  -- 끝 --





							if((double)(*allMyUnit)->getType().groundWeapon().damageAmount() > 3 && (*allMyUnit)->isCompleted() && (*allMyUnit)->getType().canMove() && !((*allEnemyUnit)->BWAPI::UnitTypes::Zerg_Overlord))
							{
								//프로브는 적이 일꾼일때만 나오게.
								if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe && !((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe) && !((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Zerg_Drone) && !((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_SCV))
									continue;
								if(!((*allMyUnit)->getType().canMove()))
									continue;
								if(!((*allMyUnit)->isCompleted()))
									continue;

								if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe) //일꾼끼리는 따로 공격명령 해줌.
									continue;

								//언제나 begin과 begin+1만 선택되는 문제 해결 
								j++;
								if(enemyi<=j || k==true)
								{			
									//BWAPI::Broodwar->drawLineMap((*allMyUnit)->getPosition().x(), (*allMyUnit)->getPosition().y(), (*allEnemyUnit)->getPosition().x(), (*allEnemyUnit)->getPosition().y(), BWAPI::Colors::Red);
									(*allMyUnit)->attack((*allEnemyUnit)->getPosition());	
									defense_ing = true;
									//(*allMyUnit)->rightClick(*allEnemyUnit);
									enemyi = enemyi + 1;
									i++;
									k=true;


									//만약 적이 일꾼이면 우리편 한마리만 달라붙어라. - 적용하지 않았음.
									/*
									if(((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe) || ((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Zerg_Drone) || ((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_SCV))
									break;
									*/

									if(i==2)
										break;
								}
							}
						}
						//BWAPI::Position myPosition = new Position(0,0);
						//	myPosition = (*allMyUnit)->getPosition();
						//	double myPower=0;
						//	myPower = ((double)allMyUnit->getType().groundWeapon().damageAmount())/allMyUnit->getType().groundWeapon().damageCooldown();
					}



				}

			}

		}
				
				






			

	if (Broodwar->isReplay()) return;
	if (!this->analyzed) return;

	this->buildManager->update();
	this->buildOrderManager->update();
	this->baseManager->update();
	
	
	this->workerManager->update();//chc

	//this->techManager->update();
	this->upgradeManager->update();
	this->supplyManager->update();
	this->moodManager->update();
	this->spendManager->update();
	this->scoutManager->update();
	if (!this->informationManager->getEnemyBases().empty() && Broodwar->getFrameCount() > 3000)
	  {
			this->defenseManager->update();

	    if (!this->spyManager->isInitialized())
	      this->spyManager->initialize();
	    this->spyManager->update();
	  }
	this->enhancedUI->update();
	this->borderManager->update();
	this->underAttackManager->update();
	this->armyManager->update();
	this->arbitrator.update();

	if ((Broodwar->mapName().compare(1,11,"Destination")  == 0) && !destinationMineralSpotted)
	{
		destinationMineralSpotted = true;
		Position *p;

		//Low position
		if (BWAPI::Broodwar->self()->getStartLocation().y() > 50)
		{
			p = new Position(1312,3856);
		}
		//High position
		else
		{
			p = new Position(1760,208);
		}
		for each (BWAPI::Unit *u in Broodwar->getAllUnits())
		{
			if ((u->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) && (u->getPosition().x() == p->x()) && (u->getPosition().y() == p->y()))
			{
				workerManager->setDestinationMineral(u);
				break;
			}
		}
		delete p;
	}

	

	if (debugMode)
	{
		Broodwar->drawTextScreen(420,30,"DPS: %.2f", armyManager->myDPS());
		Broodwar->drawTextScreen(420,40,"Zealots:        %3d%% / %3d%%", (int)(spendManager->getUnitCount()->zealotRatio*100), (int)(spendManager->getUnitCount()->zealotIdealRatio*100));
		Broodwar->drawTextScreen(420,50,"Dragoons:      %3d%% / %3d%%", (int)(spendManager->getUnitCount()->dragoonRatio*100), (int)(spendManager->getUnitCount()->dragoonIdealRatio*100));
		Broodwar->drawTextScreen(420,60,"Corsairs:       %3d%% / %3d%%", (int)(spendManager->getUnitCount()->corsairRatio*100), (int)(spendManager->getUnitCount()->corsairIdealRatio*100));
		Broodwar->drawTextScreen(420,70,"Dark Templars: %3d%% / %3d%%", (int)(spendManager->getUnitCount()->darkTemplarRatio*100), (int)(spendManager->getUnitCount()->darkTemplarIdealRatio*100));
	}


	
//	Broodwar->drawTextScreen(5,0,"MY dps :   %dEnemy dps :%d ", this->armyManager->myDPS(), this->armyManager->enemyDPS());
  
/*chc 프로브 많이생산 하는것 .. 나중에 
	if ((Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe) < 5))
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,90);

		*/
		/*
	//chc
 for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
			//BWAPI::Unit* u = b->builderUnit;
			Position *q = new Position(1760,208);;
			(*i)->rightClick(*q);
	   }
	}
//
*/
	//chc
	if (Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe) == 10)
	{		
		this->scoutManager->setScoutCount(1);
		
	}


	std::set<Unit*> units=Broodwar->self()->getUnits();
	if (this->showManagerAssignments)
	{
		for(std::set<Unit*>::iterator i=units.begin();i!=units.end();i++)
		{
			if (this->arbitrator.hasBid(*i))
			{
				int x=(*i)->getPosition().x();
				int y=(*i)->getPosition().y();
				std::list< std::pair< Arbitrator::Controller<BWAPI::Unit*,double>*, double> > bids=this->arbitrator.getAllBidders(*i);
				int y_off=0;
				bool first = false;
				const char activeColor = '\x07', inactiveColor = '\x16';
				char color = activeColor;
				for(std::list< std::pair< Arbitrator::Controller<BWAPI::Unit*,double>*, double> >::iterator j=bids.begin();j!=bids.end();j++)
				{
					Broodwar->drawTextMap(x,y+y_off,"%c%s: %d",color,j->first->getShortName().c_str(),(int)j->second);
					y_off+=15;
					color = inactiveColor;
				}
			}
		}
	}
}

void Terran_ZR_Drop::onUnitDestroy(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->arbitrator.onRemoveObject(unit);
	this->buildManager->onRemoveUnit(unit);
	this->techManager->onRemoveUnit(unit);
	this->upgradeManager->onRemoveUnit(unit);
	this->workerManager->onRemoveUnit(unit);  
	this->scoutManager->onRemoveUnit(unit);
	this->spyManager->onRemoveUnit(unit);
	this->defenseManager->onRemoveUnit(unit);
	this->informationManager->onUnitDestroy(unit);
	this->baseManager->onRemoveUnit(unit);
	this->underAttackManager->onRemoveUnit(unit);
	this->armyManager->onRemoveUnit(unit);
}

void Terran_ZR_Drop::onUnitDiscover(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->informationManager->onUnitDiscover(unit);
	this->unitGroupManager->onUnitDiscover(unit);
}
void Terran_ZR_Drop::onUnitEvade(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->informationManager->onUnitEvade(unit);
	this->unitGroupManager->onUnitEvade(unit);
}

void Terran_ZR_Drop::onUnitMorph(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->unitGroupManager->onUnitMorph(unit);
}
void Terran_ZR_Drop::onUnitRenegade(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->unitGroupManager->onUnitRenegade(unit);
}



void Terran_ZR_Drop::onSendText(std::string text)
{
	if (Broodwar->isReplay())
	{
		Broodwar->sendText("%s",text.c_str());
		return;
	}
	UnitType type=UnitTypes::getUnitType(text);
	if (text=="debug")
	{
		if (this->showManagerAssignments==false)
		{
			this->showManagerAssignments=true;
			this->buildOrderManager->setDebugMode(true);
			this->scoutManager->setDebugMode(true);
			this->spyManager->setDebugMode(true);
			debugMode = true;
		}
		else
		{
			this->showManagerAssignments=false;
			this->buildOrderManager->setDebugMode(false);
			this->scoutManager->setDebugMode(true); //chc
			this->spyManager->setDebugMode(false);
			debugMode = false;
		}
		Broodwar->printf("%s",text.c_str());
		return;
	}
	if (text=="expand")
	{
		this->baseManager->expand();
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
			this->techManager->research(type);
		}
		else
		{
			UpgradeType type=UpgradeTypes::getUpgradeType(text);
			if (type!=UpgradeTypes::Unknown)
			{
				this->upgradeManager->upgrade(type);
			}
			else
				Broodwar->printf("You typed '%s'!",text.c_str());
		}
	}
	Broodwar->sendText("%s",text.c_str());
}


