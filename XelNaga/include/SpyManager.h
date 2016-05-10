#pragma once
#include <Arbitrator.h>
#include <InformationManager.h>
#include <BuildOrderManager.h>
#include <UnitGroupManager.h>
#include <Random.h>
#include <BWAPI.h>
#include <stack>

class SpyManager : public Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class SpyData
	{
	public:
		enum SpyMode
		{
			Idle,
			Searching,
			Spying,
			Fleeing
		};
		SpyData(){ mode = Idle; }
		BWTA::BaseLocation* target;
		int lastFrameDodging;
		SpyMode mode;
	};

	SpyManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator);
	~SpyManager();

	virtual void onOffer	(std::set<BWAPI::Unit*> units);
	virtual void onRevoke	(BWAPI::Unit* unit, double bid);
	virtual void update		();

	virtual std::string getName			() const;
	virtual std::string getShortName() const;
	void								onRemoveUnit(BWAPI::Unit* unit);

	// Non-Controller methods.
	void buildObserver				();
	void setInformationManager(InformationManager*);
	void setBuildOrderManager	(BuildOrderManager*);
	void setSpyCount					(int);
	void setDebugMode					(bool);
	void initialize						();
	bool isInitialized				();

private:
	void																				drawAssignments();
	Arbitrator::Arbitrator<BWAPI::Unit*,double>	*arbitrator;
	InformationManager													*informationManager;
	BuildOrderManager														*buildOrderManager;

	Random																			*randomDodge;

	std::map<BWAPI::Unit*, SpyData>							spies;
	std::stack <BWTA::BaseLocation*>						baseLocationsToSpy;
	std::set<BWTA::BaseLocation*>								baseLocationsSpied;
	BWTA::BaseLocation													*myStartLocation;
	const BWTA::BaseLocation										*enemyStartLocation;

	int																					numberBasesToSpy;
	int																					spyingStartFrame;
	bool																				debugMode;
	int																					lastFrameCheck;
	bool																				initialized;
};
