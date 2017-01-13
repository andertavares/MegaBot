#pragma once

#include "Interface.h"

class PositionPath
{
public:
	PositionPath();

	void addNode(Position pos);

	bool isComplete;

	void drawPath();
	int getLength();
	std::list<Position> path;
};