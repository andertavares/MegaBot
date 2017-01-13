#pragma once

#include "Interface.h"

#include "Singleton.h"
#include "Task.h"

class ExpansionManagerClass
{
public:
	ExpansionManagerClass();

	void update();

private:
	std::list<TaskPointer> mRefineryTasks;
	void updateRefineries();

	TaskPointer mPylon;
	std::list<TaskPointer> mDefenseTasks;
	void updateDefense();

	std::list<TaskPointer> mExpandTasks;
	void updateExpands();
};

typedef Singleton<ExpansionManagerClass> ExpansionManager;