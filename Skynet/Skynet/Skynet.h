#pragma once

#include "Interface.h"

#include "Task.h"

class Skynet : public BWAPI::AIModule
{
public:
	Skynet();

	virtual void onStart();
	virtual void onEnd(bool isWinner);

	virtual void onFrame();

	virtual void onSendText(std::string text);
	virtual void onPlayerLeft(Player player);

private:
	bool mOnBegin;

	int mLeavingGame;
	bool mSaidGG;
};