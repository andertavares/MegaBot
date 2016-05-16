#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "MegaBot.h"
#include "Xelnaga.h"
#include "Skynet.h"
#include "NUSBotModule.h"
#include "strategy/StrategySelector.h"
#include "data/Configuration.h"
#include "data/MatchData.h"

using namespace BWAPI;

bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;

//initializes consts
const string MegaBot::SKYNET = "Skynet";
const string MegaBot::XELNAGA = "Xelnaga";
const string MegaBot::NUSBot = "NUSBot";

MegaBot::MegaBot(){
	//initalizes behaviors
	behaviors.insert(make_pair(SKYNET, new Skynet()));
	behaviors.insert(make_pair(XELNAGA, new Xelnaga()));
	behaviors.insert(make_pair(NUSBot, new NUSBotModule()));

	//initializes reverse map
	behaviorNames.insert(make_pair(behaviors[SKYNET], SKYNET));
	behaviorNames.insert(make_pair(behaviors[XELNAGA], XELNAGA));
	behaviorNames.insert(make_pair(behaviors[NUSBot], NUSBot));

	Configuration::getInstance()->parseConfig();

	enemyBehaviorName = "Unknown";
	//for (auto behv : behaviors) {
	//	behaviorNames.insert(make_pair(behv.second, behv.first));
	//}

}

void MegaBot::onStart() {
	// Uncomment to enable complete map information
	//Broodwar->enableFlag(Flag::CompleteMapInformation);
	MatchData::getInstance()->registerMatchBegin();

	myBehaviorName = StrategySelector::getInstance()->getStrategy();//Configuration::getInstance()->strategyID;
	currentBehavior = behaviors[myBehaviorName];
	
	currentBehavior->onStart();

	//sends behavior communication message
	Broodwar->sendText("Behavior: %s", myBehaviorName.c_str());

  
	//overrides speed and gui set by currentBehavior
	int speed = Configuration::getInstance()->speed;
	Broodwar->printf("Setting speed to %d.", speed);
	Broodwar->setLocalSpeed(speed);

	bool gui = Configuration::getInstance()->enableGUI;
	Broodwar->printf("Setting GUI to %s.", gui ? "enabled" : "disabled");
	Broodwar->setGUI(gui);
}

void MegaBot::onEnd(bool isWinner) {
	currentBehavior->onEnd(isWinner);

	int result = MatchData::LOSS;
	if (isWinner) result = MatchData::WIN;
	if (Broodwar->elapsedTime() / 60 >= 80) result = MatchData::DRAW;

	//StrategySelector::getInstance()->addResult(win);
	//StrategySelector::getInstance()->saveStats();
	//Statistics::getInstance()->saveResult(win);

	MatchData::getInstance()->registerMatchFinish(result);
	MatchData::getInstance()->writeSummary();
	MatchData::getInstance()->writeDetailedResult();
}

void MegaBot::onFrame() {

	if (Broodwar->elapsedTime() / 60 >= 81) {	//leave stalled game
		Broodwar->leaveGame();
		return;
	}

	currentBehavior->onFrame();
	/*
	if (show_visibility_data)
		drawVisibilityData();

	if (show_bullets)
		drawBullets();

	if (Broodwar->isReplay())
		return;

	drawStats();
	  if (analyzed && Broodwar->getFrameCount()%30==0)
	  {
		//order one of our workers to guard our chokepoint.
		for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
		{
		  if ((*i)->getType().isWorker())
		  {
			//get the chokepoints linked to our home region
			std::set<BWTA::Chokepoint*> chokepoints= home->getChokepoints();
			double min_length=10000;
			BWTA::Chokepoint* choke=NULL;

			//iterate through all chokepoints and look for the one with the smallest gap (least width)
			for(std::set<BWTA::Chokepoint*>::iterator c=chokepoints.begin();c!=chokepoints.end();c++)
			{
			  double length=(*c)->getWidth();
			  if (length<min_length || choke==NULL)
			  {
				min_length=length;
				choke=*c;
			  }
			}

			//order the worker to move to the center of the gap
			(*i)->rightClick(choke->getCenter());
			break;
		  }
		}
	  }
	  if (analyzed)
		drawTerrainData();

	  if (analysis_just_finished)
	  {
		Broodwar->printf("Finished analyzing map.");
		analysis_just_finished=false;
	  }*/
}

