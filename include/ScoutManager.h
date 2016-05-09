#pragma once
#include <Arbitrator.h>
#include <InformationManager.h>
#include <BorderManager.h>
#include <WorkerManager.h>
#include <BWAPI.h>
#include <cmath>

class ScoutManager : public Arbitrator::Controller<BWAPI::Unit*,double>
{
public:
	class ScoutData
	{
	public:
		enum ScoutMode
		{
			Idle,
			Searching,
			Roaming,
			Harassing,
			Fleeing
		};
		ScoutData(){ mode = Idle; }
		BWTA::BaseLocation* target;
		ScoutMode mode;
	};
	ScoutManager(Arbitrator::Arbitrator<BWAPI::Unit*,double>*, InformationManager*);

	virtual void onOffer(std::set<BWAPI::Unit*>);
	virtual void onRevoke(BWAPI::Unit*, double);
	virtual void update();

	virtual std::string getName() const;
	virtual std::string getShortName() const;
	void onRemoveUnit(BWAPI::Unit* unit);

	// Non-Controller methods.
	bool scoutDone_chc;
	bool isScouting() const;
	void setScoutCount(int count);
	void setDebugMode(bool debugMode);
	void setDestinationMineral(BWAPI::Unit*);
	
	
	//chc
	void firstsearch();
	void observer();
    void corsair();
	void sometimes();
	
	

	




	std::map<BWAPI::Unit*, ScoutData> scouts;
	Arbitrator::Arbitrator<BWAPI::Unit*,double>* arbitrator;

	std::list<BWTA::BaseLocation*> baseLocationsToScout;
	std::set<BWTA::BaseLocation*> baseLocationsExplored;
	BWTA::BaseLocation *myStartLocation;

private:
	bool needMoreScouts() const;
	void requestScout(double bid);
	void addScout(BWAPI::Unit* unit);
	void updateScoutAssignments();
	void drawAssignments();
	InformationManager* informationManager;
	WorkerManager*		workerManager;
	BorderManager*		borderManager;

	size_t desiredScoutCount;
	int scoutingStartFrame;
	bool debugMode;
	int lastFrameCheck;
	bool scoutDone;
	
};
