#include "PositionPath.h"

PositionPath::PositionPath()
	: isComplete(false)
{
}

void PositionPath::addNode(Position pos)
{
	path.push_front(pos);
}

void PositionPath::drawPath()
{
	Position previousPosition = BWAPI::Positions::None;
	for each(const Position &pos in path)
	{
		if(previousPosition != BWAPI::Positions::None)
			BWAPI::Broodwar->drawLineMap(pos.x, pos.y, previousPosition.x, previousPosition.y, BWAPI::Colors::Yellow);

		previousPosition = pos;
	}
}

int PositionPath::getLength()
{
	int totalLength = 0;
	Position previousPosition = BWAPI::Positions::None;
	for each(const Position &pos in path)
	{
		if(previousPosition != BWAPI::Positions::None)
			totalLength += pos.getApproxDistance(previousPosition);

		previousPosition = pos;
	}

	return totalLength;
}