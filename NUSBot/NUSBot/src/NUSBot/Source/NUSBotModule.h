#pragma once

#include <BWAPI.h>
#include "IPBManager.h"
#include <iostream>
#include <fstream>
#include "Logger.h"
#include "MapTools.h"
#include "HardCodedInfo.h"
#include "../../StarcraftBuildOrderSearch/Source/starcraftsearch/StarcraftData.hpp"
//#include "SparCraftManager.h"
//#include "ReplayVisualizer.h"

#include "../../SparCraft/source/SparCraft.h"
#include "EnhancedInterface.hpp"
#include "UnitCommandManager.h"

#include "Options.h"

class NUSBotModule : public BWAPI::AIModule
{
	IPBManager			IPBManager;
	EnhancedInterface		eui;
	//SparCraftManager		micro;

public:
			
	NUSBotModule();
	~NUSBotModule();

	void	onStart();
	void	onFrame();
	void	onEnd(bool isWinner);
	void	onUnitDestroy(BWAPI::Unit * unit);
	void	onUnitMorph(BWAPI::Unit * unit);
	void	onSendText(std::string text);
	void	onUnitCreate(BWAPI::Unit * unit);
	void	onUnitShow(BWAPI::Unit * unit);
	void	onUnitHide(BWAPI::Unit * unit);
	void	onUnitRenegade(BWAPI::Unit * unit);
};
