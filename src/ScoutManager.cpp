#include <BWTA.h>
#include <ScoutManager.h>
#include <UnitGroupManager.h>
using namespace BWAPI;

//chc

void ScoutManager::firstsearch()
{
	
	for (std::set<BWAPI::Unit *>::const_iterator it = BWAPI::Broodwar->self()->getUnits().begin(); it != BWAPI::Broodwar->self()->getUnits().end(); ++it)
    {	
		if ((*it)->getType().isWorker() || (*it)->getType() == BWAPI::UnitTypes::Protoss_Observer)
        {
			
			static_cast< Arbitrator::Arbitrator<BWAPI::Unit*,double>* >(arbitrator)->setBid(this, (*it),92);
		}
	}
}


void ScoutManager::observer()
{
}
void ScoutManager::corsair()
{
}
void ScoutManager::sometimes()
{
}







std::pair<std::list<BWTA::BaseLocation*>, double> getBestPathHelper(std::set<BWTA::BaseLocation* > baseLocations)
{
	std::pair<std::list<BWTA::BaseLocation*>, double> shortest_path;
	shortest_path.second=0;
	if (baseLocations.empty()) return shortest_path;
	if (baseLocations.size()==1)
	{
		shortest_path.first.push_back(*baseLocations.begin());
		return shortest_path;
	}
	for(std::set<BWTA::BaseLocation*>::iterator i=baseLocations.begin();i!=baseLocations.end();i++)
	{
		BWTA::BaseLocation* node=*i;
		std::set<BWTA::BaseLocation* > baseLocations2=baseLocations;
		baseLocations2.erase(*i);
		std::pair<std::list<BWTA::BaseLocation*>, double> path_result=getBestPathHelper(baseLocations2);
		double dist=path_result.second+node->getGroundDistance(path_result.first.front());
		if (dist<shortest_path.second || shortest_path.first.empty())
		{
			path_result.first.push_front(node);
			shortest_path=std::make_pair(path_result.first,dist);
		}
	}
	return shortest_path;
}

std::pair<std::list<BWTA::BaseLocation*>, double> getBestPath(std::set<BWTA::BaseLocation* > baseLocations)
{
	std::pair<std::list<BWTA::BaseLocation*>, double> shortest_path;
	shortest_path.second=0;
	if (baseLocations.empty()) return shortest_path;
	BWTA::BaseLocation* start=BWTA::getStartLocation(BWAPI::Broodwar->self());
	baseLocations.erase(start);
	if (baseLocations.size()==1)
	{
		shortest_path.first.push_back(*baseLocations.begin());
		return shortest_path;
	}
	for(std::set<BWTA::BaseLocation*>::iterator i=baseLocations.begin();i!=baseLocations.end();i++)
	{
		BWTA::BaseLocation* node=*i;
		std::set<BWTA::BaseLocation* > baseLocations2=baseLocations;
		baseLocations2.erase(*i);
		std::pair<std::list<BWTA::BaseLocation*>, double> path_result=getBestPathHelper(baseLocations2);
		double dist=start->getGroundDistance(node)+node->getGroundDistance(path_result.first.front())+path_result.second;
		if (dist<shortest_path.second || shortest_path.first.empty())
		{
			path_result.first.push_front(node);
			shortest_path=std::make_pair(path_result.first,dist);
		}
	}
	return shortest_path;
}
ScoutManager::ScoutManager(Arbitrator::Arbitrator<BWAPI::Unit*,double> *arbitrator, InformationManager* infoManager)
{
	this->arbitrator = arbitrator;
	this->informationManager = infoManager;
	desiredScoutCount = 0;
	scoutDone_chc = false; //chc

	myStartLocation = BWTA::getStartLocation(BWAPI::Broodwar->self());
	std::set<BWTA::BaseLocation *> startLocations = BWTA::getStartLocations();
	startLocations.erase(myStartLocation);
	std::set<BWTA::BaseLocation *>::iterator l_next;
	for(std::set<BWTA::BaseLocation *>::iterator l = startLocations.begin(); l != startLocations.end(); l=l_next)
	{
		l_next=l;
		l_next++;
		if (myStartLocation->getGroundDistance(*l) <= 0)
		{
			startLocations.erase(*l);
		}
	}

	if (startLocations.size() == 1)
	{
		baseLocationsToScout.clear();
		informationManager->setEnemyStartLocation(*startLocations.begin());
		baseLocationsExplored.insert(*startLocations.begin());
		scoutDone = true;
	}
	else
	{
		std::list<BWTA::BaseLocation*> path=getBestPath(startLocations).first;
		for(std::list<BWTA::BaseLocation*>::iterator p=path.begin();p!=path.end();p++)
			baseLocationsToScout.push_back(*p);
		scoutDone = false;
	}

	baseLocationsExplored.insert(myStartLocation);	
	this->debugMode=true; //chc
}

