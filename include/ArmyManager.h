#pragma once
#include <Arbitrator.h>
#include <DefenseManager.h>
#include <InformationManager.h>
#include <UnitGroupManager.h>
#include <BaseManager.h>
#include <BWAPI.h>
#include <BWTA.h>
//#include <SpendManager.h>

class ArmyManager : Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class ArmyData
	{
	public:
		enum ArmyMode
		{
			Idle,
			Harassing,
			Attacking,
			Fleeing
		};
		ArmyData(){ mode = Idle; }
		BWTA::BaseLocation* target;
		ArmyMode mode;
	};
	ArmyManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);
	void setDefenseManager			(DefenseManager*);
	void setInformationManager	(InformationManager*);
	void setBaseManager					(BaseManager*);
	virtual void onOffer(std::set<BWAPI::Unit*> unitGroup);
	virtual void onRevoke(BWAPI::Unit *p, double bid);
	void onRemoveUnit(BWAPI::Unit *unit);
	virtual void update();
	virtual std::string getName() const;
	virtual std::string getShortName() const;
	double enemyDPS();
	double myDPS();
	bool   observer;
	bool   total_attack;
	//SpendManager					*spendManager;
private:
	Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator;
//chc
	std::vector<BWTA::Chokepoint*>							myBorderVector;
	DefenseManager										*defenseManager;
	InformationManager								*informationManager;
	BaseManager												*baseManager;
	std::map<BWAPI::Unit*, ArmyData>	company;
	std::vector<BWAPI::Position>			enemyBuildings;
	int																lastFrameCheck;
	int																lastExpandFrame;
	bool															firstAttack;
	bool                                                            d;
	bool                                                            dd;
	bool                                                            ddd;
	bool                                                            dddd;

	int																round;
};