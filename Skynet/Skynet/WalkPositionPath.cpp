#include "WalkPositionPath.h"

WalkPositionPath::WalkPositionPath()
	: isComplete(false)
{
}

void WalkPositionPath::addNode(WalkPosition pos)
{
	path.push_front(pos);
}

void WalkPositionPath::drawPath()
{
	WalkPosition previousPosition(-1, -1);
	for each(const WalkPosition &pos in path)
	{
		if(previousPosition.x != -1 && previousPosition.y != -1)
			BWAPI::Broodwar->drawLineMap(pos.x * 8 + 4, pos.y * 8 + 4, previousPosition.x * 8 + 4, previousPosition.y * 8 + 4, BWAPI::Colors::Yellow);

		previousPosition = pos;
	}
}

WalkPositionPath WalkPositionPath::getReverse()
{
	WalkPositionPath returnPath;

	for each(const WalkPosition &pos in path)
	{
		returnPath.path.push_front(pos);
		returnPath.isComplete = isComplete;
	}

	return returnPath;
}