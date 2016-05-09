#include "Protoss_ZR.h"
#include "../src/Util.h"
using namespace BWAPI;



int dd21 = 0;
bool isPhotoRush = false;
bool isRushMyNexus = false;

bool isFirstChoke = false;
bool isLastChoke = false;

//파일 입출력
#include <stdlib.h>
void Protoss_ZR::onStart()
{
	
	scout_start = false;
	enemy_start_location_change = 0;
	enemy_start_location_change_true = true;
	enemy_start_first = true;
	//d5 = true;
	ddddd=true;
	dddd = true;
	firsss21 = false;
	first_probe_onrevoke = true;
	d = true;
	dd= true;
	a=0;
	search_prob_id = -1000;
	high_templer_loaded_complete = true; //하이템플러 컴플리트
	en_expand_exist = false;
	ob_bool = false;
	ob_bool2 = false;
	enemy_dead_frame = 999999;
	enemy_dead_after_2min = false;
	first_tt22 = false;
	enemy_attack_first1 = false;
	probe_attack_id = 0;
	zerg_first_attack = false; //저그 처음 질럿11마리 공격 
	early_rush = false;
	continuos_bu = false;
	very_early_rush = false;
	cheat_early = false;
	my_exten = false;
	cyber_cancel = false;
	defense_makgi = false;
	very_early_rush_build = false;
	very_early_rush_auto_build = false;
	multi_bool1 = false;
	multi_bool2 = false;
	multi_bool3 = false;
	multi_bool4 = false;
	cig1 = false;
	cig2 = false;
	cig0 = false;
	cig3_fastest_3gate_zealot = true;
	cig4_fatest_fix_build_end = false;
	cig4_slow_fix_build_end = false;
	cig_2013_cyber_timing = -2;
	cig_2013_adun_timing = -2;
	cig5_terran_start_auto_build = false;
	cig2013_11 = false;

	Protoss_one_Gateway_cig_id = -2;
	Protoss_two_Gateway_cig_id = -2;
	Protoss_one_gas_cig_id = -2;

	getFirst_scout_probe_ID = -1000;
	getCIG2013_scout_probe_ID = -1000;

	aiide2014_auto_start = false;	
	aiide2014_once1 = false;

	bool			firsss = false;
	bool			firsss2 = false;

	scout_complete2 = false;
	early_attack_assimi = true;
	early_attack_gate = true;
	very_early_attack_gate = true;


	zerg_first_ = false;

	used_ob[0] = 0; //옵저버 관련 초기화
	used_ob[1] = 0; //옵저버 관련 초기화
	ob_3_expand = 0;
	enemy_search_unit = 0; //적 정찰유닛

	enemy_dead = false;

	base_lo_first_input = false;
	my_auto_build = false;
	my_expand_true = false;

	robotics_1 = false;
	shuttle_1 = false;
	storm_enemy_confidence_first = false;



	this->showManagerAssignments=false;
	total_attack=false; //총공격 flase;
	if (Broodwar->isReplay()) return;

	Broodwar->setTextSize(0);
	Broodwar->sendText("Protoss_ZR v2.0 AI Bot Testing");

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
		//테란전 기본빌드 더블넥
		//더블넥 cig2013
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Probe,470); //8
		this->buildOrderManager->build(1,UnitTypes::Protoss_Pylon,460);
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,450); //9
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,430); //10
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,410); //11

		baseManager->expand(405); 
		this->buildOrderManager->build(1,UnitTypes::Protoss_Gateway,400);
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Probe,395); //13
		this->buildOrderManager->build(1,UnitTypes::Protoss_Gateway,390);
	}


	if(enemyRace == Races::Zerg) // 
	{
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
	}

	if(enemyRace == Races::Protoss) // 
	{
		this->buildOrderManager->buildAdditional(4,UnitTypes::Protoss_Probe,470); //8
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,460);
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Probe,450); //9
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Gateway,440);
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Probe,430); //12
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Gateway,420); 
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,410); //13
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,400); //15
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,390); //15
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,380); //16
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,370); //20
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Assimilator,360); //20
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,350); //21
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,340); //15
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,330); //25
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Probe,320); //27
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Cybernetics_Core,310); //27
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,300); //28
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,295); //32
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,290); //32


		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Probe,285); //33
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,280); //33
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Zealot,275); //37
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Dragoon,275); //37
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,270); //37
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Citadel_of_Adun,265); //37
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,260); //41
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Observatory,255); //41
		this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,250); //45
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,245); //45
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Observer,240); //45
		this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Support_Bay,235); //45
	}
}

Protoss_ZR::~Protoss_ZR()
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

void Protoss_ZR::onEnd(bool isWinner)
{
	log("onEnd(%d)\n",isWinner);
}

void Protoss_ZR::onFrame()
{


	static int aa414 = Broodwar->getFrameCount();
	if(aa414 == Broodwar->getFrameCount()) 
	{
		aa414 = aa414+4;
			
	//캐논러쉬방어 aiide2014

		for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit)
				{
					if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*allMyUnit)->isCompleted())
					{
						for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit !=BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
						{

							if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Pylon)
							{
								if((*allEnemyUnit)->getDistance(BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition()) < 500)
									(*allMyUnit)->attack(*allEnemyUnit);																		

							}


						}
					}
		}

		for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
				{
					if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*allMyUnit)->isCompleted())
					{
						for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit !=BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
						{

							if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon)
							{
								if((*allEnemyUnit)->getDistance(BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition()) < 500)
									(*allMyUnit)->attack(*allEnemyUnit);																		

							}


						}
					}
		}
	}



	//----------------------------------------------------------수비부분----------------------------------------------------- 시작
	///☆☆☆☆☆☆☆☆☆☆☆☆질럿vs질럿 전투만 시작☆☆☆☆☆☆☆☆☆☆

	int Start_Combat_GetId[20] = {0,};
	int Start_Combat_GetId_num=0;


	static int cig2013_1 = Broodwar->getFrameCount();
	static int cig2013_2 = Broodwar->getFrameCount();
	static int cig2013_3 = Broodwar->getFrameCount();


	if(Broodwar->enemy()->getRace() == Races::Protoss)
	{

	}