void MegaBot::onSendText(std::string text) {
	currentBehavior->onSendText(text);

	if (text=="/show bullets") {
		show_bullets = !show_bullets;
	} 
	else if (text=="/show players") {
		showPlayers();
	} 
	else if (text=="/show forces")  {
		showForces();
	}
	else if (text=="/show visibility") {
		show_visibility_data=!show_visibility_data;
	}
	else if (text=="/analyze") {
		if (analyzed == false) {
			Broodwar->printf("Analyzing map... this may take a minute");
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	} 
	else{
		Broodwar->printf("You typed '%s'!",text.c_str());
		Broodwar->sendText("%s",text.c_str());
	}
}

void MegaBot::onReceiveText(BWAPI::Player* player, std::string text) {
	currentBehavior->onReceiveText(player, text);
	
	//Broodwar->printf(">>>>> substr: %s", text.substr(0, 8).c_str());
	if (text.substr(0, 9) == "Behavior:") {	//receives behavior communication message
		//splits text in 2 parts and gets 2nd part: this is enemy's name
		istringstream iss(text);
		vector<string> tokens;
		copy(
			istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter(tokens)
		);
		//the 'magic' above is from: http://stackoverflow.com/a/237280/1251716

		enemyBehaviorName = tokens[1];

		Broodwar->printf(">>>>> Enemy is: %s <<<<<", enemyBehaviorName);
	}
	else {
		Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
	}
}

void MegaBot::onPlayerLeft(BWAPI::Player* player) {
	currentBehavior->onPlayerLeft(player);
	Broodwar->sendText("%s left the game.",player->getName().c_str());
}

void MegaBot::onNukeDetect(BWAPI::Position target) {
	currentBehavior->onNukeDetect(target);

  if (target!=Positions::Unknown)
    Broodwar->printf("Nuclear Launch Detected at (%d,%d)",target.x(),target.y());
  else
    Broodwar->printf("Nuclear Launch Detected");
}

void MegaBot::onUnitDiscover(BWAPI::Unit* unit) {
	currentBehavior->onUnitDiscover(unit);

  //if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
  //  Broodwar->printf("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void MegaBot::onUnitEvade(BWAPI::Unit* unit) {
	currentBehavior->onUnitEvade(unit);

	//if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
	//	Broodwar->printf("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void MegaBot::onUnitShow(BWAPI::Unit* unit) {
	currentBehavior->onUnitShow(unit);
	//if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
	//	Broodwar->printf("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void MegaBot::onUnitHide(BWAPI::Unit* unit) {
	currentBehavior->onUnitHide(unit);
	//if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
		//Broodwar->printf("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void MegaBot::onUnitCreate(BWAPI::Unit* unit){
	currentBehavior->onUnitCreate(unit);

  /*if (Broodwar->getFrameCount()>1)
  {
    if (!Broodwar->isReplay())
      Broodwar->printf("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
    else
    {
      /*if we are in a replay, then we will print out the build order
      (just of the buildings, not the units).*
      if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral()==false)
      {
        int seconds=Broodwar->getFrameCount()/24;
        int minutes=seconds/60;
        seconds%=60;
        Broodwar->printf("%.2d:%.2d: %s creates a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
      }
    }
  }*/
}

void MegaBot::onUnitDestroy(BWAPI::Unit* unit) {
	currentBehavior->onUnitDestroy(unit);
  //if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
  //  Broodwar->printf("A %s [%x] has been destroyed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void MegaBot::onUnitMorph(BWAPI::Unit* unit){
	currentBehavior->onUnitMorph(unit);

  /*if (!Broodwar->isReplay())
    Broodwar->printf("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  else
  {
    /*if we are in a replay, then we will print out the build order
    (just of the buildings, not the units).*
    if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral()==false)
    {
      int seconds=Broodwar->getFrameCount()/24;
      int minutes=seconds/60;
      seconds%=60;
      Broodwar->printf("%.2d:%.2d: %s morphs a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
    }
  }*/
}

void MegaBot::onUnitRenegade(BWAPI::Unit* unit) {
	currentBehavior->onUnitRenegade(unit);
}

void MegaBot::onSaveGame(std::string gameName) {
	currentBehavior->onSaveGame(gameName);
	Broodwar->printf("The game was saved to \"%s\".", gameName.c_str());
}

void MegaBot::onUnitComplete(BWAPI::Unit *unit) {
	currentBehavior->onUnitComplete(unit);

	//if (!Broodwar->isReplay() && Broodwar->getFrameCount()>1)
	//	Broodwar->printf("A %s [%x] has been completed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

DWORD WINAPI AnalyzeThread() {
  BWTA::analyze();

  //self start location only available if the map has base locations
  if (BWTA::getStartLocation(BWAPI::Broodwar->self())!=NULL)  {
    home       = BWTA::getStartLocation(BWAPI::Broodwar->self())->getRegion();
  }
  //enemy start location only available if Complete Map Information is enabled.
  if (BWTA::getStartLocation(BWAPI::Broodwar->enemy())!=NULL) {
    enemy_base = BWTA::getStartLocation(BWAPI::Broodwar->enemy())->getRegion();
  }
  analyzed   = true;
  analysis_just_finished = true;
  return 0;
}

void MegaBot::drawStats() {
  /*std::set<Unit*> myUnits = Broodwar->self()->getUnits();
  Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
  std::map<UnitType, int> unitTypeCounts;
  for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++) {
    if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end()) {
      unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
    }
    unitTypeCounts.find((*i)->getType())->second++;
  }
  int line=1;
  for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++) {
    Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
    line++;
  }*/
}

void MegaBot::drawBullets() {
  /*std::set<Bullet*> bullets = Broodwar->getBullets();
  for(std::set<Bullet*>::iterator i=bullets.begin();i!=bullets.end();i++)
  {
    Position p=(*i)->getPosition();
    double velocityX = (*i)->getVelocityX();
    double velocityY = (*i)->getVelocityY();
    if ((*i)->getPlayer()==Broodwar->self())
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Green);
      Broodwar->drawTextMap(p.x(),p.y(),"\x07%s",(*i)->getType().getName().c_str());
    }
    else
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Red);
      Broodwar->drawTextMap(p.x(),p.y(),"\x06%s",(*i)->getType().getName().c_str());
    }
  }*/
}

void MegaBot::drawVisibilityData() {
  for(int x=0;x<Broodwar->mapWidth();x++) {
    for(int y=0;y<Broodwar->mapHeight();y++) {
      if (Broodwar->isExplored(x,y)) {
        if (Broodwar->isVisible(x,y))
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Green);
        else
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Blue);
      }
      else
        Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Red);
    }
  }
}

