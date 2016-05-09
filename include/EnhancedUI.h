#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <UnitGroupManager.h>
#include <InformationManager.h>

class EnhancedUI
{
public:
	void update() const;
	void setInformationManager	(InformationManager*);

private:
	void drawStats() const;
	void drawBases() const;
	void drawTerrain() const;
	void drawProgress() const;
	void drawProgressBar(BWAPI::Position pos, double progressFaction, BWAPI::Color innerBar = BWAPI::Colors::Green) const;
	InformationManager								*informationManager;
};