/*
	if(Broodwar->enemy()->getRace() == Races::Protoss)
	{
		if(cig2013_1 == Broodwar->getFrameCount())
		{
			cig2013_1 = cig2013_1 + 4;

			//완성된 질럿이 6마리 이하이거나 드라군이 없을 때 발동
			if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) ==0 && total_attack == false)
			{

				

				//--1.각 유닛이 적과 일정거리(132) 안에 있으면 전투시스템 발동
				for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
				{
					if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*allMyUnit)->isCompleted())
					{
						for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit !=BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
						{
							if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
							{
								if((*allMyUnit)->getDistance(*allEnemyUnit) < 132)
									Start_Combat_GetId[Start_Combat_GetId_num] = (*allMyUnit)->getID();
							}
						}

						if(Start_Combat_GetId[Start_Combat_GetId_num] !=0)
							Start_Combat_GetId_num++;
					}
				}
				bool total_attack_zealot_cig2013=false;
				//--2 전체 도망갈지 전체 싸울지 결정 -

				
				// 질럿<=6일 때 -2마리까지 상대가능.  aiide2014 이전에는 +0  
				if(Start_Combat_GetId_num-2 >= BWAPI::Broodwar->enemy()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) && BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) <=6)
					total_attack_zealot_cig2013=true;

				// 질럿>=7 일 때 -2마리까지 상대가능. aiide2014 이전에는 +1
				if(Start_Combat_GetId_num-2 >= BWAPI::Broodwar->enemy()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) && BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >=7)
					total_attack_zealot_cig2013=true;

				

				//질럿 도망가는 액션
				if(total_attack_zealot_cig2013 == false)
					for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
					{
						if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*allMyUnit)->isCompleted())
						{
							bool right_combat_cig=false;
							for(int ii=0;ii<20;ii++)
							{
								if((*allMyUnit)->getID() == Start_Combat_GetId[ii])
									right_combat_cig=true;
							}


							/////////////////////

							//질럿이 14이상일때는 도망 안감.  AIIDE2014 이전 - 질럿이 8이상일때는 도망 안감.
							if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) < 14)
								if(right_combat_cig==true && (*allMyUnit)->getDistance(BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition()) > 250)//도망가기. 본진주변에서는 도망 못감 // AIIDE2014 이전에는 140
									(*allMyUnit)->move(BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition());
						}
					}

					int num_low_hp=0;


					if(cig2013_2 == Broodwar->getFrameCount())
					{
						cig2013_2 = cig2013_2 + 16;
						if(total_attack_zealot_cig2013==true)
						{
							for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
							{
								if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*allMyUnit)->isCompleted())
								{
									bool right_combat_cig=false;
									for(int ii=0;ii<20;ii++)
									{
										if((*allMyUnit)->getID() == Start_Combat_GetId[ii])
											right_combat_cig=true;
									}

									if(right_combat_cig==true)
									{

										//--3 공격할 적 선택하고 공격
										//적들 점수 내기


										//distance 보통 : 20~110
										//hitpoint 보통 : 0~100
										int min_good_attack_unit_score=999999;
										int enemy_under_attack_score=0;

										std::set<BWAPI::Unit *>::const_iterator attack_good = BWAPI::Broodwar->enemy()->getUnits().begin();

										for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
										{
											if((*allEnemyUnit)->isUnderAttack())
												enemy_under_attack_score = -5;



											if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) <=4)//4마리보다 적으면 1.4가능
												if(min_good_attack_unit_score > (*allMyUnit)->getDistance(*allEnemyUnit) + (*allEnemyUnit)->getHitPoints() + enemy_under_attack_score)
												{
													min_good_attack_unit_score = (*allMyUnit)->getDistance(*allEnemyUnit) + (*allEnemyUnit)->getHitPoints() + enemy_under_attack_score;
													attack_good = allEnemyUnit;
												}

												if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) > 4)//4마리보다 많으면 그냥가까운애
													if(min_good_attack_unit_score > (*allMyUnit)->getDistance(*allEnemyUnit))
													{
														min_good_attack_unit_score = (*allMyUnit)->getDistance(*allEnemyUnit);
														attack_good = allEnemyUnit;
													}
										}

										//!!!!일단은 그냥 적 아무나 공격하는 것으로 함...!!
										if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) <=4)//4마리보다 적으면 1.4가능
										{
											if(!(*allMyUnit)->isAttacking())
												(*allMyUnit)->attack((*attack_good));
										}

										if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) > 4)//4마리보다 많으면 그냥가까운애
										{
											if(!(*allMyUnit)->isAttacking())
												(*allMyUnit)->attack((*attack_good)->getPosition());

										}

										//--3 공격할 적 선택하고 공격 끝

									}
								}
							}
						}
					}
			}
		}
	}*/
	//----------------------------------------------------------수비부분----------------------------------------------------- 끝





	//테스트.
	static int build_fram213e = Broodwar->getFrameCount();
	if(build_fram213e == Broodwar->getFrameCount()) 
	{
		/////////////적 스타트포지션, 초크포인트,  적 앞마당 포지션 저장///////////
		Position en_start = this->informationManager->getEnemyStartLocation()->getPosition();
		Position en_choke = BWTA::getNearestChokepoint(en_start)->getCenter();
		Position en_expand = BWTA::getNearestBaseLocation(en_choke)->getPosition();
		Position my_start = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
		Position my_choke = BWTA::getNearestChokepoint(my_start)->getCenter();
		Position my_ex1 =  BWTA::getNearestBaseLocation(my_choke)->getPosition();


		//초크 포인트 체인지!

		int new_Choke_X = my_choke.x() + (my_start.x() - my_choke.x()) * 0.12;
		int new_Choke_Y = my_choke.y() + (my_start.y() - my_choke.y()) * 0.12;

		int new_DChoke_X = my_choke.x() + (my_start.x() - my_choke.x()) * 0.17;
		int new_DChoke_Y = my_choke.y() + (my_start.y() - my_choke.y()) * 0.17;

		BWAPI::Position *new_Choke = new Position(new_Choke_X,new_Choke_Y);
		BWAPI::Position *new_DChoke = new Position(new_DChoke_X,new_DChoke_Y);

		Broodwar->drawText(CoordinateType::Map,new_Choke->x(),new_Choke->y(),"%d");



		build_fram213e = build_fram213e+4;   ///전체를 4프레임마다 한번씩 실행하게 했음.  시스템이 너무 무거워서 다운되는 일 때문.    따라서 이 안에서 프레임을 제한해줄때는 4프레임의 배수나 약수로 해줘야함.



		// !!!!!! 주의    !! 프레임 별로 제한했을 때 프레임이 안맞으면 서로 존재하지 않는다고 판단하는 경우가 생김.

		//빌드만들기

		static int build_frame = Broodwar->getFrameCount();
		if(build_frame == Broodwar->getFrameCount())
		{
			build_frame = build_frame + 12 ; //0.5초마다 실행


			/////////////프로토스 추가 빌드 부분.

			//확장해주기부터 시작

			if(Broodwar->enemy()->getRace() == Races::Protoss)
			{

				//cig2013☆☆☆☆☆☆☆☆☆☆적 판단하는 것 시작☆☆☆☆ cig3_fastest_3gate_zealot를 false로 해주기. 기본은 true



				// 1. 빠른러쉬 판단
				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
				{	
					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Gateway)
					{
						if(Protoss_one_Gateway_cig_id == -2)
							Protoss_one_Gateway_cig_id = (*allEnemyUnit)->getID();

						if(Protoss_one_Gateway_cig_id != (*allEnemyUnit)->getID() && Protoss_two_Gateway_cig_id == -2)
							Protoss_two_Gateway_cig_id = (*allEnemyUnit)->getID();


					}


					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Assimilator)
					{
						Protoss_one_gas_cig_id = (*allEnemyUnit)->getID(); 
					}

					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Cybernetics_Core && cig_2013_cyber_timing ==-2)
					{
						//사이버네스틱코어 지어지는 시간 38초 -- 즉 사이버네스틱코어가 지어지기 시작한 시점을 의미함.
						cig_2013_cyber_timing = Broodwar->getFrameCount()/24 - 38 +  (*allEnemyUnit)->getRemainingBuildTime();
					}

					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun && cig_2013_adun_timing ==-2)
					{
						cig_2013_adun_timing = Broodwar->getFrameCount();
					}



				}

				//한번만 들어오도록
				if(cig0 == false && Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >=2)
				{
					//투게이트 이상에 가스가 없으면 빠른러쉬	
					if(Protoss_one_Gateway_cig_id != -2 && Protoss_two_Gateway_cig_id != -2 && Protoss_one_gas_cig_id == -2)
						cig3_fastest_3gate_zealot = true;
					else
						cig3_fastest_3gate_zealot = false;


					if(Protoss_one_Gateway_cig_id == -2 && Protoss_two_Gateway_cig_id == -2 && Protoss_one_gas_cig_id == -2)
						cig3_fastest_3gate_zealot = true; //아무것도 못봤으면 빠른러쉬

					cig0 = true;
				}


				//2. 사이버 속도로 다크판단 + 아둔으로 다크판단.	 지금 (*allEnemyUnit)->getRemainingBuildTime



				if(Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >=2) //이건 프로토스 전체에 해당함. 1게이트 이전에는 onstart에서 해줌.
				{
					this->buildOrderManager->build(100,UnitTypes::Protoss_Scarab,220); //스캐럽뽑기

						if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Robotics_Support_Bay) == 1  && aiide2014_auto_start ==false) //aiide2014 파일런 4개부터 시작
						{

						//발업부터 시작
						for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
						{					

							if((*its)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun)
								(*its)->upgrade(UpgradeTypes::Leg_Enhancements);  //질럿 발업
							
						}
						}

						if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Robotics_Support_Bay) == 1  && aiide2014_auto_start ==false) //aiide2014 파일런 4개부터 시작
						{


						if(aiide2014_once1==false)
						{
							this->buildOrderManager->build(1,UnitTypes::Protoss_Observer,230); 
							this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Reaver,220);
							this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,215);									
							this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Shuttle,210);
							this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Pylon,205);
							this->buildOrderManager->buildAdditional(2,UnitTypes::Protoss_Zealot,200);
							aiide2014_once1=true;						
						}

						}
					
						if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Shuttle) >= 1)
							aiide2014_auto_start=true;

					if(aiide2014_auto_start) //아주 빠른 러쉬일 때
					{
						this->buildOrderManager->build(100,UnitTypes::Protoss_Scarab,850); //스캐럽뽑기

						
						if(dd21 == 0)
						{
							baseManager->expand(190);
							dd21 = 1;
						}

						this->buildOrderManager->build(14,UnitTypes::Protoss_Probe,800); //14
						this->buildOrderManager->build(3,UnitTypes::Protoss_Gateway,785);
						this->buildOrderManager->build(14,UnitTypes::Protoss_Probe,780); //14
						this->buildOrderManager->build(1,UnitTypes::Protoss_Zealot,779); //16
						this->buildOrderManager->build(15,UnitTypes::Protoss_Probe,778); //17
						this->buildOrderManager->build(4,UnitTypes::Protoss_Zealot,776); //23
						this->buildOrderManager->build(2,UnitTypes::Protoss_Pylon,775); 
						this->buildOrderManager->build(17,UnitTypes::Protoss_Probe,774); //25

						this->buildOrderManager->build(7,UnitTypes::Protoss_Zealot,772); 
						this->buildOrderManager->build(3,UnitTypes::Protoss_Pylon,771); 
						this->buildOrderManager->build(19,UnitTypes::Protoss_Probe,770);
						this->buildOrderManager->build(10,UnitTypes::Protoss_Zealot,769);

						//this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,774);

						if(Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe)>=18)
							cig4_fatest_fix_build_end = true;

						if(cig4_fatest_fix_build_end==true)		
							if(Broodwar->self()->supplyTotal()/2!=200)
							{

								if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
								{
									this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

								}

								if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.2 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus))  //6을 더해줫음.
								{
									this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

								}

							}

							//*****아주빠른 러쉬 일 때 오토빌드 시작
							//기본 유닛들+게이트웨이 cig2013

							if(cig4_fatest_fix_build_end)
							{

								//적 사이버 시간에 따라 프로브 더 뽑아주기.
								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 150)
										this->buildOrderManager->build(24,UnitTypes::Protoss_Probe,770);
									else if(cig_2013_cyber_timing <= 180)
										this->buildOrderManager->build(23,UnitTypes::Protoss_Probe,770);
									else if(cig_2013_cyber_timing <= 240)
										this->buildOrderManager->build(23,UnitTypes::Protoss_Probe,770);						
								}



								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 200)
										this->buildOrderManager->build(3,UnitTypes::Protoss_Dragoon,769);

								}

								this->buildOrderManager->build(11,UnitTypes::Protoss_Zealot,760); //질럿 11마리 뽑기. cig임시 
								this->buildOrderManager->build(43,UnitTypes::Protoss_Probe,750);  //앞마당 까지 먹었을 때의 일꾼 수  미네랄당 프로브 수 2.5가 최대이고, 2.3정도가 적정 우린 더 낮게. 



								this->buildOrderManager->build(100,UnitTypes::Protoss_Zealot,700);
								//	this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.7),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.7배 뽑기
								this->buildOrderManager->build(7,UnitTypes::Protoss_Gateway,500);												


								///기본빌드 파일런 시작~~~

								//지금 만드는 파일런 인구수 + 토탈인구수 - 지금 사용중 인구수 <= 지금 생산중인 인구로 해야하지만. (그냥 만들고 있는 갯수 * 1.9로)

								if(Broodwar->self()->supplyTotal()/2!=200)
								{

									if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
									{
										this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

									}

									if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.4 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus) + 6)  //6을 더해줫음.
									{
										this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

									}

								}

								//cig2013 가스가 없으면 아무것도 생산되지 않을수도 주의
								if(Broodwar->self()->gas() > 200)
									this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.6),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.6배 뽑기


								////////////추가빌드.


								/////////////!!!!!!!!!!사이버에 따라서 질럿조절
								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 150)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 3) //질럿이 3마리 이상이면 가스 2개 한번에 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,826);
										}

										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 3) //질럿이 3마리 이상이면 코어 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,825);
										}

									}
									else if(cig_2013_cyber_timing <= 180)
									{	
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 4) //질럿이 5마리 이상이면 가스 2개 한번에 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,826);
										}

										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 4) //질럿이 5마리 이상이면 코어 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,825);
										}

									}
									else if(cig_2013_cyber_timing <= 230)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 7) //질럿이 8마리 이상이면 가스 2개 한번에 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,826);
										}

										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >=7) //질럿이 8마리 이상이면 코어 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,825);
										}
									}

									else if(cig_2013_cyber_timing >230)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 13) //질럿이 13마리 이상이면 가스 2개 한번에 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,826);
										}

										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 13) //질럿이 13마리 이상이면 코어 지음
										{
											this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,825);
										}

									}
								}

								if(cig_2013_cyber_timing==-2)
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 13) //질럿이 13마리 이상이면 가스 2개 한번에 지음
									{
										this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,826);
									}

									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 13) //질럿이 13마리 이상이면 코어 지음
									{
										this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,825);
									}


								}


								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 2) //드라군이 2마리 이상이면 가스 2개지음.
								{
									this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,826);
								}


								//적 아둔 발견했을 시 옵저버
								if(cig_2013_adun_timing!=-2)
								{
									this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,900);
									this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,899);

									if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core) >= 1) //드라군이 1마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
									{

										//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

										if(robotics_1 == false)
										{
											this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,898); //
											robotics_1 = true;
										}


										if(robotics_1 == true)
											this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,897);

										this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,880);   


									}
								}

								//옵저버 조절


								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 150)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 3) //드라군이 1마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
										{

											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}




									}
									else if(cig_2013_cyber_timing <= 180)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 4) //드라군이 4마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
										{

											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}

									}
									else if(cig_2013_cyber_timing <= 230)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 4) //드라군이 4마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
										{

											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}
									}
									else if(cig_2013_cyber_timing > 230)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 5) //드라군이 4마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
										{

											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}
									}
								}


								if(cig_2013_cyber_timing==-2)
								{

									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 5) //드라군이 4마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
									{

										//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

										if(robotics_1 == false)
										{
											this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
											robotics_1 = true;
										}
										this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
									}
								}

								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observatory) >= 1) //옵저버 터리 있으면 옵저버 2마리 만듦
								{
									this->buildOrderManager->build(2,UnitTypes::Protoss_Observer,799);// 옵저버
								}


								if(BWAPI::Broodwar->self()->supplyUsed()/2 > 90)  //인구수가 90이상이면 옵저버 4마리 유지.
									this->buildOrderManager->build(3,UnitTypes::Protoss_Observer,789); //3마리유지



								for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 3) //드라군4마리 이상 있으면 사업해줌
									{
										if((*its)->getType() == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
											(*its)->upgrade(UpgradeTypes::Singularity_Charge);  //드라군 사업

									}


									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 8 && Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 1)  //드라군 9마리 이상 있고, 아둔 있으면 발업 해줌
									{

										if((*its)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun)
											(*its)->upgrade(UpgradeTypes::Leg_Enhancements);  //질럿 발업
									}

								}

								//드라군이 7마리 이상이면 아둔 올림
								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 6) 
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 0)
										this->buildOrderManager->build(1,UnitTypes::Protoss_Citadel_of_Adun,812);  
								}

							}
					}


					//*****아주빠른 러쉬 일 때 오토빌드 끝


					//☆☆☆☆☆☆일반적일 때 오토빌드 시작 ☆☆☆☆☆☆☆//


					if(false) //빠른 러쉬 아닐 때
					{
						this->buildOrderManager->build(14,UnitTypes::Protoss_Probe,800); //14

						this->buildOrderManager->build(2,UnitTypes::Protoss_Gateway,785);
						this->buildOrderManager->build(15,UnitTypes::Protoss_Probe,780); //14
						this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,779); //가스

						this->buildOrderManager->build(1,UnitTypes::Protoss_Zealot,779); //16
						this->buildOrderManager->build(16,UnitTypes::Protoss_Probe,778); //18

						if(cig_2013_cyber_timing!=-2)
						{
							if(cig_2013_cyber_timing <= 200)
							{
								this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,776);
								this->buildOrderManager->build(3,UnitTypes::Protoss_Zealot,775); //21
							}
							else if(cig_2013_cyber_timing > 200)
							{
								this->buildOrderManager->build(3,UnitTypes::Protoss_Zealot,776); //21
								this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,775);
							}

						}
						if(cig_2013_cyber_timing==-2)
						{
							this->buildOrderManager->build(3,UnitTypes::Protoss_Zealot,776); //21
							this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,775);
						}


						this->buildOrderManager->build(18,UnitTypes::Protoss_Probe,774); //23
						this->buildOrderManager->build(5,UnitTypes::Protoss_Zealot,490); //23
						this->buildOrderManager->build(2,UnitTypes::Protoss_Pylon,489); //23


						//this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,774);


						if(Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe)>=18)
							cig4_slow_fix_build_end = true;

						if(cig4_slow_fix_build_end==true)		
							if(Broodwar->self()->supplyTotal()/2!=200)
							{

								if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
								{
									this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

								}

								if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.3 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus))  //6을 더해줫음.
								{
									this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

								}

							}


							//*****아주빠른 러쉬 일 때 오토빌드 시작
							//기본 유닛들+게이트웨이 cig2013

							if(cig4_slow_fix_build_end == true)
							{


								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 160)
									{
										this->buildOrderManager->build(5,UnitTypes::Protoss_Zealot,770);
										this->buildOrderManager->build(24,UnitTypes::Protoss_Probe,769);
										this->buildOrderManager->build(2,UnitTypes::Protoss_Dragoon,768);

									}
									else if(cig_2013_cyber_timing <= 220)	
									{
										this->buildOrderManager->build(6,UnitTypes::Protoss_Zealot,770);
										this->buildOrderManager->build(24,UnitTypes::Protoss_Probe,769);
										this->buildOrderManager->build(3,UnitTypes::Protoss_Dragoon,768);

									}
									else if(cig_2013_cyber_timing > 220)	
									{
										this->buildOrderManager->build(6,UnitTypes::Protoss_Zealot,770);
										this->buildOrderManager->build(24,UnitTypes::Protoss_Probe,769);
										this->buildOrderManager->build(5,UnitTypes::Protoss_Dragoon,768);

									}					
								}

								if(cig_2013_cyber_timing==-2)
								{
									this->buildOrderManager->build(6,UnitTypes::Protoss_Zealot,770);
									this->buildOrderManager->build(24,UnitTypes::Protoss_Probe,769);
									this->buildOrderManager->build(5,UnitTypes::Protoss_Dragoon,768);

								}



								this->buildOrderManager->build(43,UnitTypes::Protoss_Probe,750);  //앞마당 까지 먹었을 때의 일꾼 수  미네랄당 프로브 수 2.5가 최대이고, 2.3정도가 적정 우린 더 낮게. 


								this->buildOrderManager->build(100,UnitTypes::Protoss_Zealot,700);
								//	this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.7),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.7배 뽑기
								this->buildOrderManager->build(8,UnitTypes::Protoss_Gateway,500);												


								///기본빌드 파일런 시작~~~

								//지금 만드는 파일런 인구수 + 토탈인구수 - 지금 사용중 인구수 <= 지금 생산중인 인구로 해야하지만. (그냥 만들고 있는 갯수 * 1.9로)

								if(Broodwar->self()->supplyTotal()/2!=200)
								{

									if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
									{
										this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

									}

									if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.3 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus) + 6)  //6을 더해줫음.
									{
										this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

									}

								}

								//cig2013 가스가 없으면 아무것도 생산되지 않을수도 주의
								if(Broodwar->self()->gas() > 200)
									this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.6),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.6배 뽑기


								////////////추가빌드.


								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 200)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 4) //드라군이 3마리 이상이면 가스 2개 한번에 지음
											this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,826);
									}
									else if(cig_2013_cyber_timing > 200)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 6) //드라군이6마리 이상이면 가스 2개 한번에 지음
											this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,826);
									}

								}
								if(cig_2013_cyber_timing==-2)
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 6) //드라군이6마리 이상이면 가스 2개 한번에 지음
										this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,826);
								}

								//적 아둔 발견했을 시 옵저버
								if(cig_2013_adun_timing!=-2)
								{
									this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,900);
									this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,899);

									if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core) >= 1) //드라군이 1마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음  
									{

										//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

										if(robotics_1 == false)
										{
											this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,898); //
											robotics_1 = true;
										}


										if(robotics_1 == true)
											this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,897);

										this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,880);   


									}
								}


								if(cig_2013_cyber_timing!=-2)
								{
									if(cig_2013_cyber_timing <= 160)
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 4) //드라군이 3마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음
										{
											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}


									}
									else if(cig_2013_cyber_timing <= 220)	
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 4) //드라군이 4마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음
										{
											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}

									}
									else if(cig_2013_cyber_timing > 220)	
									{
										if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 6) //드라군이 6마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음
										{
											//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

											if(robotics_1 == false)
											{
												this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
												robotics_1 = true;
											}
											this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
										}


									}					
								}

								if(cig_2013_cyber_timing==-2)
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 6) //드라군이 6마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음
									{
										//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

										if(robotics_1 == false)
										{
											this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
											robotics_1 = true;
										}
										this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
									}


								}


								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observatory) >= 1) //옵저버 터리 있으면 옵저버 2마리 만듦
								{
									this->buildOrderManager->build(2,UnitTypes::Protoss_Observer,799);// 옵저버
								}


								if(BWAPI::Broodwar->self()->supplyUsed()/2 > 90)  //인구수가 90이상이면 옵저버 4마리 유지.
									this->buildOrderManager->build(3,UnitTypes::Protoss_Observer,789); //3마리유지



								for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 2) //드라군 2마리 이상 있으면 사업해줌
									{
										if((*its)->getType() == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
											(*its)->upgrade(UpgradeTypes::Singularity_Charge);  //드라군 사업

									}


									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 9 && Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 1)  //드라군 9마리 이상 있고, 아둔 있으면 발업 해줌
									{

										if((*its)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun)
											(*its)->upgrade(UpgradeTypes::Leg_Enhancements);  //질럿 발업
									}

								}

								//드라군이 7마리 이상이면 아둔 올림
								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 7) 
								{
									if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 0)
										this->buildOrderManager->build(1,UnitTypes::Protoss_Citadel_of_Adun,812);  //템플러 테크를 로보틱스 테크보다 빨리
								}






							}
					}



					//★★★★★★일반적일 때 오토빌드 끝 ★★★★★★★★★//

				}
			}


			/////////////프로토스 추가 빌드 부분  끝


			//저그 확장해주기부터 시작	



			if(Broodwar->enemy()->getRace() == Races::Zerg)
			{	
				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 11 && !my_expand_true) //다크가 2마리 이상 있고 확장이 없으면 확장해주기
				{
					baseManager->expand(1100);
					my_auto_build = true;
					my_expand_true = true;
					this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,850);
					this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,843);
				}


				///확장해주기 끝

				if(my_auto_build) //나의 오토빌드가 켜져 있어야 실행됨.
				{	

					//빌드 만들기. 멀티 먹은 이후 빌드  테란전은 멀티 먹고 나서 조금 후의 빌드

					//기본빌드.  프로브, 질럿, 드라군, 게이트웨이
					this->buildOrderManager->build(42,UnitTypes::Protoss_Probe,750);  //앞마당 까지 먹었을 때의 일꾼 수  미네랄당 프로브 수 2.5가 최대이고, 2.3정도가 적정 우린 더 낮게. 
					this->buildOrderManager->build(100,UnitTypes::Protoss_Zealot,700);
					this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.7),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.7배 뽑기
					this->buildOrderManager->build(8,UnitTypes::Protoss_Gateway,500);

					/////////////


					///기본빌드 파일런 시작~~~

					//지금 만드는 파일런 인구수 + 토탈인구수 - 지금 사용중 인구수 <= 지금 생산중인 인구로 해야하지만. (그냥 만들고 있는 갯수 * 1.9로)

					if(Broodwar->self()->supplyTotal()/2!=200)
					{
						if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
						{
							this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

						}

						if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.4 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus) + 6)  //5를 더해줫음.
						{
							this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

						}


					}
					///기본빌드 파일런 끝~~~

					//추가빌드.

					if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 1) //드라군이 1마리 이상이면 가스 지음
					{
						this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,826);
					}

					if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 3) //드라군이 4마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음
					{
						this->buildOrderManager->build(1,UnitTypes::Protoss_Citadel_of_Adun,812);
						//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

						if(robotics_1 == false)
						{
							this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
							robotics_1 = true;
						}
						this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
					}



					if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observatory) >= 1) //옵저버 터리 있으면 옵저버 2마리 만듦
					{
						this->buildOrderManager->build(2,UnitTypes::Protoss_Observer,770);// 옵저버
					}


					if(BWAPI::Broodwar->self()->supplyUsed()/2 >80)  //인구수가 80이상이면 옵저버 3마리 유지.
						this->buildOrderManager->build(3,UnitTypes::Protoss_Observer,755);



					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{
						if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 3) //드라군 4마리 이상 있으면 사업해줌
						{
							if((*its)->getType() == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
								(*its)->upgrade(UpgradeTypes::Singularity_Charge);  //드라군 사업

						}


						if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 5)  //드라군 6마리 이상 있으면 발업, 스톰업 해줌
						{
							if((*its)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun)
								(*its)->upgrade(UpgradeTypes::Leg_Enhancements);  //질럿 발업
						}
					}
				}





			}

			///////////저그 빌드 끝

			//////////////!!!!!!테란빌드 시작


			//테란 확장 오토빌드 활성화 타이밍
			if(Broodwar->enemy()->getRace() == Races::Terran)
			{

				//게이트 1개이상일 때가 빌드 활성화타이밍
				if(Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway)>=1)
				{
					this->buildOrderManager->build(13,UnitTypes::Protoss_Probe,800); //14

					this->buildOrderManager->build(2,UnitTypes::Protoss_Gateway,785);
					this->buildOrderManager->build(14,UnitTypes::Protoss_Probe,780); //14
					this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,779); //가스

					this->buildOrderManager->build(1,UnitTypes::Protoss_Zealot,779); //16
					this->buildOrderManager->build(15,UnitTypes::Protoss_Probe,778); //17
					this->buildOrderManager->build(3,UnitTypes::Protoss_Zealot,776); //23

					this->buildOrderManager->build(1,UnitTypes::Protoss_Cybernetics_Core,775); //23


					//this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,774);


					if(Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core) >=1)
						cig5_terran_start_auto_build = true;

					if(cig5_terran_start_auto_build==true)		
						if(Broodwar->self()->supplyTotal()/2!=200)
						{

							if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
							{
								this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

							}

							if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.2 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus))  //6을 더해줫음.
							{
								this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

							}

						}


						//*****아주빠른 러쉬 일 때 오토빌드 시작
						//기본 유닛들+게이트웨이 cig2013

						if(cig5_terran_start_auto_build==true)
						{

							this->buildOrderManager->build(20,UnitTypes::Protoss_Probe,762);
							this->buildOrderManager->build(6,UnitTypes::Protoss_Zealot,761);
							this->buildOrderManager->build(21,UnitTypes::Protoss_Probe,760);
							this->buildOrderManager->build(7,UnitTypes::Protoss_Zealot,759);


							this->buildOrderManager->build(43,UnitTypes::Protoss_Probe,750);  //앞마당 까지 먹었을 때의 일꾼 수  미네랄당 프로브 수 2.5가 최대이고, 2.3정도가 적정 우린 더 낮게. 



							this->buildOrderManager->build(100,UnitTypes::Protoss_Zealot,700);
							//	this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.7),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.7배 뽑기
							this->buildOrderManager->build(8,UnitTypes::Protoss_Gateway,500);												


							///기본빌드 파일런 시작~~~

							//지금 만드는 파일런 인구수 + 토탈인구수 - 지금 사용중 인구수 <= 지금 생산중인 인구로 해야하지만. (그냥 만들고 있는 갯수 * 1.9로)

							if(Broodwar->self()->supplyTotal()/2!=200)
							{

								if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2 <= (Broodwar->self()->incompleteUnitCount(UnitTypes::AllUnits) - Broodwar->self()->incompleteUnitCount(UnitTypes::Buildings)) * 1.9)
								{
									this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000);

								}

								if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Pylon)*8 + Broodwar->self()->supplyTotal()/2 - Broodwar->self()->supplyUsed()/2  < BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) * 2.4 + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus) + 6)  //6을 더해줫음.
								{
									this->buildOrderManager->build(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Pylon) + 1 , UnitTypes::Protoss_Pylon,1000); // 파일런 하나 만들기

								}

							}




							//cig2013 가스가 없으면 아무것도 생산되지 않을수도 주의
							if(Broodwar->self()->gas() > 200)
								this->buildOrderManager->build((int)(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot)*1.6),UnitTypes::Protoss_Dragoon,720); //드라군은 질럿의 1.6배 뽑기




							////////////추가빌드.

							if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 4) //드라군이 2마리 이상이면 가스 2개 한번에 지음
							{
								this->buildOrderManager->build(2,UnitTypes::Protoss_Assimilator,826);
							}







							if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 9) //드라군이 9마리 이상이면  로보틱스, 옵저버터리, 아둔, 템플러 아카이브 지음
							{
								//this->buildOrderManager->build(1,UnitTypes::Protoss_Assimilator,822);

								if(robotics_1 == false)
								{
									this->buildOrderManager->buildAdditional(1,UnitTypes::Protoss_Robotics_Facility,790); //
									robotics_1 = true;
								}
								this->buildOrderManager->build(1,UnitTypes::Protoss_Observatory,780);   
							}


							if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observatory) >= 1) //옵저버 터리 있으면 옵저버 2마리 만듦
							{
								this->buildOrderManager->build(2,UnitTypes::Protoss_Observer,799);// 옵저버
							}


							if(BWAPI::Broodwar->self()->supplyUsed()/2 > 90)  //인구수가 90이상이면 옵저버 4마리 유지.
								this->buildOrderManager->build(3,UnitTypes::Protoss_Observer,789); //3마리유지



							for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
							{
								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 2) //드라군 2마리 이상 있으면 사업해줌
								{
									if((*its)->getType() == BWAPI::UnitTypes::Protoss_Cybernetics_Core)
										(*its)->upgrade(UpgradeTypes::Singularity_Charge);  //드라군 사업

								}


								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 9 && Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 1)  //드라군 10마리 이상 있고, 아둔 있으면 발업 해줌
								{

									if((*its)->getType() == BWAPI::UnitTypes::Protoss_Citadel_of_Adun)
										(*its)->upgrade(UpgradeTypes::Leg_Enhancements);  //질럿 발업
								}

							}

							//드라군이 8마리 이상이면 아둔 올림
							if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 7) 
							{
								if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 0)
									this->buildOrderManager->build(1,UnitTypes::Protoss_Citadel_of_Adun,812);  //템플러 테크를 로보틱스 테크보다 빨리
							}
						}

				}
			}
			////////////테란 빌드 끝
		}

		//////////////////////////////////////////****빌드 끝

		/****총공격하기***////

		/******우리편 자신감시스템 시작 *****/// 123123

		for(int i=0; i<150; i++)  //배열들 초기화
		{
			self_attack_unit_attak_power[i] = 0;
			self_attack_unit_position_x[i] = 0; 
			self_attack_unit_position_y[i] = 0;


			confidence[i] = 0;
			confidence_enemy[i] = 0;
			enemy_attack_unit_position_x[i] = 0;
			enemy_attack_unit_position_y[i] = 0;
			enemy_attack_unit_attak_power[i] = 0;

		}

		//******☆☆☆☆☆우리편과 적 공격유닛 위치, 공격력 저장 시작 ☆☆☆☆☆☆☆
		//int unit_self=0; //임의로 매긴 우리편 공격력 16이상의 유닛번호 0부터시작

		//int unit_s=0;
		int unit_e=0;
		int low_confidence_unit=0; //자신감 낮아서 도망가는 애들 숫자 세주기
		std::set<BWAPI::Unit *>::const_iterator confidence_nice_unit = BWAPI::Broodwar->self()->getUnits().begin(); //자신감 높은 유닛 찾기위함
		for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
		{
			if((*its)->getType().canAttack() && (*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
			{
				////////////적 유닛 주변에 있으면 자신감 하락
				for (std::set<BWAPI::Unit *>::const_iterator enemys = BWAPI::Broodwar->enemy()->getUnits().begin(); enemys != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemys)
				{	

					//우리편 자신감 낮추기.
					//벙커일때는 일단은 뺐음.. 나중에 벙커점수고려하여 생각
					if((*enemys)->getType().canAttack() && (*enemys)->getType().groundWeapon().damageAmount() >= 7 || (*enemys)->getType() == BWAPI::UnitTypes::Terran_Marine || (*enemys)->getType() == BWAPI::UnitTypes::Zerg_Zergling && !((*enemys)->getType() == BWAPI::UnitTypes::Terran_Bunker)) // 데미지가 10보다 크다면 . 공격유닛 선택하기 위함
					{
						if(130>(*its)->getDistance(*enemys))
						{
							confidence[unit_e] = confidence[unit_e] - (double)(*enemys)->getType().groundWeapon().damageAmount()/(*enemys)->getType().groundWeapon().damageCooldown();
							//	unit_e++;
						}
					}
				}
				////////////적 유닛 주변에 있으면 자신감 하락 끝

				//우리 편 유닛 주변에 있으면 자신감 상승
				for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
				{
					if((*its2)->getType().canAttack() && (*its2)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
					{
						if(130>(*its)->getDistance(*its2))
						{
							confidence[unit_e] = confidence[unit_e] + (double)(*its2)->getType().groundWeapon().damageAmount()/(*its2)->getType().groundWeapon().damageCooldown();
							//unit_s++;
						}

					}
				}
				////우리 편 유닛 주변에 있으면 자신감 상승 끝

				//우리편 자신감 출력
				Position q=(*its)->getPosition(); 
				//Broodwar->drawText(CoordinateType::Map,q.x(),q.y(),"%d", (int)(confidence[unit_e]*100));

				//// 누가 자신감이 큰지 찾기 시작///
				int confidence_max=0;
				for(int i=0; i<150 ;i++)
				{
					if(confidence[i] >= confidence_max)
						confidence_max = i;
				}

				////누가 자신감이 큰지 찾기 끝///

				int search_confidence_nice_unit_num=0;
				for (std::set<BWAPI::Unit *>::const_iterator its3 = BWAPI::Broodwar->self()->getUnits().begin(); its3 != BWAPI::Broodwar->self()->getUnits().end(); ++its3)
				{			
					if((*its3)->getType().groundWeapon().damageAmount() >= 8 && (*its3)->getType().canAttack()) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
					{
						if(search_confidence_nice_unit_num == confidence_max)
							confidence_nice_unit = its3;
						search_confidence_nice_unit_num++;
					}

				}


				if(Broodwar->enemy()->getRace() == Races::Zerg)//저그전의 경우 -180보다 자신감이 작으면 도망감
				{
					if((int)(confidence[unit_e]*100) < -200)
					{
						low_confidence_unit++; //자신감 낮은애들 숫자 ++
						run_away[unit_e]=true;

					}
				}

				if(Broodwar->enemy()->getRace() == Races::Terran)//테란전의의 경우 -200보다 자신감이 작으면 도망감
				{
					if((int)(confidence[unit_e]*100) < -200)
					{
						low_confidence_unit++; //자신감 낮은애들 숫자 ++
						run_away[unit_e]=true;

					}
				}

				if(Broodwar->enemy()->getRace() == Races::Protoss)//플토전의 경우 -120보다 자신감이 작으면 도망감
				{
					if((int)(confidence[unit_e]*100) < -120)
					{
						low_confidence_unit++; //자신감 낮은애들 숫자 ++
						run_away[unit_e]=true;

					}
				}
				unit_e++;
			}

		}


		///애들 도망가게 하기..  단 5마리 이상 도망가게 설정되면 도망가지 않는다.
		unit_e = 0;  //0으로 초기화시켜줘야한다.
		for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
		{
			if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
			{

				if(Broodwar->enemy()->getRace() == Races::Zerg)//저그전의 경우 -200보다 자신감이 작으면 도망감
				{
					if((int)(confidence[unit_e]*100) < -200 && low_confidence_unit <=2)  //3마리 이상이면 후퇴안함.. 나중에 수정
					{
						Position q=(*its)->getPosition(); 
						//Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Red,false);
						BWAPI::Position *confidence_nice_position = new Position(0,0);
						*confidence_nice_position = (*confidence_nice_unit)->getPosition();
						//(*its)->move(*confidence_nice_position);
					}

					/*
					if(!((int)(confidence[unit_e]*100) < -200 && low_confidence_unit <=4) && run_away[unit_e] == true) // 이젠 자신감이 부족하지도 않은데 애가 지금 도망가는 중이라면 다시 공격하게 해라.
					{
					(*its)->attack(this->informationManager->getEnemyStartLocation()->getPosition());
					run_away[unit_e]=false; //도망 false
					}
					*/

				}


				if(Broodwar->enemy()->getRace() == Races::Terran)//테란전의 경우 -200보다 자신감이 작으면 도망감
				{
					if((int)(confidence[unit_e]*100) < -200 && low_confidence_unit <=3)  //4마리 이상이면 선택안됨. 나중에 수정
					{
						Position q=(*its)->getPosition(); 
						//	Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Red,false);
						BWAPI::Position *confidence_nice_position = new Position(0,0);
						*confidence_nice_position = (*confidence_nice_unit)->getPosition();
						//	(*its)->move(*confidence_nice_position);
					}
				}

				if(Broodwar->enemy()->getRace() == Races::Protoss)//프로토스전의 경우 -120보다 자신감이 작으면 도망감
				{
					if((int)(confidence[unit_e]*100) < -120 && low_confidence_unit <=3)  //4마리 이상이면 선택안됨. 나중에 수정
					{
						Position q=(*its)->getPosition(); 
						//	Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Red,false);
						BWAPI::Position *confidence_nice_position = new Position(0,0);
						*confidence_nice_position = (*confidence_nice_unit)->getPosition();
						//(*its)->move(*confidence_nice_position);
					}
				}



				//if(Broodwar->enemy()->getRace() == Races::Zerg)//저그전의 경우 -200보다 자신감이 작으면 도망감
				unit_e++;

			}
		}


		/////************우리편 자신감시스템끝****///////////


		//////****적편 자신감시스템 + 하이템플러공격 시작***////


		int psionic_storm_attackedX[100] = {0,}; //하이템플러에게 싸이오닉 스톰을 쏘라고 명령 내린 지점 선택
		int psionic_storm_attackedY[100] = {0,};
		int psionic_storm_num=0; //사이오닉스톰을 쏘라고 한 횟수  

		//하이템플러 셔틀 컨트롤에 필요한 변수들
		bool its_right_click_enemys = false;   
		bool its_unload_its2 = false;
		std::set<BWAPI::Unit *>::const_iterator its_right_click_enemys_unit = BWAPI::Broodwar->enemy()->getUnits().begin();
		////

		unit_e=0; //0으로 초기화해줘야함.
		std::set<BWAPI::Unit *>::const_iterator confidence_nice_enemy_unit = BWAPI::Broodwar->enemy()->getUnits().begin(); //적편 자신감 높은 유닛 찾기위함
		int confidence_nice_enemy_unit_confidence_num=0;
		//4프레임에 한번씩 돌아가도록
		static int fram3__2 = Broodwar->getFrameCount();
		if(fram3__2 == Broodwar->getFrameCount())
		{
			fram3__2=fram3__2 + 4;
			for (std::set<BWAPI::Unit *>::const_iterator enemys = BWAPI::Broodwar->enemy()->getUnits().begin(); enemys != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemys)
			{
				if((*enemys)->getType().groundWeapon().damageAmount() >= 5 && (*enemys)->getType().canAttack() && (*enemys)->getType().canMove() &&  (*enemys)->getType() != BWAPI::UnitTypes::Protoss_Probe && (*enemys)->getType() != BWAPI::UnitTypes::Zerg_Drone && (*enemys)->getType() != BWAPI::UnitTypes::Terran_SCV) // 데미지가 5보다 크다면...... 유닛만 선택됨. 일꾼 제외
				{
					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{

						//적편 자신감 낮추기.
						//벙커일때는 일단은 뺐음.. 나중에 벙커점수고려하여 생각
						if((*its)->getType().canAttack() && (*its)->getType().groundWeapon().damageAmount() >= 7) // 데미지가 10보다 크다면 . 공격유닛 선택하기 위함
						{
							if(130>(*enemys)->getDistance(*its))
							{
								confidence_enemy[unit_e] = confidence_enemy[unit_e] - (double)(*its)->getType().groundWeapon().damageAmount()/(*its)->getType().groundWeapon().damageCooldown();
								//	unit_e++;
							}
						}
					}
					///적편 자신감 낮추기 끝



					//적편 유닛 주변에 적이 있으면 자신감 상승 시작   - 건물 빼고 유닛만 계산해줬음.
					for (std::set<BWAPI::Unit *>::const_iterator enemys2 = BWAPI::Broodwar->enemy()->getUnits().begin(); enemys2 != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemys2)
					{
						if((*enemys2)->getType().canAttack() && (*enemys2)->getType().groundWeapon().damageAmount() >= 5 && (*enemys2)->getType().canMove() && (*enemys2)->getType() != BWAPI::UnitTypes::Protoss_Probe && (*enemys2)->getType() != BWAPI::UnitTypes::Zerg_Drone && (*enemys2)->getType() != BWAPI::UnitTypes::Terran_SCV) // 데미지가 5보다 크다면
						{
							if(130>(*enemys)->getDistance(*enemys2))
							{
								confidence_enemy[unit_e] = confidence_enemy[unit_e] + (double)(*enemys2)->getType().groundWeapon().damageAmount()/(*enemys2)->getType().groundWeapon().damageCooldown();
								//unit_s++;
							}

						}
					}

					//적편 유닛 주변에 적이 있으면 자신감 상승 끝


					////적편 누가 자신감이 큰지 찾기 시작///
					int confidence_max=0;
					for(int i=0; i<150 ;i++)
					{
						if(confidence_enemy[i] >= confidence_max)
							confidence_max = i;
					}

					////적편 누가 자신감이 큰지 찾기 끝///

					int search_confidence_nice_unit_num=0;
					for (std::set<BWAPI::Unit *>::const_iterator enemys2 = BWAPI::Broodwar->enemy()->getUnits().begin(); enemys2 != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemys2)
					{			
						if((*enemys2)->getType().groundWeapon().damageAmount() >= 5 && (*enemys2)->getType().canAttack() && (*enemys2)->getType().canMove() &&  (*enemys2)->getType() != BWAPI::UnitTypes::Protoss_Probe && (*enemys2)->getType() != BWAPI::UnitTypes::Zerg_Drone && (*enemys2)->getType() != BWAPI::UnitTypes::Terran_SCV) // 데미지가 5보다 크다면...... 유닛만 선택됨. 일꾼 제외
						{
							if(search_confidence_nice_unit_num == confidence_max)
							{
								confidence_nice_enemy_unit_confidence_num = confidence_max;
								confidence_nice_enemy_unit = enemys2;
							}
							search_confidence_nice_unit_num++;
						}

					}

					unit_e++; //unit숫자 1씩 증가시켜주기.

				}
			}
		}

		/////****적 자신감 가장 높은애한테 쏘기

		int enemy_num121=0;
		for (std::set<BWAPI::Unit *>::const_iterator enemys2 = BWAPI::Broodwar->enemy()->getUnits().begin(); enemys2 != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemys2)
		{
			enemy_num121++; //적 몇명인지.
		}


		std::set<BWAPI::Unit *>::const_iterator min_my_attack_unit_en_expand_unit = BWAPI::Broodwar->self()->getUnits().end(); // 적 앞마당과 가장 가까운 우리 공격유닛 
		int min_my_attack_unit_en_expand = 100000;  

		///////////////적 앞마당과 가장 가까운 공격유닛 선택////////////
		static int frame__2 = Broodwar->getFrameCount();
		if(frame__2 == Broodwar->getFrameCount())
		{
			frame__2 = frame__2 + 4;  //4프레임마다 실행되게.
			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{	
				if((*its)->isCompleted() && (*its)->getType().groundWeapon().damageAmount() >= 5 && (*its)->getType().canAttack() && (*its)->getType().canMove() &&  (*its)->getType() != BWAPI::UnitTypes::Protoss_Probe && (*its)->getType() != BWAPI::UnitTypes::Zerg_Drone && (*its)->getType() != BWAPI::UnitTypes::Terran_SCV) // 데미지가 5보다 크다면...... 유닛만 선택됨. 일꾼 제외
				{
					if((*its)->getDistance(en_expand)< min_my_attack_unit_en_expand)
					{
						min_my_attack_unit_en_expand = (*its)->getDistance(en_expand);
						min_my_attack_unit_en_expand_unit = its;
					}

				}
			}

		}

		//** 총공격일 때 하이템플러 셔틀에서 내리기 컨트롤 2 시작

	
		int shuttle_loaded_num=0;
		bool enemy_shuttle_too_close=false;


		
		static int frame___44 = Broodwar->getFrameCount();
		if(frame___44 == Broodwar->getFrameCount())
		{
			frame___44 = frame___44+4 ; //4프레임 마다 실행
			//if(total_attack) //총공격이면 
			if(high_templer_loaded_complete) //하이템플러 4마리 모두 탔으면 셔틀 컨트롤 시작.
			{
				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
				{
					if((*its)->getType() == BWAPI::UnitTypes::Protoss_Shuttle) //셔틀선택
					{

						//변수들 설정
						for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
							{
								if((*its2)->isLoaded())
									shuttle_loaded_num++;
							}

						for (std::set<BWAPI::Unit *>::const_iterator enemy2 = BWAPI::Broodwar->enemy()->getUnits().begin(); enemy2 != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemy2)
							{
								//7.31추가코드, 옵저버면 내리면 안됨..
								if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer)
									continue;

								if((*its)->getDistance(*enemy2) <= 270)
									enemy_shuttle_too_close = true;							
							}


						
						//1. 리버 타고 있지 않으면
						if(shuttle_loaded_num == 0)
						{	
							//리버 위로 이동
							for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
							{
								if((*its2)->getType() == BWAPI::UnitTypes::Protoss_Reaver) 
									(*its)->move((*its2)->getPosition());						
							}

							//적군이 가까이 있지 않으면 태움.
								for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
							{
								if((*its2)->getType() == BWAPI::UnitTypes::Protoss_Reaver && enemy_shuttle_too_close ==false) 
									(*its)->load(*its2);				
							}						
						
						}

						//2. 리버 타고 있으면
						if(shuttle_loaded_num == 1)
						{
							//질럿 따라감
							(*its)->move((*min_my_attack_unit_en_expand_unit)->getPosition());
							
							//적군 가까이 있으면 리버 내림
							if(enemy_shuttle_too_close == true)
							{
							
							for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
							{
								if((*its2)->getType() == BWAPI::UnitTypes::Protoss_Reaver) 
							{
								(*its)->unload(*its2);
								}
							}
							}
						}
					
						if((*its)->getShields()  == 0)
						{
								
							for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
							{
								if((*its2)->getType() == BWAPI::UnitTypes::Protoss_Reaver) 
							{
								(*its)->unload(*its2);
							}
							}

						
						}
							

					}
				}
			}

		}

		//** 총공격일 때 하이템플러 셔틀에서 내리기 컨트롤  2 끝


		/////***옵저버컨트롤 시작.***//////
		static int frame_obob=Broodwar->getFrameCount();
		if(frame_obob==Broodwar->getFrameCount())
		{
			frame_obob = frame_obob+4; //4프레임 마다 실행

			int observer_total = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Observer);
			//int *total_array_ob = new int[observer_total];
			int count_i=0;


			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{ //자신감높은 유닛 따라다니는 옵저버 used_ob[0] 과 적 확장기지 정찰가는 옵저버 used_ob[1]의 아이디 저장.
				if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer)
				{
					if(ob_bool == false)
					{
						used_ob[0]=(*its)->getID();
						ob_bool = true;
						//Broodwar->drawTextScreen(420,10,"%s %d","used_ob[0] : ", used_ob[0]);
						break;
					}
					if(ob_bool2 == false)
					{
						if((*its)->getID() != used_ob[0])
						{
							used_ob[1]=(*its)->getID();
							ob_bool2 = true;
							//Broodwar->drawTextScreen(420,20,"%s %d","used_ob[1] : ", used_ob[1]);
							break;
						}
						else
							break;
					}
				}
			}

			count_i=0;
			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{ //기 선택된 옵저버가 죽었는지 살았는지 아이디로 검색, 아이디 없으면 죽은것으로 판단함. 자신감 따라다니는 옵저버
				if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer)
				{	
					count_i++;
					if((*its)->getID() == used_ob[0]) 
						break;
					else if((*its)->getID() != used_ob[0] && count_i == observer_total)
					{
						ob_bool = false;
						break;
					}

				}
			}

			count_i=0;
			if(ob_3_expand < 3)
			{
				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
				{
					if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer)
					{	
						count_i++;
						if((*its)->getID() == used_ob[1]) 
							break;
						else if((*its)->getID() != used_ob[1] && count_i == observer_total && observer_total != 1) // 여기가 죽었다고 판단하는 부분
						{
							ob_bool2 = false; // 아이디를 새로 받기 위해
							ob_3_expand++;
							count_i=0;
							break;
						}
					}
				}
			}
			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{ // 자신감 높은 유닛에게 옵저버 따라 다니기
				if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer && (*its)->getID() == used_ob[0])
				{
					(*its)->move((*confidence_nice_unit)->getPosition());
					//BWAPI::Broodwar->drawLineMap((*its)->getPosition().x(), (*its)->getPosition().y(), (*confidence_nice_unit)->getPosition().x(), (*confidence_nice_unit)->getPosition().y(), BWAPI::Colors::Red);
					//Broodwar->drawTextScreen(420,80,"%s %d","confidence",used_ob[1]);
					break;
				}	
			}

			Position en_start = this->informationManager->getEnemyStartLocation()->getPosition();
			Position en_choke = BWTA::getNearestChokepoint(en_start)->getCenter();
			Position en_expand = BWTA::getNearestBaseLocation(en_choke)->getPosition();

			if(ob_3_expand < 3)// 옵저버 3마리까지만 적 확장기지 정찰하러 보냄. 무한정 가는것을 방지 하기 위하여
			{
				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
				{  
					if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer && (*its)->getID() == used_ob[1]) 
					{
						(*its)->rightClick(en_expand);
						//Broodwar->drawTextScreen(420,70,"%s %d","expand", used_ob[1]);
						break;
					}
				}
			}

			std::set<BWAPI::Unit *>::const_iterator search_save = BWAPI::Broodwar->self()->getUnits().begin(); //공격받고 있는 유닛을 begin부터 검색하는데 가장 최근까지 검색한 유닛 저장
			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its) // 옵저버검색
			{
				if((*its)->getType() == BWAPI::UnitTypes::Protoss_Observer && (*its)->getID() != used_ob[0] && (*its)->getID() != used_ob[1]) // 옵저버인지 판단하고, 자신감 높은 유닛을 이미 쫓고있는 옵저버는 아니어야한다.
				{
					for (std::set<BWAPI::Unit *>::const_iterator itss = search_save; itss != BWAPI::Broodwar->self()->getUnits().end(); ++itss) // 모든 유닛 검색
					{
						search_save++;
						if((*itss)->isUnderAttack() && !((*itss)->getType() == BWAPI::UnitTypes::Protoss_Observer)) // 검색된 유닛이 공격받고 있고 옵저버가 아니면
						{
							Position q=(*itss)->getPosition(); // 포지션 받아와서
							(*its)->rightClick(q); // 이동
							//Broodwar->drawTextScreen(420,90,"%s","attack");
							break;	
						}
					}

				}		
			}	

			for (std::set<BWAPI::Unit *>::const_iterator en = Broodwar->enemy()->getUnits().begin(); en != BWAPI::Broodwar->enemy()->getUnits().end(); ++en)
			{
				if((*en)->getType() == BWAPI::UnitTypes::Protoss_Nexus || (*en)->getType() == BWAPI::UnitTypes::Terran_Command_Center || (*en)->getType() == BWAPI::UnitTypes::Zerg_Hatchery)
				{
					if((*en)->getDistance(en_expand) < 40)
					{
						en_expand_exist = true;
						break;
					}
				}
			}
		}
		////////////////옵저버 끝

		//프로토스전만 //
		if(Broodwar->enemy()->getRace() == Races::Protoss)
		{
			//정찰가는 프로브 아이디 받아오기  --프로토스전만--
			if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe) == 9 && cig1 == false)
			{
				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
				{ 
					if((*its)->getDistance(my_start) > 300 && (*its)->getType() == BWAPI::UnitTypes::Protoss_Probe)
					{
						getCIG2013_scout_probe_ID = (*its)->getID();

						cig1 = true;//한번만 받기위함.
					}


				}
			}

			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{ 
				if((*its)->getID() == getCIG2013_scout_probe_ID)
				{
					//BWAPI::Broodwar->drawCircleMap((*its)->getPosition().x(),(*its)->getPosition().y(), 6, BWAPI::Colors::Yellow);
				}
			}



			if(cig2 == false)
			{
				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
				{ 
					if(cig1=true && (*its)->getType() == BWAPI::UnitTypes::Protoss_Probe && (*its)->getID() == getCIG2013_scout_probe_ID && Broodwar->getFrameCount() < 24*300) //5분 이내일때만
					{
						if((*its)->isGatheringMinerals())
						{
							(*its)->rightClick(this->informationManager->getEnemyStartLocation()->getPosition());
							cig2=true;
						}

					}

				}
			}


			//-----------2인용맵일 때 프로토스전 정찰가기---시작--------// 나중에 시간되면 해주기

			if(BWAPI::Broodwar->getStartLocations().size() ==2)
			{

				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe) == 9 && cig2013_11 == false) {

					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{ 
						if((*its)->getType() == BWAPI::UnitTypes::Protoss_Probe && (*its)->isCompleted())
						{
							getCIG2013_scout_probe_ID = (*its)->getID();
							(*its)->rightClick(this->informationManager->getEnemyStartLocation()->getPosition()); 
							cig2013_11 = true;
							break;				

						}
					}
				}

				for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
				{ 

					if(getFirst_scout_probe_ID == (*its)->getID())
						if((*its)->isConstructing())
							getFirst_scout_probe_ID = -2;


				}
			}


			//-----------2인용맵일 때 프로토스전 정찰가기---끝--------//



		}


		////----------------------한번만 실행되게 .. 정찰프로브 온리보크 시켜주기  

		if(first_probe_onrevoke)
		{

			for (std::set<BWAPI::Unit *>::const_iterator en = Broodwar->enemy()->getUnits().begin(); en != BWAPI::Broodwar->enemy()->getUnits().end(); ++en)
			{ 
				if ((*en)->getType() == BWAPI::UnitTypes::Protoss_Nexus || (*en)->getType() == BWAPI::UnitTypes::Terran_Command_Center || (*en)->getType() == BWAPI::UnitTypes::Zerg_Hatchery)
				{
					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{ 
						if((*en)->getDistance(*its)<30)
						{
							std::set<BWAPI::Unit *>::const_iterator probe = its;
							//this->workerManager->onRevoke(*probe,2);

							//BWAPI::Broodwar->self()->getUnits().
							if ((*its)->getType() == BWAPI::UnitTypes::Protoss_Probe)
							{
								std::set<BWAPI::Unit *>::const_iterator probe = its;
								//Position *d = new Position(3310,3530);
								//BWAPI::Position pos = this->informationManager->get

								getFirst_scout_probe_ID = (*probe)->getID();
								this->workerManager->onRevoke(*probe,2);
								//(*probe)->
							}


						}

					}
				}
			}
		}

		// 일꾼돌리기

		double ww=0;
		double bvx[20] = {0,};
		double bvy[20] = {0,};
		int countx = 0;
		static double enemy_base_loc_x = 0;
		static double enemy_base_loc_y = 0;
		std::set<BWAPI::Unit *>::const_iterator a = BWAPI::Broodwar->self()->getUnits().begin();
		for (std::set<BWAPI::Unit *>::const_iterator en = Broodwar->enemy()->getUnits().begin(); en != BWAPI::Broodwar->enemy()->getUnits().end(); ++en)
		{
			//전체 우리 유닛 선택
			for (std::set<BWAPI::Unit *>::const_iterator itss = BWAPI::Broodwar->self()->getUnits().begin(); itss != BWAPI::Broodwar->self()->getUnits().end(); ++itss)
			{

				//Broodwar->drawTextScreen(420,150,"first  %d,  after %d", getFirst_scout_probe_ID, (*itss)->getID());
				if((*itss)->getID() == getFirst_scout_probe_ID) //아이디가 기존의 정찰프로브와 같다면
				{



					//if(*en)->getType() == BWAPI::UnitTypes::Protoss_Nexus)
					if(!((*en)->getType().canMove()) && !((*en)->getType().isInvincible())) //적 건물 받아오기.   && 뒤는 무적일 때(죽, 미네랄이나 가스를 의미함)
					{
						//건물과 프로브 포지션 받기,  
						BWAPI::Position builded = (*en)->getPosition();
						BWAPI::Position probe = (*itss)->getPosition();
						//if((*en)->getDistance(*itss)<180)

						a = itss;

						//애는 80도로한것
						Position *bv = new Position((builded.x()-probe.x())*0.17365 - (builded.y()-probe.y()) * (-0.9848), (builded.x()-probe.x()) * (-0.9848) + 0.17365 * (builded.y()-probe.y()));
						//Broodwar->drawTextScreen(420,60,"bvxx, bvyy : %d, %d", (*bv).x(), (*bv).y());


						if(this->informationManager->getEnemyStartLocation()->getPosition().x() == builded.x() && this->informationManager->getEnemyStartLocation()->getPosition().y() == builded.y())
						{

						}
						else
						{
							bvx[countx] = (*bv).x();
							bvy[countx] = (*bv).y();
							countx++;

						}
					}
				}
			}
		}

		for (std::set<BWAPI::Unit *>::const_iterator a = BWAPI::Broodwar->self()->getUnits().begin(); a != BWAPI::Broodwar->self()->getUnits().end(); ++a)
		{



			if((*a)->getID() == getFirst_scout_probe_ID) //아이디가 기존의 정찰프로브와 같다면
			{
				scout_start = true;			

				//BWAPI::Broodwar->printf("first  %d,  after %d", getFirst_scout_probe_ID, (*a)->getID());

				//Broodwar->drawTextScreen(420,150,"first  %d,  after %d", getFirst_scout_probe_ID, (*a)->getID());
				//프로브포지션
				BWAPI::Position probe2 = (*a)->getPosition();
				//넥서스 건물 스태틱
				//enemy_base_loc_x = this->informationManager->getEnemyStartLocation()->getPosition().y() - probe2.y() ;
				//	enemy_base_loc_y = -(this->informationManager->getEnemyStartLocation()->getPosition().x() - probe2.x());

				//애는 80도로한것
				enemy_base_loc_x = (this->informationManager->getEnemyStartLocation()->getPosition().x() - probe2.x()) * 0.17365 -  (this->informationManager->getEnemyStartLocation()->getPosition().y() - probe2.y()) * (-0.9848)  ;
				enemy_base_loc_y = (this->informationManager->getEnemyStartLocation()->getPosition().x() - probe2.x()) *  (-0.9848) + 0.17365 * (this->informationManager->getEnemyStartLocation()->getPosition().y() - probe2.y()) ;

				//애는 82도로 한것
				//enemy_base_loc_x = (this->informationManager->getEnemyStartLocation()->getPosition().x() - probe2.x()) * 0.13917 -  (this->informationManager->getEnemyStartLocation()->getPosition().y() - probe2.y()) * (-0.99027)  ;
				//enemy_base_loc_y = (this->informationManager->getEnemyStartLocation()->getPosition().x() - probe2.x()) *  (-0.99027) + 0.13917 * (this->informationManager->getEnemyStartLocation()->getPosition().y() - probe2.y()) ;



				// 빌딩벡터 합함
				double bvxx = 0;
				double bvyy = 0;

				for(int l=0 ; l<20 ; l++)
				{
					bvxx += bvx[l];
					bvyy += bvy[l];
				}

				bvxx += enemy_base_loc_x;
				bvyy += enemy_base_loc_y;
				//Broodwar->drawTextScreen(420,90,"enemy_startlocation x , y : %d, %d", this->informationManager->getEnemyStartLocation()->getPosition().x(), this->informationManager->getEnemyStartLocation()->getPosition().y());
				//Broodwar->drawTextScreen(420,80,"enemy_base x , y : %d, %d", enemy_base_loc_x, enemy_base_loc_y);
				int test1=0;
				int test2=0;
				test1 = (bvxx * ww) * 1/100;
				test2 = (bvyy * ww) * 1/100;
				//bvx[l] = bvx[l]*300/sqrt(bvx[l] * bvx[l] + bvy[l] * bvy[l]);
				//bvy[0] = bvy[l]*300/sqrt(bvx[l] * bvx[l] + bvy[l] * bvy[l]);
				bvxx = bvxx * 300/sqrt(bvxx * bvxx + bvyy * bvyy);
				bvyy = bvyy * 300/sqrt(bvxx * bvxx + bvyy * bvyy);

				//Position *bvv = new Position(bvxx ,bvyy);
				Position *bvv = new Position(bvxx + probe2.x(), bvyy + probe2.y());

				//12프레임마다 움직이게
				static int frametest=Broodwar->getFrameCount();   //24프레임 = 1초

				if(Broodwar->getFrameCount() == frametest) 
				{
					//프로브가 멀리 나가지 않도록 프로브가 무브 클릭하는곳 보정(무브 클릭할 곳과 프로브위치와의 내분) 1:3  무브클릭할 쪽으로 더 많이.
					//Position *bvvv = new Position((3*(*bvv).x() + (*a)->getPosition().x())/4 , (3*(*bvv).y() + (*a)->getPosition().y())/2 );


					//	BWAPI::Broodwar->drawCircleMap((*bvv).x(),(*bvv).y(), 6, BWAPI::Colors::Yellow); //노란색 원으로 프로브 찍는곳 표시
					//BWAPI::Broodwar->printf("first  %d,  after %d", getFirst_scout_probe_ID, (*a)->getID());

					//Broodwar->drawTextScreen(10,50,"frametest : %d", frametest);


					(*a)->rightClick(*bvv);	
					frametest = frametest + 12;
					//	Broodwar->drawTextScreen(420,50,"frametest : %d", frametest);

				}

			}
		}


		//////////////////////////////////////////////////////////////방어시작

		//static int frametest2 = Broodwar->getFrameCount();
		//	if(Broodwar->getFrameCount() == frametest2) 
		//	{
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
			frametest_defens = frametest_defens + 8;


			if(!total_attack)  // 총공격이 아닐 때만 방어 시스템 작동 
			{
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

					if((*allEnemyUnit)->getDistance(pos1241) < 550 || distEnemyNexus < 340 || (*allEnemyUnit)->getDistance(*new_Choke) < 180)
					{
						for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
						{
							//cig2013  -- 초반에 질럿vs 질럿 전투이면서 적과 120거리 미만인에들은 방어시스템 적용 x 어짜피, 질럿vs 질럿에서 공격해줌.  -- 시작 --

							if(Broodwar->enemy()->getRace() == Races::Protoss)
							{
								bool right_combat_cig=false;
								for(int ii=0;ii<20;ii++)
								{
									if((*allMyUnit)->getID() == Start_Combat_GetId[ii])
										right_combat_cig=true;
								}

								if(right_combat_cig == true)
									continue;
							}

							//cig2013  -- 초반에 질럿vs 질럿 전투이면서 적과 120거리 미만인에들은 방어시스템 적용 x 어짜피, 질럿vs 질럿에서 공격해줌.  -- 끝 --


							if(((double)(*allMyUnit)->getType().groundWeapon().damageAmount() > 3 && (*allMyUnit)->isCompleted() && (*allMyUnit)->getType().canMove() && !((*allEnemyUnit)->BWAPI::UnitTypes::Zerg_Overlord)) || (*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Reaver)
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
									//무조건 어택하는듯하여 우선 주석처리, 7월31일
									//(*allMyUnit)->attack((*allEnemyUnit)->getPosition());
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
					}
				}

			}

		}

		/////////////////방어 끝


		int zeal_nn=0;
		for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
		{
			if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*allMyUnit)->isCompleted())
			{
				zeal_nn++;

			}
		}

		if(!firsss21)
		{
			firsss21 = true;
			for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
			{
				if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe)
				{
					if((*allMyUnit)->getDistance(my_start) > 800)
						search_prob_id = (*allMyUnit)->getID();
				}
			}
		}


		/////////////일꾼 vs 일꾼 방어. 일꾼 자신이 공격당할 때마다 대응공격함.

		if(zeal_nn == 0)
		{
			static int frametest_11 = Broodwar->getFrameCount();   //24프레임 = 1초
			if(frametest_11 == Broodwar->getFrameCount())
			{
				frametest_11 = frametest_11 + 20; //20프레임 마다 공격

				enemy_search_unit = 0;
				////정찰온 적 일꾼 수 세기.enemy_search_unit;
				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit !=BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
				{
					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_SCV || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Zerg_Drone)
					{
						if((*allEnemyUnit)->getDistance(my_start)<400)
						{
							enemy_search_unit++;
						}
					}
				}


				///정찰이 완료되었느냐?
				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit !=BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit) 
				{
					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Nexus || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Zerg_Hatchery || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_Command_Center)
						scout_complete2 = true;

				}


				static int frametest_13 = Broodwar->getFrameCount();   //24프레임 = 1초   
				if(frametest_13 == Broodwar->getFrameCount())
				{
					frametest_13 = frametest_13 + 60; // 2.7초정도 마다 공격. 

					for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
					{
						if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe && getFirst_scout_probe_ID != (*allMyUnit)->getID() && search_prob_id != (*allMyUnit)->getID() && getCIG2013_scout_probe_ID != (*allMyUnit)->getID()) //정찰유닛과 아이디가 같지 않다면
						{
							if(enemy_attack_first1 == false || probe_attack_id ==  (*allMyUnit)->getID())  // 처음이거나 일꾼의 아이디가 처음에 선택했던 유닛과 같다면
							{
								probe_attack_id = (*allMyUnit)->getID(); //처음에 바뀌고 이후로는 같은 아이디만 들어올 수 있음.
								enemy_attack_first1 = true;
								if((*allMyUnit)->getDistance(my_start)<350) 
								{

									for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit)
										if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_SCV || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Zerg_Drone)
										{
											if((*allEnemyUnit)->getDistance(my_start)<350)
											{
												(*allMyUnit)->rightClick(*allEnemyUnit);
												break;
											}

										}

								}

								// 거리가 760보다 멀어지면 프로브를 내 스타팅 포인트쪽으로 이동시킴
								if((*allMyUnit)->getDistance(my_start)>760)
								{
									(*allMyUnit)->move(my_start); 
								}
								
							}
						}
					}

				}

				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) <= 2)
				{
					if(enemy_search_unit>=2 || Broodwar->self()->deadUnitCount(BWAPI::UnitTypes::Protoss_Probe) >= 1) //적 일꾼 정찰이 2마리 이상이면

					{
						for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit !=BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
						{
							if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe)
							{
								if((*allMyUnit)->getDistance(my_start)<450)
								{
									if((*allMyUnit)->getShields() < 10)
									{
										(*allMyUnit)->attack((*allMyUnit)->getPosition());
									}
									// 프로브 이동
									if((*allMyUnit)->getDistance(my_start)>760)
									{
										(*allMyUnit)->move(my_start); 
									}

								}

							}
						}
					}
				}


			}
		}

		///////////////////////////////방어 완전  끝


		//하이템플러 태우기

		if(!high_templer_loaded_complete)
		{
			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{
				if((*its)->getType() == BWAPI::UnitTypes::Protoss_Shuttle) //셔틀선택
				{
					for (std::set<BWAPI::Unit *>::const_iterator its2 = BWAPI::Broodwar->self()->getUnits().begin(); its2 != BWAPI::Broodwar->self()->getUnits().end(); ++its2)
					{
						if((*its2)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
							(*its)->load(*its2);

					}
				}

			}
		}

		////***************총공격가기 //////////

		double enemyDPS2 = 0;


		for each (BWAPI::Unit *u in SelectAllEnemy()(canMove)(isCompleted)(maxGroundHits,">",0).not(isWorker))
		{
			enemyDPS2 += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
		}

		double myDPS2 = 0;

		for each (BWAPI::Unit *u in SelectAll()(canMove)(isCompleted)(maxGroundHits,">",0).not(isWorker))
		{
			myDPS2 += ((double)u->getType().groundWeapon().damageAmount())/u->getType().groundWeapon().damageCooldown();
		}


		//Broodwar->drawTextScreen(420,100,"Nexus Distance : %lf ",myDPS2);
		//myDPS2;


		//	enemy_dead_after_2min 사용하기 .. 아미매니져 지우고, 초크포인트로 이동 여기서 해주기.  total_attack  enemy_dead_after_2min 고려
		//. 8초마다 한번씩 적 본진 공격명령  일단 지워둠 .armymanager에서 하기


		static int frametest_total_attack = Broodwar->getFrameCount();   //24프레임 = 1초
		if(Broodwar->getFrameCount() == frametest_total_attack) 
		{

			///********테란전 공격가기 시작
			if(Broodwar->enemy()->getRace() == Races::Terran) 
			{
				//if (en_expand_exist && BWAPI::Broodwar->self()->supplyUsed()/2 > 110 && high_templer_loaded_complete || BWAPI::Broodwar->self()->supplyUsed()/2 > 135)  // 하이템플러가 다 탔고, 인구수가 105 이상이고, 적이 앞마당을 먹었으면 처들어 가기.  아니면 인구수가 135 이상되면 무조건 처들어가기 
				if (BWAPI::Broodwar->self()->supplyUsed()/2 > 135 && high_templer_loaded_complete || BWAPI::Broodwar->self()->supplyUsed()/2 > 135)  // 하이템플러가 다 탔고, 인구수가 105 이상이고, 적이 앞마당을 먹었으면 처들어 가기.  아니면 인구수가 135 이상되면 무조건 처들어가기 
				{
					total_attack = true;
					static int total_attack_start_time = Broodwar->getFrameCount();   //토탈 어택한 처음 시간.

					//일단 센터로 가기  total_attack_start_time
					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{
						Position *tmp = new Position(BWAPI::Broodwar->mapWidth()*32/2, BWAPI::Broodwar->mapHeight()*32/2);
						if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
						{
							
								(*its)->attack(*tmp); 
						}

						//chc 하이템플러도 같이 가기.
						if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
							(*its)->move(*tmp);

					}


					//공격가기

					if(total_attack_start_time+24*13 < Broodwar->getFrameCount())  //센터 간 후 13초 후에 공격함. 이 프레임이 8초마다 실행되므로 16초 후에 공격갈듯.   센터 갔다가 공격가는것은 맨 처음 공격에만 먹힘.
						for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
						{
							if(confidence_nice_enemy_unit_confidence_num < 300)
							{

								if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
								{
									
										(*its)->attack(this->informationManager->getEnemyStartLocation()->getPosition());
								}

								//chc 하이템플러도 같이 가기.
								if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
									(*its)->move(this->informationManager->getEnemyStartLocation()->getPosition());

							}


							//적 자신감이 높은 유닛이 있으면 그쪽으로 공격가기. 각 종족별로 자신감 숫자 바꿔서 써주기.
							if(confidence_nice_enemy_unit_confidence_num >= 300) //토스전 300이상
							{
								if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
								{
									
										(*its)->attack((*confidence_nice_enemy_unit)->getPosition());
								}


								if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
									(*its)->move((*confidence_nice_enemy_unit)->getPosition());

							}



						}

				}
			}


			///********테란전 공격가기 끝
			////////////

			///********프로전 공격가기 시작 123123123123
			if(Broodwar->enemy()->getRace() == Races::Protoss) 
			{
				if(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Shuttle)>=1) 
					total_attack=true;

				if(total_attack) 
				{
	
					total_attack = true;
					static int total_attack_start_time = Broodwar->getFrameCount();   //토탈 어택한 처음 시간.

					//일단 센터로 가기  total_attack_start_time
					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{
						/*int center_X = en_choke.x() + (my_choke.x() - en_choke.x()) * 0.4;
						int center_Y = en_choke.y() + (my_choke.y() - en_choke.y()) * 0.4;

						Position *tmp = new Position(center_X, center_Y);*/

						Position *tmp = new Position(BWAPI::Broodwar->mapWidth()*32/2, BWAPI::Broodwar->mapHeight()*32/2);

						if((*its)->getType().groundWeapon().damageAmount() >= 8 || (*its)->getType() == BWAPI::UnitTypes::Protoss_Reaver) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
						{							
							(*its)->attack(*tmp); 
						}

						//chc 하이템플러도 같이 가기.
						if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
							(*its)->move(*tmp);

					}


					//공격가기

					
					if(total_attack_start_time+24*16 < Broodwar->getFrameCount())  //센터 간 후 13초 후에 공격함. 이 프레임이 8초마다 실행되므로 16초 후에 공격갈듯.   센터 갔다가 공격가는것은 맨 처음 공격에만 먹힘.
					{
						for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
						{
							if(confidence_nice_enemy_unit_confidence_num < 300)
							{

								if((*its)->getType().groundWeapon().damageAmount() >= 8 || (*its)->getType() == BWAPI::UnitTypes::Protoss_Reaver) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
								{

									(*its)->attack(this->informationManager->getEnemyStartLocation()->getPosition());
								}

								//chc 하이템플러도 같이 가기.
								if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
									(*its)->move(this->informationManager->getEnemyStartLocation()->getPosition());

							}


							//적 자신감이 높은 유닛이 있으면 그쪽으로 공격가기. 각 종족별로 자신감 숫자 바꿔서 써주기.
							if(confidence_nice_enemy_unit_confidence_num >= 300) //토스전 300이상
							{
								if((*its)->getType().groundWeapon().damageAmount() >= 8 || (*its)->getType() == BWAPI::UnitTypes::Protoss_Reaver) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
								{
							
									(*its)->attack((*confidence_nice_enemy_unit)->getPosition());
								}


								if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
									(*its)->move((*confidence_nice_enemy_unit)->getPosition());

							}

						}
					}
						
		//질럿, 드라군 적 본진앞에서 멈추는 컨트롤 AIIDE2014


		/*bool shuttle_arrived = false;
		for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
		{
			if((*its)->getType() == BWAPI::UnitTypes::Protoss_Shuttle)
			{
				if(BWTA::getGroundDistance(BWTA::getNearestBaseLocation(en_choke)->getTilePosition(),(*its)->getTilePosition()) <= 1500)
				{
					shuttle_arrived = true;
				}
			}

		}*/

		
	/*for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
		{
			if((*its)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*its)->getType() == BWAPI::UnitTypes::Protoss_Dragoon && (*its)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar && (*its)->isCompleted())
			{
				if(BWTA::getGroundDistance(BWTA::getNearestBaseLocation(en_choke)->getTilePosition(),(*its)->getTilePosition()) <= 1300)
				{
					if(shuttle_arrived == false)
						(*its)->stop();
				}

			}
				
				
	}*/


					}
			}

			////////////프로전 공격가기 끝

			////*********저그전 공격가기 시작***//
			/*if(Broodwar->enemy()->getRace() == Races::Zerg) //
			{
				//질럿이 생산중이 아니고, 11마리 이상이면 공격 고
				//if(!zerg_first_)

				int my_zealot_num=0;

				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Assimilator) == 0)
				{
					//질럿이 11마리 태어나면 
					for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
					{	
						if ((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot && (*it)->isCompleted())
						{
							my_zealot_num++;
						}
					}
					//	if(Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Zealot) < 1 && BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot >= 11))
					//if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= 11)
					if(my_zealot_num>=11) //질럿 11마리 이상이면 공격 고고
					{
						///하기
						for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
						{			
							if ((*it)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
							{
							
								(*it)->attack(this->informationManager->getEnemyStartLocation()->getPosition());
								zerg_first_attack = true;
								defense_makgi = true;
								
								//zerg_first_ = true
							}
						}

					}
				}


				if (BWAPI::Broodwar->self()->supplyUsed()/2 > 135 && high_templer_loaded_complete || BWAPI::Broodwar->self()->supplyUsed()/2 > 135)  // 하이템플러가 다 탔고, 인구수가 135 이상이고, 적이 앞마당을 먹었으면 처들어 가기.  아니면 인구수가 135 이상되면 무조건 처들어가기 
				{
					total_attack = true;
					static int total_attack_start_time = Broodwar->getFrameCount();   //토탈 어택한 처음 시간.

					//일단 센터로 가기  total_attack_start_time
					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{
						Position *tmp = new Position(BWAPI::Broodwar->mapWidth()*32/2, BWAPI::Broodwar->mapHeight()*32/2);
						if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
						{
							
								(*its)->attack(*tmp); 
						}

						//chc 하이템플러도 같이 가기.
						if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
							(*its)->move(*tmp);

					}


					//공격가기  

					if(total_attack_start_time+24*16 < Broodwar->getFrameCount())  //센터 간 후 16초 후에 공격함. 이 프레임이 8초마다 실행되므로 16초 후에 공격갈듯.   센터 갔다가 공격가는것은 맨 처음 공격에만 먹힘.
						for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
						{
							if(confidence_nice_enemy_unit_confidence_num < 300)
							{

								if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
								{
							
									(*its)->attack(this->informationManager->getEnemyStartLocation()->getPosition());
								}

								//chc 하이템플러도 같이 가기.
								if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
									(*its)->move(this->informationManager->getEnemyStartLocation()->getPosition());

							}


							//적 자신감이 높은 유닛이 있으면 그쪽으로 공격가기. 각 종족별로 자신감 숫자 바꿔서 써주기.
							if(confidence_nice_enemy_unit_confidence_num >= 300) //토스전 300이상
							{
								if((*its)->getType().groundWeapon().damageAmount() >= 8) // 데미지가 8보다 크다면 . 공격유닛 선택하기 위함  질럿은 8씩 두번공격하는것임.
								{
									
									(*its)->attack((*confidence_nice_enemy_unit)->getPosition());
								}


								if((*its)->getType() == BWAPI::UnitTypes::Protoss_High_Templar)
									(*its)->move((*confidence_nice_enemy_unit)->getPosition());

							}
						}

				}
			}*/
			frametest_total_attack = frametest_total_attack+ 24 * 4; //4초
		}


		////////////////////////테스트중 끝




		///////////////****막멀티먹기 시작
		if(Broodwar->getFrameCount() >= 24*60*17 && multi_bool1 == false)   //17분에 하나 짓기.
		{
			baseManager->expand(1600);
			multi_bool1 = true;
		}

		if(Broodwar->getFrameCount() >= 24*60*19 && multi_bool2 == false)   //19분에 하나 짓기.
		{
			baseManager->expand(1620);
			multi_bool2 = true;
		}

		if(Broodwar->getFrameCount() >= 24*60*30 && multi_bool2 == false)   //30분에 3개 짓기.
		{
			baseManager->expand(1660);
			baseManager->expand(1670);
			baseManager->expand(1690);
			multi_bool2 = true;
		}



		///***********막멀티먹기 끝


		/////*****적 멀티 검색 후 공격 시작
		int enemy_startlocation_exist_myunit = 0;  //적 본진 근처에 우리 유닛이 몇마리 있는지
		static int tttaa_1=Broodwar->getFrameCount();
		if(tttaa_1==Broodwar->getFrameCount())
		{
			tttaa_1 = tttaa_1 + 72; //3초에 한번씩 실행

			for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
			{

				if((*its)->getDistance(en_start) < 80)
				{
					enemy_startlocation_exist_myunit++; // 적 본진 근처의 우리 유닛 마리수.
				}
			}
		}

		if(enemy_startlocation_exist_myunit>=5)
			enemy_dead=true;

		if(enemy_dead && !first_tt22)  //적이 끝났으면 그 프레임을 저장한다. 한번만 해주기위해 bool 변수 사용
		{
			enemy_dead_frame = Broodwar->getFrameCount(); 
			first_tt22 = true;

		}

		if(Broodwar->getFrameCount() > enemy_dead_frame + 24*60*3 )  //적 죽은 뒤 2분이상 지났으면 true
			enemy_dead_after_2min = true;



		/////////****적 멀티 검사 후 공격 시작////////

		if(enemy_dead_after_2min)  //적을 다 물리친 뒤 . 3 분 후
		{

			if(!base_lo_first_input)
			{
				base_lo = BWTA::getBaseLocations().begin();
				base_lo_first_input=true;	
			}	


			static int frame_expa_attack=Broodwar->getFrameCount();
			if(frame_expa_attack==Broodwar->getFrameCount())
			{
				frame_expa_attack = frame_expa_attack + 24; //1초에 한번씩 실행.

				if(base_lo != BWTA::getBaseLocations().end())
				{
					for (std::set<BWAPI::Unit *>::const_iterator its = BWAPI::Broodwar->self()->getUnits().begin(); its != BWAPI::Broodwar->self()->getUnits().end(); ++its)
					{
						if(!(*its)->hasPath((*base_lo)->getPosition())) //도달할 수 있는 지점이라면(섬이 아니라면) 베이스 다음으로 넘기고 브레이크.
						{
							base_lo++;
							break;
						}


						if((*its)->getType().groundWeapon().damageAmount() >= 8)  //공격유닛
						{
							(*its)->move((*base_lo)->getPosition()); //적 베이스로 이동


							if((*its)->getDistance((*base_lo)->getPosition()) < 50) // 적 베이스에 도착했다면 베이스로케이션 올리고 빠져나옴.
							{
								base_lo++;
								break;
							}

							for (std::set<BWAPI::Unit *>::const_iterator enemys = BWAPI::Broodwar->enemy()->getUnits().begin(); enemys != BWAPI::Broodwar->enemy()->getUnits().end(); ++enemys)
							{
								(*its)->attack((*enemys)->getPosition()); // 적을 하나라도 발견하면 적 공격

							}


						}

					}
				}
			}

			if(base_lo==BWTA::getBaseLocations().end()) //적 베이스로케이션이 엔드이면 다시 처음으로.  계속 순환하기 위함임. 잘 되는지 테스트해보기 잘못하면 에러날수도
				base_lo = BWTA::getBaseLocations().begin();

		}



		/////////****적 멀티 검사 후 공격 끝////////123123123

		//적 유닛을 너무 멀리 따라가지 않도록 ..  본진넥서스로 돌아가도록 . 나중에 초크포인트로 가도록 수정해야함.

		if(!total_attack && !zerg_first_attack)  //총공격모드일때만. 빼라.   chc 수정 zerg_first_attack
		{
			// 드랍 방어(?) 디펜스매니저를 꺼서 필요할듯..
			if(isRushMyNexus == false)
			{
				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit)
				{
					if((*allEnemyUnit)->getDistance(my_start) <= 600 && ((*allEnemyUnit)->getType().groundWeapon().damageAmount() >= 7 || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Pylon || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon) )
					{
						for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
						{
							if((*allMyUnit)->getType().groundWeapon().damageAmount() >= 7)
								(*allMyUnit)->attack((*allEnemyUnit)->getPosition());
						}
						*new_Choke = (*allEnemyUnit)->getPosition();
						isRushMyNexus = true;
					}
				}
				
			}


			//(*en)->getType() == BWAPI::UnitTypes::Protoss_Nexus
			/*if(isPhotoRush == false)
			{
				for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit)
				{
					//포토러시 대비
					if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Pylon)
					{
						int distEnemyPylon = (*allEnemyUnit)->getDistance(my_start);
						if(distEnemyPylon <= 600)
						{
							//포토러시임.
							isPhotoRush = true;
							// 초크포인트 변경. 
							*new_Choke = (*allEnemyUnit)->getPosition();
						}
					}
				}
			}
			
			int attackCount = 0;

			if(isPhotoRush == true)
			{
				for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
				{
					if(((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe))
					{
						(*allMyUnit)->rightClick(*new_Choke);
						attackCount++;
					}
					if(attackCount >= 4)
						break;
				}
			}*/


			for (std::set<BWAPI::Unit *>::const_iterator allMyUnit = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit) 
			{

				if(!((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe))
				{
					if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Nexus) 
					{
						//distEnemyNexus = (*allEnemyUnit)->getDistance(*allMyUnit);
						*myNexusPosition = (*allMyUnit)->getPosition();
					}


					//(*allMyUnit)->getDistance(
					BWAPI::Position pos = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();
					distMyNexus = (*allMyUnit)->getDistance(pos);	
					//Broodwar->drawTextScreen(420,100,"Nexus Distance : %lf , choke distance : %lf,  ground distance: %lf ", distMyNexus);
					//Broodwar->drawTextScreen(20,100,"Nexus Distance : %lf , choke distance : %lf,  ground distance: %lf ", *myNexusPosition);


					//7.31추가. 유닛이 아무것도 안하면서 초크포인트에서 100이상 떨어지면 우선 초크포인트로 이동.
					if((*allMyUnit)->getDistance(*new_Choke) > 100 && (*allMyUnit)->isIdle())
					{
						if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
						{
							(*allMyUnit)->attack(*new_Choke);
						}
						else if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
						{
							// 들어온 프로브 드라군으로 사살
							for (std::set<BWAPI::Unit *>::const_iterator allEnemyUnit = BWAPI::Broodwar->enemy()->getUnits().begin(); allEnemyUnit != BWAPI::Broodwar->enemy()->getUnits().end(); ++allEnemyUnit)
							{
								(*allMyUnit)->attack(*new_DChoke);

								if((*allEnemyUnit)->getType() == BWAPI::UnitTypes::Protoss_Probe || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Terran_SCV || (*allEnemyUnit)->getType() == BWAPI::UnitTypes::Zerg_Drone)
								{
									if((*allEnemyUnit)->getDistance(my_start)<650)
									{
										(*allMyUnit)->rightClick(*allEnemyUnit);
										break;
									}

								}
							}
						}
						else
						{
							(*allMyUnit)->attack(*new_DChoke);
						}
					}
					
					
					if( distMyNexus > 500 && (*allMyUnit)->getDistance(*new_Choke) > 140 && (*allMyUnit)->getType() != BWAPI::UnitTypes::Protoss_Observer && (*allMyUnit)->getType() != BWAPI::UnitTypes::Protoss_Dark_Templar)
					{

						// 유닛을 초크포인트로 이동.

						if((*allMyUnit)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
						{
							(*allMyUnit)->attack(*new_Choke);
						}
						else
						{
							(*allMyUnit)->attack(*new_DChoke);
						}

						//주변유닛들도 초크포인트로 같이
						for (std::set<BWAPI::Unit *>::const_iterator allMyUnit2 = BWAPI::Broodwar->self()->getUnits().begin(); allMyUnit2 != BWAPI::Broodwar->self()->getUnits().end(); ++allMyUnit2) 
						{
							if((*allMyUnit2)->getType() == BWAPI::UnitTypes::Protoss_Zealot || (*allMyUnit2)->getType() == BWAPI::UnitTypes::Protoss_Dragoon || (*allMyUnit2)->getType() == BWAPI::UnitTypes::Protoss_Reaver)
								if((*allMyUnit)->getDistance(*allMyUnit2)<150) //cig2013
								{
									//(*allMyUnit2)->move(*new_Choke);
									if((*allMyUnit2)->getType() == BWAPI::UnitTypes::Protoss_Zealot)
									{
										(*allMyUnit2)->attack(*new_Choke);
									}
									else
									{
										(*allMyUnit2)->attack(*new_DChoke);
									}
								}
						}

					}
				}
				// frametest2  =  frametest2 + 8;
			}
		}

		////////////////////////////////////////////////방어끝

		//적이 프로토스
		if(Broodwar->enemy()->getRace() == Races::Protoss) 
		{

		}
		//적 테란일때

		if(false)  //일단 끄기
		{
			if(Broodwar->enemy()->getRace() == Races::Terran)
			{
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

								/*  적 앞마당  알아내는 소스
								BWAPI::Position pos101 = informationManager->getEnemyStartLocation()->getPosition();

								Position pp = BWTA::getNearestChokepoint(pos10)->getCenter();
								BWTA::getNearestBaseLocation(pp)->getPosition();

								*/

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

								if((*n)->getType() == BWAPI::UnitTypes::Terran_Bunker || (*n)->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode || (*n)->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || (*n)->getType() == BWAPI::UnitTypes::Terran_Missile_Turret)
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
		}


		if (Broodwar->isReplay()) return;
		if (!this->analyzed) return;

		//로보틱스 짓기위해서.  cig2013 이건 뭐지?
		if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) > 3 && Broodwar->self()->minerals() <= 215 ) //드라군 4마리 이상이고, 미네랄이 215 이하이면 빌더오더 매니저를 꺼라
		{
			;
		}
		else
		{
			this->buildManager->update();
			this->buildOrderManager->update();
		}

		this->baseManager->update();


		this->workerManager->update();//chc

		//this->techManager->update();
		this->upgradeManager->update();

		//if(my_auto_build) // 오토빌드가 true이면 그때부터 파일런 자동으로 지어줌
		//this->supplyManager->update(); //chc

		this->moodManager->update();
		this->spendManager->update();
		this->scoutManager->update();
		if (!this->informationManager->getEnemyBases().empty() && Broodwar->getFrameCount() > 3000)
		{
			//if(!total_attack)  //total_attack이면 디펜스매니저 끔. 초크포인트로 이동하는것 때문에.

			//공격가면 디펜스매니저 끄기

			if(Broodwar->enemy()->getRace() == Races::Zerg) //
			{

				if(zerg_first_attack == false && defense_ing == false) // 공격 명령이 내려지기 전. 그리고 수비중이 아닐 때
					//this->defenseManager->update();

				//이것 확인
				if(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon) >= 1)
					zerg_first_attack = false;

				if(BWAPI::Broodwar->self()->supplyUsed()/2>135)
					zerg_first_attack = true;

			}

			else if(Broodwar->enemy()->getRace() == Races::Protoss)
			{
				if (!(BWAPI::Broodwar->self()->supplyUsed()/2 > 143 && high_templer_loaded_complete || BWAPI::Broodwar->self()->supplyUsed()/2 > 185) && defense_ing == false) //공격시 그리그 수비중이 아닐 때 디펜스매니저 킴
				{
					//if(total_attack == false) //한번 공격가면 수비 안됨 나중에 바꿔주기.
						//this->defenseManager->update();
				}

			}

			else if(Broodwar->enemy()->getRace() == Races::Terran)
			{
				if(!(BWAPI::Broodwar->self()->supplyUsed()/2 > 135 && high_templer_loaded_complete || BWAPI::Broodwar->self()->supplyUsed()/2 > 185) && defense_ing == false) //공격시 그리그 수비중이 아닐 때 디펜스매니저 킴
				{
					//this->defenseManager->update();

				}
			}

		}


		//this->enhancedUI->update();
		this->borderManager->update();
		//this->underAttackManager->update();
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
		}

		if(Broodwar->enemy()->getRace() == Races::Protoss)
			if (Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe) ==  10) //정찰가는 시간 정해주기 cig2013-- 9로 해줌
			{		
				this->scoutManager->setScoutCount(1);

			}

			if(Broodwar->enemy()->getRace() == Races::Terran)
				if (Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe) ==  10) //정찰가는 시간 정해주기 cig2013-- 9로 해줌
				{		
					this->scoutManager->setScoutCount(1);
				}


				if(Broodwar->enemy()->getRace() == Races::Zerg)
					if (Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe) ==  10) //정찰가는 시간 정해주기 cig2013-- 9로 해줌
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
									//Broodwar->drawTextMap(x,y+y_off,"%c%s: %d",color,j->first->getShortName().c_str(),(int)j->second);
									y_off+=15;
									color = inactiveColor;
								}
							}		
						}
					}
	}
}
void Protoss_ZR::onUnitDestroy(BWAPI::Unit* unit)
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

void Protoss_ZR::onUnitDiscover(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->informationManager->onUnitDiscover(unit);
	this->unitGroupManager->onUnitDiscover(unit);
}
void Protoss_ZR::onUnitEvade(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->informationManager->onUnitEvade(unit);
	this->unitGroupManager->onUnitEvade(unit);
}

void Protoss_ZR::onUnitMorph(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->unitGroupManager->onUnitMorph(unit);
}
void Protoss_ZR::onUnitRenegade(BWAPI::Unit* unit)
{
	if (Broodwar->isReplay()) return;
	this->unitGroupManager->onUnitRenegade(unit);
}



void Protoss_ZR::onSendText(std::string text)
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


