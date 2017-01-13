#pragma once

#include "Common.h"
#include "CombatCommander.h"
#include "InformationManager.h"
#include "MapGrid.h"
#include "base/WorkerManager.h"
#include "base/ProductionManager.h"
#include "base/BuildingManager.h"
#include "ScoutManager.h"
#include "StrategyManager.h"

#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\Timer.hpp"

using namespace NUSBot;

class TimerManager
{

	std::vector<BuildOrderSearch::Timer> timers;
	std::vector<std::string> timerNames;

	int barWidth;

public:

	enum Type { All, Worker, Production, Building, Combat, Scout, InformationManager, MapGrid, MapTools, Search, NumTypes };


	TimerManager() : timers(std::vector<BuildOrderSearch::Timer>(NumTypes)), barWidth(40)
	{
		timerNames.push_back("Total");
		timerNames.push_back("Worker");
		timerNames.push_back("Production");
		timerNames.push_back("Building");
		timerNames.push_back("Combat");
		timerNames.push_back("Scout");
		timerNames.push_back("UnitInfo");
		timerNames.push_back("MapGrid");
		timerNames.push_back("MapTools");
		timerNames.push_back("Search");
	}

	~TimerManager() {}

	void startTimer(const TimerManager::Type t)
	{
		timers[t].start();
	}

	void stopTimer(const TimerManager::Type t)
	{
		timers[t].stop();
	}

	double getTotalElapsed()
	{
		return timers[0].getElapsedTimeInMilliSec();
	}

	void displayTimers(int x, int y)
	{
		if (Options::Debug::DRAW_NUSBOT_DEBUG) BWAPI::Broodwar->drawBoxScreen(x-5, y-5, x+110+barWidth, y+5+(10*timers.size()), BWAPI::Colors::Black, true);

		int yskip = 0;
		double total = timers[0].getElapsedTimeInMilliSec();
		for (size_t i(0); i<timers.size(); ++i)
		{
			double elapsed = timers[i].getElapsedTimeInMilliSec();

			int width = (int)((elapsed == 0) ? 0 : (barWidth * (elapsed / total)));

			if (Options::Debug::DRAW_NUSBOT_DEBUG) BWAPI::Broodwar->drawTextScreen(x, y+yskip-3, "\x04 %s", timerNames[i].c_str());
			if (Options::Debug::DRAW_NUSBOT_DEBUG) BWAPI::Broodwar->drawBoxScreen(x+60, y+yskip, x+60+width+1, y+yskip+8, BWAPI::Colors::White);
			if (Options::Debug::DRAW_NUSBOT_DEBUG) BWAPI::Broodwar->drawTextScreen(x+70+barWidth, y+yskip-3, "%.4lf", elapsed);
			yskip += 10;
		}
	}
};

class UnitToAssign
{
public:

	BWAPI::Unit * unit;
	bool isAssigned;

	UnitToAssign(BWAPI::Unit * u)
	{
		unit = u;
		isAssigned = false;
	}
};


/* This is the Game Phase we want to monitor inside IPBManager
 * IPBManager will try to create strategy base on these game phases
 * We will focus more on utilizing the Layered Influence Map information as well
 * as using a TransportManager
 * FLIWA - Modified GameCommander into IPBManager
 */

enum GAME_PHASE  { INITIAL=0,
				   OPENING,
				   MIDDLE,
				   END
				 } ;

class IPBManager 
{
	CombatCommander		combatCommander;
	NUSBot::ScoutManager		scoutManager;	//prevents ambiguity with other bots' ScoutManager
	TimerManager		timerManager;

	std::set<BWAPI::Unit *> combatUnits;
	std::set<BWAPI::Unit *> scoutUnits;
	std::set<BWAPI::Unit *> workerUnits;
	//FLIWA
	std::set<BWAPI::Unit *> transportUnits;

	std::set<BWAPI::Unit *>	validUnits;
	std::set<BWAPI::Unit *> assignedUnits;

	BWAPI::Unit * currentScout;
	int numWorkerScouts;

public:
	

	//FLIWA - Added 
	int force_level;
	int unit_level;
	int squad_level;
	GAME_PHASE curr_phase ;
	GAME_PHASE next_phase ;


	const bool isAssigned(BWAPI::Unit * unit) const;


public:

	IPBManager();
	~IPBManager() {};

	void update();

	void populateUnitVectors();
	void setValidUnits();
	void setScoutUnits();
	void setWorkerUnits();
	void setCombatUnits();
	//FLIWA
	void setTransporUnits();

	void drawDebugInterface();

	bool isValidUnit(BWAPI::Unit * unit);
	bool isCombatUnit(BWAPI::Unit * unit) const;
	//FLIWA
	bool isTransportUnit(BWAPI::Unit * unit) const;

	BWAPI::Unit * getFirstSupplyProvider();
	BWAPI::Unit * getClosestUnitToTarget(BWAPI::UnitType type, BWAPI::Position target);
	BWAPI::Unit * getClosestWorkerToTarget(BWAPI::Position target);

	void onUnitShow(BWAPI::Unit * unit);
	void onUnitHide(BWAPI::Unit * unit);
	void onUnitCreate(BWAPI::Unit * unit);
	void onUnitRenegade(BWAPI::Unit * unit);
	void onUnitDestroy(BWAPI::Unit * unit);
	void onUnitMorph(BWAPI::Unit * unit);


	//FLIWA - Added

	void setGamePhase(GAME_PHASE);
	void getMapDetails();
	void describeBattleField();
	void enableThreatModel();
	void developCourseOfAction();			
};