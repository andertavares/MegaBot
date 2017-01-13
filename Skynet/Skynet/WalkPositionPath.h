#pragma once

#include "Interface.h"

class WalkPositionPath
{
public:
	WalkPositionPath();

	void addNode(WalkPosition pos);

	bool isComplete;

	void drawPath();

	std::list<WalkPosition> path;

	WalkPositionPath getReverse();
};