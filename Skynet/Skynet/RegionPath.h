#pragma once

#include "Interface.h"

#include "Region.h"

class RegionPath
{
public:
	RegionPath();

	void addNode(Region reg);

	bool isComplete;

	void drawPath();
	std::list<Region> path;
};