void ScoutManager::onOffer(std::set<BWAPI::Unit*> units)
{
	std::set<BWAPI::Unit*>::iterator u2;
	for(std::set<BWAPI::Unit*>::iterator u = units.begin(); u != units.end(); u=u2)
	{
		u2=u;
		u2++;
		//ignore if its already a scout
		if (scouts.find(*u) != scouts.end())
		{
			arbitrator->accept(this, *u);
			units.erase(u);
		}
		else if ((*u)->getType().isWorker() && needMoreScouts())
		{
			arbitrator->accept(this, *u);
			addScout(*u);
			units.erase(u);
		}
	}

	//decline remaining units
	for(std::set<BWAPI::Unit*>::iterator u = units.begin(); u != units.end(); u++)
	{
		arbitrator->decline(this, *u, 0);
	}
}

void ScoutManager::onRevoke(BWAPI::Unit *unit, double bid)
{
	onRemoveUnit(unit);
}

void ScoutManager::update()
{
	if (!scoutDone)
	{
		if (needMoreScouts())
		{
			requestScout(30); // Bid 30.
		}
		else
		{
			int sCount = desiredScoutCount;
			if (baseLocationsExplored.size()==BWAPI::Broodwar->getStartLocations().size())
				sCount = 0;
			while ((int)scouts.size()>sCount)
			{
				arbitrator->removeBid(this, scouts.begin()->first);
				scouts.erase(scouts.begin());
			}
		}
		updateScoutAssignments();
		if (debugMode)
		{
			drawAssignments();
		}
	}
}

std::string ScoutManager::getName() const
{
	return "Scout Manager";
}

std::string ScoutManager::getShortName() const
{
	return "Scout";
}

void ScoutManager::onRemoveUnit(BWAPI::Unit* unit)
{
	if (scouts.find(unit) != scouts.end())
	{
		BWTA::BaseLocation* lostTarget = scouts[unit].target;
		if (baseLocationsExplored.find(lostTarget) == baseLocationsExplored.end())
		{
			baseLocationsToScout.push_back(lostTarget);
			if (debugMode)
			{
				BWAPI::Broodwar->printf("Reassigning (%d,%d)", lostTarget->getPosition().x(), lostTarget->getPosition().y());
			}
		}
		scouts.erase(unit);
	}
}

void ScoutManager::setScoutCount(int count)
{
	this->desiredScoutCount=count;
}

void ScoutManager::setDebugMode(bool debugMode)
{
	this->debugMode = debugMode;
}

void ScoutManager::drawAssignments()
{
	//draw target vector for each scout
	for (std::map<BWAPI::Unit*,ScoutData>::iterator s = scouts.begin(); s != scouts.end(); s++)
	{
		if ((*s).second.mode != ScoutData::Idle)
		{
			BWAPI::Position scoutPos = (*s).first->getPosition();
			BWAPI::Position targetPos = (*s).second.target->getPosition();
			BWAPI::Broodwar->drawLineMap(scoutPos.x(), scoutPos.y(), targetPos.x(), targetPos.y(), BWAPI::Colors::Yellow);
			BWAPI::Broodwar->drawCircleMap(scoutPos.x(), scoutPos.y(), 6, BWAPI::Colors::Yellow);
			BWAPI::Broodwar->drawCircleMap(targetPos.x(), targetPos.y(), (*s).first->getType().sightRange(), BWAPI::Colors::Yellow);
		}
	}
}

bool ScoutManager::isScouting() const
{
	return scoutDone_chc; //chc
}

bool ScoutManager::needMoreScouts() const
{
	int sCount = desiredScoutCount;
	if (baseLocationsExplored.size()==BWAPI::Broodwar->getStartLocations().size())
		sCount = 0;
	return (int)scouts.size() < sCount;
}