void MegaBot::drawTerrainData(){
  //we will iterate through all the base locations, and draw their outlines.
/*  for(std::set<BWTA::BaseLocation*>::const_iterator i=BWTA::getBaseLocations().begin();i!=BWTA::getBaseLocations().end();i++)
  {
    TilePosition p=(*i)->getTilePosition();
    Position c=(*i)->getPosition();

    //draw outline of center location
    Broodwar->drawBox(CoordinateType::Map,p.x()*32,p.y()*32,p.x()*32+4*32,p.y()*32+3*32,Colors::Blue,false);

    //draw a circle at each mineral patch
    for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getStaticMinerals().begin();j!=(*i)->getStaticMinerals().end();j++)
    {
      Position q=(*j)->getInitialPosition();
      Broodwar->drawCircle(CoordinateType::Map,q.x(),q.y(),30,Colors::Cyan,false);
    }

    //draw the outlines of vespene geysers
    for(std::set<BWAPI::Unit*>::const_iterator j=(*i)->getGeysers().begin();j!=(*i)->getGeysers().end();j++)
    {
      TilePosition q=(*j)->getInitialTilePosition();
      Broodwar->drawBox(CoordinateType::Map,q.x()*32,q.y()*32,q.x()*32+4*32,q.y()*32+2*32,Colors::Orange,false);
    }

    //if this is an island expansion, draw a yellow circle around the base location
    if ((*i)->isIsland())
      Broodwar->drawCircle(CoordinateType::Map,c.x(),c.y(),80,Colors::Yellow,false);
  }

  //we will iterate through all the regions and draw the polygon outline of it in green.
  for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
  {
    BWTA::Polygon p=(*r)->getPolygon();
    for(int j=0;j<(int)p.size();j++)
    {
      Position point1=p[j];
      Position point2=p[(j+1) % p.size()];
      Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Green);
    }
  }

  //we will visualize the chokepoints with red lines
  for(std::set<BWTA::Region*>::const_iterator r=BWTA::getRegions().begin();r!=BWTA::getRegions().end();r++)
  {
    for(std::set<BWTA::Chokepoint*>::const_iterator c=(*r)->getChokepoints().begin();c!=(*r)->getChokepoints().end();c++)
    {
      Position point1=(*c)->getSides().first;
      Position point2=(*c)->getSides().second;
      Broodwar->drawLine(CoordinateType::Map,point1.x(),point1.y(),point2.x(),point2.y(),Colors::Red);
    }
  }
  */
}

void MegaBot::showPlayers()
{
  /*std::set<Player*> players=Broodwar->getPlayers();
  for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
  {
    Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
  }*/
}

void MegaBot::showForces()
{
  /*std::set<Force*> forces=Broodwar->getForces();
  for(std::set<Force*>::iterator i=forces.begin();i!=forces.end();i++)
  {
    std::set<Player*> players=(*i)->getPlayers();
    Broodwar->printf("Force %s has the following players:",(*i)->getName().c_str());
    for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++)
    {
      Broodwar->printf("  - Player [%d]: %s",(*j)->getID(),(*j)->getName().c_str());
    }
  }*/
}


