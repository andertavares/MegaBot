#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "BaseSquad.h"
#include "BuildOrder.h"
#include "DefenseSquad.h"

class SquadManagerClass
{
public:
	SquadManagerClass();

	void update();

	void onChangeBuild();
	BaseSquadTaskPointer createSquad(SquadType type);

private:
	std::map<SquadType, std::set<BaseSquadTaskPointer>> mSquads;

	ArmyBehaviour mCurrentBehaviour;

	BaseSquadTaskPointer mDefaultSquad;
	std::map<Base, DefenseSquadPointer> mDefenseSquads;
};

typedef Singleton<SquadManagerClass> SquadManager;