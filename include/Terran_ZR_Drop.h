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

class Terran_ZR_Drop : public BWAPI::AIModule
{
private:
	double	minDist;
	int			frame;
	int			lastFrameExpand;
	int			lastScout;
	bool         dddd; //다크길 열어주기 한번만
	bool             d;
	bool            dd;
	bool           ddddd;
	bool             d5;
	int           a;
	bool		total_attack;
	

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

	~Terran_ZR_Drop(); //not part of BWAPI::AIModule

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
	bool									showManagerAssignments;
	bool									debugMode;
	bool									destinationMineralSpotted;
	bool									cleaningPath;
};