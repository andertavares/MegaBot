#pragma once

#include <BWAPI.h>

class Skynet : public BWAPI::AIModule
{
public:
	Skynet();

	virtual void onStart();
	virtual void onEnd(bool isWinner);

	virtual void onFrame();
};