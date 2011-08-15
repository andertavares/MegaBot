#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Task.h"
#include "Base.h"

class BlockedPathManagerClass
{
public:
	BlockedPathManagerClass();

	void onBegin();

	void update();

private:
	std::map<Base, UnitGroup> mBlockingMinerals;

	std::set<Base> mMyBases;
};

typedef Singleton<BlockedPathManagerClass> BlockedPathManager;