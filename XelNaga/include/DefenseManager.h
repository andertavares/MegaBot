#pragma once
#include <Arbitrator.h>
#include <BWAPI.h>
#include <BWTA.h>
#include <BaseManager.h>
#include <InformationManager.h>
class BorderManager;
class DefenseManager : Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class DefenseData
	{
	public:
		enum DefenseMode
		{
			Idle,
			Moving,
			Defending
		};
		DefenseData(){ mode = Idle; }
		DefenseMode mode;
	};
	DefenseManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);

	void										setBorderManager			(BorderManager* borderManager);
	void										setBaseManager				(BaseManager *baseManager);
	void										setInformationManager	(InformationManager *informationManager);
	std::set<BWAPI::Unit*>& getIdleDefenders			();
	virtual void						onOffer								(std::set<BWAPI::Unit*> units);
	virtual void						onRevoke							(BWAPI::Unit* unit, double bid);
	void										onRemoveUnit					(BWAPI::Unit* unit);
	virtual void						update								();
	virtual std::string			getName								() const;
	virtual std::string			getShortName					() const;

	//chc
	// std::set<BWTA::Chokepoint*> enemyBorder;
	// std::vector<BWTA::Chokepoint*> enemyBorderVector;

private:
	BorderManager																*borderManager;
	BaseManager																	*baseManager; 
	InformationManager													*informationManager;
	Arbitrator::Arbitrator<BWAPI::Unit*,double>	*arbitrator;
	std::set<BWTA::Chokepoint*>									myBorder;
	std::vector<BWTA::Chokepoint*>							myBorderVector;

	bool	firsss2; //chc
	bool	very_early_rush;
	bool	very_early_attack_gate;
	bool	early_attack_assimi;
	bool			early_attack_gate;
	bool	early_rush ;
	bool	firsss;


	std::map<BWAPI::Unit*,DefenseData>					defenders;
	std::set<BWAPI::Unit*>											idleDefenders;
	int																					lastFrameCheck;
	int																					round;
};