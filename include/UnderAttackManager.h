#pragma once
#include <Arbitrator.h>
#include <UnitGroupManager.h>
#include <UnitGroup.h>
#include <DefenseManager.h>
#include <BaseManager.h>
#include <BWAPI.h>

class UnderAttackManager : Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class UAData
	{
	public:
		enum UAMode
		{
			Idle,
			Worker,
			WorkerDefending,
			Defender,
			Defending
		};
		UAData()						{ mode = Idle; }
		UAData(UAMode mode) { this->mode = mode; }
		UAMode mode;
		BWAPI::Unit *needHelp;
	};
	
	UnderAttackManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);
	
	virtual void				onOffer						(std::set<BWAPI::Unit*> unitGroup);
	virtual void				onRevoke					(BWAPI::Unit *p, double bid);
	void								onRemoveUnit			(BWAPI::Unit *unit);
	virtual void				update						();
	virtual std::string getName						() const;
	virtual std::string getShortName			() const;

	void								setDefenseManager	(DefenseManager*);
	void								setBaseManager		(BaseManager*);

private:
	Arbitrator::Arbitrator<BWAPI::Unit*,double> *arbitrator;
	DefenseManager															*defenseManager;
	BaseManager																	*baseManager;
	int																					lastFrameCheck;

	//set of all units controlled by the UAM
	std::map<BWAPI::Unit*, UAData>							backUp;
};