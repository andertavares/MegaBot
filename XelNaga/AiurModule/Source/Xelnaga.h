#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <Arbitrator.h>
#include <WorkerManager.h>
#include <SupplyManager.h>
#include <BuildManager.h>
#include <BuildOrderManager.h>
#include <TechManager.h>
#include <UpgradeManager.h>
#include <BaseManager.h>
#include <ScoutManager.h>
#include <DefenseManager.h>
#include <UnderAttackManager.h>
#include <InformationManager.h>
#include <SpyManager.h>
#include <BorderManager.h>
#include <UnitGroupManager.h>
#include <SpendManager.h>
#include <ArmyManager.h>
#include <MoodManager.h>
#include <EnhancedUI.h>


#include <Protoss_FD.h>//chc
#include <Protoss_ZR.h>//chc
#include <Protoss_Multi.h>//chc
#include <Terran_ZR_Drop.h>//chc
#include <Protoss_FZ.h>//chc
#include <stdio.h> 

namespace XelnagaBot {
	class Xelnaga : public BWAPI::AIModule
	{
	private:
		double	minDist;



		char pre_word1[1024];
		char pre_word2[1024];
		char last_word[1024];
		int lost_strategy_num;
	
		FILE *fp1;
		FILE *fp2;
		FILE *fp3;
		FILE *fp4;
	
	



		bool				high_templer_loaded_complete;  //하이템플러 4마리가 다 탔는지
		std::set<BWTA::BaseLocation*>::const_iterator base_lo;

		//더 이상 헤더파일 선언하면 에러?


		//BWTA::BaseLocation base_lo;
		bool    base_lo_first_input;  //base_lo 초기화

		bool    enemy_dead;
		int		enemy_dead_frame;
		bool    enemy_dead_after_2min;
		bool    first_tt22;
		bool cig2013_11;


		int used_ob[2];   //옵저버 관련
		bool ob_bool;
		bool ob_bool2;

		bool en_expand_exist;

		//chc
		std::set<BWTA::Chokepoint*> enemyBorder;
		std::vector<BWTA::Chokepoint*> enemyBorderVector;


		BWTA::BaseLocation *closest;
		BWTA::BaseLocation *home;

	public:
		virtual void onStart				();
		virtual void onEnd					(bool isWinner);
		virtual void onFrame				();
		virtual void onUnitDiscover	(BWAPI::Unit* unit);
		virtual void onUnitEvade		(BWAPI::Unit* unit);
		virtual void onUnitMorph		(BWAPI::Unit* unit);
		virtual void onUnitRenegade	(BWAPI::Unit* unit);
		virtual void onUnitDestroy	(BWAPI::Unit* unit);
		virtual void onSendText			(std::string text);

		~Xelnaga(); //not part of BWAPI::AIModule

		void showStats(); //not part of BWAPI::AIModule
		void showPlayers();
		void showForces();
		bool analyzed;

		std::map<BWAPI::Unit*,BWAPI::UnitType>			buildings;
		Arbitrator::Arbitrator<BWAPI::Unit*,double> arbitrator;

		WorkerManager					*workerManager;
		SupplyManager					*supplyManager;
		BuildManager					*buildManager;
		TechManager						*techManager;
		UpgradeManager				*upgradeManager;
		BaseManager						*baseManager;
		ScoutManager					*scoutManager;
		SpendManager					*spendManager;
		BuildOrderManager			*buildOrderManager;
		DefenseManager				*defenseManager;
		UnderAttackManager		*underAttackManager;
		InformationManager		*informationManager;
		BorderManager					*borderManager;
		UnitGroupManager			*unitGroupManager;
		SpyManager						*spyManager;
		ArmyManager						*armyManager;
		MoodManager						*moodManager;
		EnhancedUI						*enhancedUI;


		Protoss_FD						*protoss_FD;
		Protoss_ZR						*protoss_ZR;
		Protoss_Multi					*protoss_Multi;
		Terran_ZR_Drop					*terran_ZR_Drop;
		Protoss_FZ						*protoss_FZ;



		bool									showManagerAssignments;
		bool									debugMode;
		bool									destinationMineralSpotted;
		bool									cleaningPath;
		//bool				total_attack; //총공격모드
	};
}