void ScoutManager::requestScout(double bid)
{
	// Bid on all completed workers.
	std::set<BWAPI::Unit*> usefulUnits=SelectAll()(isWorker,Zerg_Overlord)(isCompleted).not(isCarryingMinerals,isCarryingGas,isGatheringGas);
	arbitrator->setBid(this,usefulUnits,bid);
}

void ScoutManager::addScout(BWAPI::Unit* u)
{
	ScoutData temp;
	scouts.insert(std::make_pair(u,temp));
}

void ScoutManager::updateScoutAssignments()
{
	// Remove scout positions if the enemy is not there.
	std::map<BWAPI::Unit*, ScoutData>::iterator u;
	for(u = scouts.begin(); u != scouts.end(); u++)
	{
		if ( (*u).second.mode == ScoutData::Searching
			&& (*u).first->getPosition().getApproxDistance((*u).second.target->getPosition()) < (*u).first->getType().sightRange() - 2)
		{
			bool empty = true;
			for(int x=(*u).second.target->getTilePosition().x();x<(*u).second.target->getTilePosition().x()+4;x++)
			{
				for(int y=(*u).second.target->getTilePosition().y();y<(*u).second.target->getTilePosition().y()+3;y++)
				{
					std::set<BWAPI::Unit*> getUnitsOnTile = BWAPI::Broodwar->getUnitsOnTile(x,y);
					for each(BWAPI::Unit* u in getUnitsOnTile)
					{
						// u->getType().isInvincible() means it is a mineral field or a vespene geyser.
						if (u->getType().isBuilding() && !(u->getType().isInvincible()))
						{
							empty = false;
							break;
							
						}
					}
					if (!empty) break;
				}
				if (!empty) break;
			}

			//위에까지 정찰완료

			BWTA::BaseLocation* exploredBaseLocation = (*u).second.target;

			if (empty)
			{
				this->informationManager->setBaseEmpty((*u).second.target);
				baseLocationsToScout.remove(exploredBaseLocation);
				baseLocationsExplored.insert(exploredBaseLocation);
				(*u).second.mode = ScoutData::Idle;
						
			}
			else
			{
				this->informationManager->setEnemyStartLocation((*u).second.target);   //적 시작지점 저장
				baseLocationsToScout.clear();
				baseLocationsExplored.insert(exploredBaseLocation);
				(*u).second.mode = ScoutData::Idle;


					//일꾼 돌리기 시작 

				//1. 적 모든 건물의 position 받기
/*
for (std::set<BWAPI::Unit *>::const_iterator en = Broodwar->enemy()->getUnits().begin(); en != BWAPI::Broodwar->enemy()->getUnits().end(); ++en)
			{
			
				//if(*en)->getType() == BWAPI::UnitTypes::Protoss_Nexus)
				if((*en)->isBeingConstructed()) //지어지고 있는것? 아님 다 지어진건물?
				{
				//건물과 프로브 포지션 받기
				BWAPI::Position builded = (*en)->getPosition();
				BWAPI::Position probe = (*u).first->getPosition();
				//건물과 프로브포지션 직각이 되는 점 계산
                //하기
				}

				//프로브를 공격하려는 유닛과 프로브 와의 외분점
				//(단, 적 유닛은 미네랄 캐거나 건물짓는 유닛이 아닌 유닛들(정찰프로브 쫒차오는 유닛들))
				if(!((*en)->isConstructing()) || !((*en)->isGatheringGas()) || !((*en)->isGatheringGas()))
				{
					if((*en)->getDistance((*u).first) < 5)
					{
						BWAPI::Broodwar->printf("Probe unit distance (%d)", (*en)->getDistance((*u).first));


					}				
				 
				}

*/
				//위 모든 점들의 중점찾기 ( 나중에 가까운 곳에 대한 가중치도 부여 )
                //해야함





				/*
				
				 for (std::set<BWAPI::Unit *>::const_iterator itss = BWAPI::Broodwar->self()->getUnits().begin(); itss != BWAPI::Broodwar->self()->getUnits().end(); ++itss)
    {		
	
		if ((*itss)->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar)
        {
			(*itss)->rightClick(*pn);
		}
				 }
				}
}
		}
				


*/


				// std::set<BWTA::Chokepoint*>::const_iterator c = this->borderManager->getEnemyBorder();
			//	std::set<BWTA::Chokepoint*>::const_iterator cd =  this->borderManager->getMyBorder().begin();
/*
				std::set<BWTA::Chokepoint*>::const_iterator cd =  this->borderManager->getEnemyBorder().begin();
				 BWAPI::Position point1=(*cd)->getSides().first;
				(*u).first->rightClick(point1);5
*/
				
		  
	  
	  /* BWAPI::Position point1=(*c)->getSides().first;
    //BWAPI::Position point2=(*c)->getSides().second;
	(*u).first->rightClick(point1);
	//Position *c = (*u).first->getPosition();  */
          





					//chc 몰래건물 위치로 정찰병 옮김
			/*	
				int a = this->informationManager->getEnemyStartLocation()->getPosition().x();  
			    int b = this->informationManager->getEnemyStartLocation()->getPosition().y();
				Position *c = new Position(880,790);
				Position *d = new Position(3310,3530);
			
				if(b>=a)
					(*u).first->rightClick(*c);
				else
					(*u).first->rightClick(*d);				

*/

				//chc
				//this->workerManager      = new WorkerManager(&this->arbitrator);
			//	(*u).first->rightClick(myStartLocation->getPosition()); //다시돌아오는chc
				//this->WorkerManager->onRevoke((*u).first,1);
																
											
				arbitrator->removeBid(this,(*u).first);
				scouts.erase((*u).first);
				
				
				scoutDone = true;
				scoutDone_chc = true;

							
			
				
				BWAPI::Broodwar->printf("Sucessfully scouted (%d,%d)", exploredBaseLocation->getPosition().x(), exploredBaseLocation->getPosition().y());
				BWAPI::Broodwar->printf("Sucessfully EnemyStartLocation (%d,%d)",this->informationManager->getEnemyStartLocation()->getPosition().x(),this->informationManager->getEnemyStartLocation()->getPosition().y());

			     break;
			}
       //  this->WorkerManager->onRevoke((*u).first,1);
			
			if (debugMode)//
			{
				BWAPI::Broodwar->printf("Sucessfully scouted (%d,%d)", exploredBaseLocation->getPosition().x(), exploredBaseLocation->getPosition().y());
				BWAPI::Broodwar->printf("Sucessfully EnemyStartLocation (%d,%d)",this->informationManager->getEnemyStartLocation()->getPosition().x(),this->informationManager->getEnemyStartLocation()->getPosition().y());

			}
		}
	}

	// Set scouts to scout.
	if (baseLocationsToScout.size() == 1)
	{
		BWTA::BaseLocation* exploredBaseLocation = (*baseLocationsToScout.begin());
		this->informationManager->setEnemyStartLocation(exploredBaseLocation);
		baseLocationsExplored.insert(exploredBaseLocation);
		scouts.begin()->second.mode = ScoutData::Idle;


			//chc 몰래건물위치로 정찰병 옮김
	/*	int a = this->informationManager->getEnemyStartLocation()->getPosition().x();
			    int b = this->informationManager->getEnemyStartLocation()->getPosition().y();
				Position *c = new Position(880,790);
				Position *d = new Position(3310,3530);
				
				if(b>=a)
					scouts.begin()->first->rightClick(*c);
				else
					scouts.begin()->first->rightClick(*d);
	*/
	//	scouts.begin()->first->rightClick(myStartLocation->getPosition());
		arbitrator->removeBid(this,scouts.begin()->first);
		scouts.erase(scouts.begin()->first);
		baseLocationsToScout.clear();
		scoutDone = true;
		
	}
	if (baseLocationsToScout.size() > 1) // are there still positions to scout?
	{
		std::list<BWTA::BaseLocation*>::iterator p;
		for(u = scouts.begin(); u != scouts.end() && !baseLocationsToScout.empty(); u++)
		{ // for
			if ((u->second.mode == ScoutData::Idle) || (!u->first->isMoving()))
			{
				std::map<double, BWTA::BaseLocation*> distanceMap;
				for (p = baseLocationsToScout.begin(); p != baseLocationsToScout.end(); p++)
				{
					double distance = (*u).first->getPosition().getApproxDistance((*p)->getPosition());
					distanceMap[distance] = *p;
				}
				BWTA::BaseLocation* target = distanceMap.begin()->second;
				(*u).second.mode = ScoutData::Searching;
				(*u).first->rightClick(target->getPosition());
				(*u).second.target = target;
				if (debugMode)
				{
					BWAPI::Broodwar->printf("Scouting (%d,%d)", target->getPosition().x(), target->getPosition().y());
				}
			}
		} // for
	}


}

