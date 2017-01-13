#pragma once

#include "Interface.h"

class BuildTilePath
{
public:
	BuildTilePath();

	void addNode(TilePosition pos);

	bool isComplete;

	void drawPath();

	std::list<TilePosition> path;

	bool isStillValid();
};