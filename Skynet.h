#pragma once

#include <BWAPI.h>

class Skynet : public BWAPI::AIModule
{
public:
	Skynet();

	virtual void onStart();
};