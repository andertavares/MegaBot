
#include "Common.h"
#include "NUSBotModule.h"


BWAPI::AIModule * __NewAIModule()
{
	return new NUSBotModule();
}

NUSBotModule::NUSBotModule()  {}
NUSBotModule::~NUSBotModule() {}

void NUSBotModule::onStart()
{
	BWAPI::Broodwar->sendText("NUSBot v1.0.0.0 Online");
	BWAPI::Broodwar->setLocalSpeed(0);
	//BWAPI::Broodwar->setFrameSkip(240);

	BWAPI::Broodwar->setCommandOptimizationLevel(1); //For Optimization

    SparCraft::init();

	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
	//BWAPI::Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);

    Options::BotModes::SetBotMode(Options::BotModes::AIIDE_TOURNAMENT);
	Options::Modules::checkOptions();
	
    if (Options::Modules::USING_IPBMANAGER)
	{
		//IPBManager::GAME_PHASE gp;
		BWAPI::Broodwar->drawTextScreen(200, 330, "%s",  "IPB_MANAGER_OPENING\n");
		//IPBManager.getMapDetails();
		BWTA::readMap();
		BWTA::analyze();
		IPBManager.setGamePhase(GAME_PHASE::OPENING); 
	}
	
	if (Options::Modules::USING_MICRO_SEARCH)
	{
		SparCraft::init();
		
		//micro.onStart();
	}

	if (Options::Modules::USING_BUILD_LEARNER)
	{
		BuildOrderSearch::getStarcraftDataInstance().init(BWAPI::Broodwar->self()->getRace());
		SparCraft::Hash::initHash();
	}
}

void NUSBotModule::onEnd(bool isWinner) 
{
	if (Options::Modules::USING_IPBMANAGER)
	{
		StrategyManager::Instance().onEnd(isWinner);

		std::stringstream result;
		std::string win = isWinner ? "win" : "lose";

		double sum = 0;
		BOOST_FOREACH (BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
			{
				sum += sqrt((double)(unit->getHitPoints() + unit->getShields()));
			}
		}
		BOOST_FOREACH (BWAPI::Unit * unit, BWAPI::Broodwar->enemy()->getUnits())
		{
			if (unit->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
			{
				sum -= sqrt((double)(unit->getHitPoints() + unit->getShields()));
			}
		}

		//result << "Game against " << BWAPI::Broodwar->enemy()->getName() << " " << win << " with strategy " << StrategyManager::Instance().getCurrentStrategy() << "\n";

		result << sum << " " << BWAPI::Broodwar->getFrameCount() << "\n";

		Logger::Instance().log(result.str());

		ProductionManager::Instance().onGameEnd();
	}	
}

void NUSBotModule::onFrame()
{
	if (Options::Modules::USING_UNIT_COMMAND_MGR)
	{
		UnitCommandManager::Instance().update();
	}

	if (Options::Modules::USING_IPBMANAGER) 
	{ 
		IPBManager.update(); 
	}
	
	if (Options::Modules::USING_ENHANCED_INTERFACE)
	{
		eui.update();
	}

	if (Options::Modules::USING_MICRO_SEARCH)
	{
		//micro.update();
	}


	if (Options::Modules::USING_REPLAY_VISUALIZER)
	{
		BOOST_FOREACH (BWAPI::Unit * unit, BWAPI::Broodwar->getAllUnits())
		{
			BWAPI::Broodwar->drawTextMap(unit->getPosition().x(), unit->getPosition().y(), "   %d", unit->getPlayer()->getID());

			if (unit->isSelected())
			{
				BWAPI::Broodwar->drawCircleMap(unit->getPosition().x(), unit->getPosition().y(), 1000, BWAPI::Colors::Red);
			}
		}
	}
}

void NUSBotModule::onUnitDestroy(BWAPI::Unit * unit)
{
	if (Options::Modules::USING_IPBMANAGER) { IPBManager.onUnitDestroy(unit); }
	if (Options::Modules::USING_ENHANCED_INTERFACE) { eui.onUnitDestroy(unit); }
}

void NUSBotModule::onUnitMorph(BWAPI::Unit * unit)
{
	if (Options::Modules::USING_IPBMANAGER) { IPBManager.onUnitMorph(unit); }
}

void NUSBotModule::onSendText(std::string text) 
{ 
	BWAPI::Broodwar->sendText(text.c_str());

	if (Options::Modules::USING_REPLAY_VISUALIZER && (text.compare("sim") == 0))
	{
		BWAPI::Unit * selected = NULL;
		BOOST_FOREACH (BWAPI::Unit * unit, BWAPI::Broodwar->getAllUnits())
		{
			if (unit->isSelected())
			{
				selected = unit;
				break;
			}
		}

		if (selected)
		{
			#ifdef USING_VISUALIZATION_LIBRARIES
				//ReplayVisualizer rv;
				//rv.launchSimulation(selected->getPosition(), 1000);
			#endif
		}
	}

	if (Options::Modules::USING_BUILD_ORDER_DEMO)
	{

		std::stringstream type;
		std::stringstream numUnitType;
		int numUnits = 0;

		int i=0;
		for (i=0; i<text.length(); ++i)
		{
			if (text[i] == ' ')
			{
				i++;
				break;
			}

			type << text[i];
		}

		for (; i<text.length(); ++i)
		{
			numUnitType << text[i];
		}

		numUnits = atoi(numUnitType.str().c_str());

	
		BWAPI::UnitType t = BWAPI::UnitTypes::getUnitType(type.str());

		BWAPI::Broodwar->printf("Searching for %d of %s", numUnits, t.getName().c_str());

		MetaPairVector goal;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Probe, 8));
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Gateway, 2));
		goal.push_back(MetaPair(t, numUnits));

		ProductionManager::Instance().setSearchGoal(goal);
	}
}

void NUSBotModule::onUnitCreate(BWAPI::Unit * unit)
{ 
	if (Options::Modules::USING_IPBMANAGER) { IPBManager.onUnitCreate(unit); }
}

void NUSBotModule::onUnitShow(BWAPI::Unit * unit)
{ 
	if (Options::Modules::USING_IPBMANAGER) { IPBManager.onUnitShow(unit); }
}

void NUSBotModule::onUnitHide(BWAPI::Unit * unit)
{ 
	if (Options::Modules::USING_IPBMANAGER) { IPBManager.onUnitHide(unit); }
}

void NUSBotModule::onUnitRenegade(BWAPI::Unit * unit)
{ 
	if (Options::Modules::USING_IPBMANAGER) { IPBManager.onUnitRenegade(unit); }
}
