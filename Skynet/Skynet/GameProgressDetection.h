#pragma once

#include "Interface.h"

#include "Singleton.h"

struct StateTypeDef
{
	enum type
	{
		BuildArmy,
		TechNormal,
		TechHigh
	};
};
typedef SafeEnum<StateTypeDef> StateType;

class GameProgressDetectionClass
{
public:
	GameProgressDetectionClass(){}

	void update();

	bool shouldGG() const { return mShouldGG; }
	bool shouldAttack() const { return mShouldAttack; }
	StateType getState() const { return mState; }

private:
	bool mShouldGG;
	bool mShouldAttack;

	StateType mState;
};

typedef Singleton<GameProgressDetectionClass> GameProgressDetection;