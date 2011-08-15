#include "SquadManager.h"
#include "TaskManager.h"
#include "BuildOrderManager.h"
#include "BaseTracker.h"

#include "DefaultSquad.h"

SquadManagerClass::SquadManagerClass()
{
	mDefaultSquad = createSquad(SquadType::DefaultSquad);
}

void SquadManagerClass::update()
{
	for(std::map<Base, DefenseSquadPointer>::iterator it = mDefenseSquads.begin(); it != mDefenseSquads.end();)
	{
		if(it->first->getEnemyThreats().empty() || (it->first->isMinedOut() && it->first->getNumberOfTechBuildings() == 0))
			it->second->cancel();

		if(it->second->hasEnded())
			mDefenseSquads.erase(it++);
		else
			++it;
	}

	for each(Base base in BaseTracker::Instance().getPlayerBases())
	{
		if(!base->getEnemyThreats().empty() && (base->getNumberOfTechBuildings() > 0 || (!base->isMinedOut() && base->getResourceDepot())) && mDefenseSquads.count(base) == 0)
		{
			DefenseSquadPointer squad = std::tr1::static_pointer_cast<DefenseSquadTask>(createSquad(SquadType::DefenseSquad));
			squad->setGoal(Goal(ActionType::Defend, base));
			mDefenseSquads[base] = squad;
		}
	}
}

void SquadManagerClass::onChangeBuild()
{
	const std::map<SquadType, int> &squads = BuildOrderManager::Instance().getCurrentBuild().getSquads();
	for(std::map<SquadType, int>::const_iterator it = squads.begin(); it != squads.end(); ++it)
	{
		if(it->first == SquadType::DefaultSquad || it->first == SquadType::DefenseSquad)
			continue;

		int numNeeded = it->second - mSquads[it->first].size();
		while(numNeeded != 0)
		{
			if(numNeeded < 0)
			{
				++numNeeded;

				int smallest = std::numeric_limits<int>::max();
				BaseSquadTaskPointer smallestSquad;
				for each(BaseSquadTaskPointer squad in mSquads[it->first])
				{
					int size = squad->controlSize();
					if(size < smallest)
					{
						smallest = size;
						smallestSquad = squad;
					}
				}

				if(smallestSquad)
				{
					smallestSquad->cancel();
					mSquads[it->first].erase(smallestSquad);
				}
			}
			else
			{
				--numNeeded;
				createSquad(it->first);
			}
		}
	}

	for(std::map<SquadType, std::set<BaseSquadTaskPointer>>::iterator o = mSquads.begin(); o != mSquads.end();)
	{
		if(o->first != SquadType::DefaultSquad && o->first != SquadType::DefenseSquad && squads.find(o->first) == squads.end())
		{
			for each(BaseSquadTaskPointer task in o->second)
			{
				task->cancel();
			}

			mSquads.erase(o++);
		}
		else ++o;
	}

	mCurrentBehaviour = BuildOrderManager::Instance().getCurrentBuild().getArmyBehaiour();
	for(std::map<SquadType, std::set<BaseSquadTaskPointer>>::iterator o = mSquads.begin(); o != mSquads.end(); ++o)
	{
		for each(BaseSquadTaskPointer squad in o->second)
		{
			squad->changeBehaviour(mCurrentBehaviour);
		}
	}
}

BaseSquadTaskPointer SquadManagerClass::createSquad(SquadType type)
{
	BaseSquadTaskPointer task;

	switch(type.underlying())
	{
	case SquadType::DefaultSquad:
		task = BaseSquadTaskPointer(new DefaultSquadTask(mCurrentBehaviour));
		break;
	case SquadType::DefenseSquad:
		task = BaseSquadTaskPointer(new DefenseSquadTask(mCurrentBehaviour));
		break;
	case SquadType::ReaverDropSquad:
		//task = BaseSquadTaskPointer(new ReaverDropSquad());
		break;
	case SquadType::DarkTemplerSquad:
		//task = BaseSquadTaskPointer(new DarkTemplerSquad());
		break;
	case SquadType::CorsairSquad:
		//task = BaseSquadTaskPointer(new CorsairSquad());
		break;
	}

	if(task)
	{
		mSquads[type].insert(task);
		TaskManager::Instance().addTask(task);
	}

	return task;
}