#include "Skynet.h"

#include <iostream>

#include "DrawBuffer.h"
#include "TerrainAnaysis.h"
#include "UnitTracker.h"
#include "BaseTracker.h"
#include "BuildingPlacer.h"
#include "TaskManager.h"
#include "ResourceTracker.h"
#include "ResourceManager.h"
#include "TrainTask.h"
#include "ConstructionTask.h"
#include "MorphTask.h"
#include "WallTracker.h"
#include "BuildOrderManager.h"
#include "PlayerTracker.h"
#include "BorderTracker.h"
#include "ScoutManager.h"
#include "SupplyManager.h"
#include "MacroManager.h"
#include "ExpansionManager.h"
#include "UnitInformation.h"
#include "UnitHelper.h"
#include "LatencyTracker.h"
#include "PylonPowerTracker.h"
#include "MapHelper.h"
#include "PathFinder.h"
#include "Logger.h"
#include "GameProgressDetection.h"
#include "AOEThreatTracker.h"
#include "SquadManager.h"
#include "BlockedPathManager.h"
#include "GameMemory.h"

Skynet::Skynet()
	: mOnBegin(false)
	, mLeavingGame(0)
	, mSaidGG(false)
{
}

void Skynet::onStart()
{
	BWAPI::Broodwar->sendText("Skynet 2.1 Operational");

	BWAPI::Broodwar->setLatCom(false);
	BWAPI::Broodwar->setCommandOptimizationLevel(1);

	if(BWAPI::Broodwar->self()->getRace() != BWAPI::Races::Protoss)
		BWAPI::Broodwar->sendText("Skynet is a Protoss only bot");

	if(BWAPI::Broodwar->getRevision() != BWAPI::BWAPI_getRevision())
	{
		BWAPI::Broodwar->sendText("This version of Skynet was compiled for BWAPI rev. %d", BWAPI::BWAPI_getRevision());
		BWAPI::Broodwar->sendText("But BWAPI rev. %d is currently running", BWAPI::Broodwar->getRevision());
	}

	BWAPI::Broodwar->setLocalSpeed(0);
}

void Skynet::onEnd(bool isWinner)
{
	BuildOrderManager::Instance().onEnd(isWinner);
	GameMemory::Instance().onEnd();
}

void Skynet::onFrame()
{
	for each(BWAPI::Event BWAPIEvent in BWAPI::Broodwar->getEvents())
	{
		if(BWAPIEvent.getType() == BWAPI::EventType::UnitDiscover)
			UnitTracker::Instance().onUnitDiscover(BWAPIEvent.getUnit());
	}
	for each(BWAPI::Event BWAPIEvent in BWAPI::Broodwar->getEvents())
	{
		if(BWAPIEvent.getType() == BWAPI::EventType::UnitDestroy)
			UnitTracker::Instance().onUnitDestroy(BWAPIEvent.getUnit());
	}

	if(!mOnBegin)
	{
		mOnBegin = true;
		WallTracker::Instance().onBegin();
		PlayerTracker::Instance().onBegin();
		TerrainAnaysis::Instance().analyseBasesAndTerrain();
		BorderTracker::Instance().onBegin();
		UnitTracker::Instance().pumpUnitEvents();
		ScoutManager::Instance().onBegin();
		MacroManager::Instance().onBegin();
		GameMemory::Instance().onBegin();

		BuildOrderManager::Instance().onBegin();

		BlockedPathManager::Instance().onBegin();
	}

	UnitTracker::Instance().update();

	AOEThreatTracker::Instance().update();

	BlockedPathManager::Instance().update();
	PylonPowerTracker::Instance().update();
	UnitInformation::Instance().update();
	BorderTracker::Instance().update();
	PlayerTracker::Instance().update();
	BaseTracker::Instance().update();
	BuildingPlacer::Instance().update();
	ResourceManager::Instance().update();
	SupplyManager::Instance().update();
	MacroManager::Instance().update();
	ExpansionManager::Instance().update();
	LatencyTracker::Instance().update();
	GameProgressDetection::Instance().update();

	BuildOrderManager::Instance().update();
	ScoutManager::Instance().update();
	SquadManager::Instance().update();

	TaskManager::Instance().update();

	DrawBuffer::Instance().update();

	if(GameProgressDetection::Instance().shouldGG())
	{
		if(mLeavingGame == 0)
			mLeavingGame = BWAPI::Broodwar->getFrameCount();
	}
	else
		mLeavingGame = 0;

	if(mLeavingGame != 0)
	{
		if(BWAPI::Broodwar->getFrameCount() - mLeavingGame > 24 && !mSaidGG)
		{
			mSaidGG = true;
			BWAPI::Broodwar->sendText("gg");
		}
		else if(BWAPI::Broodwar->getFrameCount() - mLeavingGame > 80)
			BWAPI::Broodwar->leaveGame();
	}
}

void Skynet::onSendText(std::string text)
{
	BWAPI::Broodwar->sendText(text.c_str());
}

void Skynet::onPlayerLeft(Player player)
{
	PlayerTracker::Instance().onPlayerLeft(player);
}