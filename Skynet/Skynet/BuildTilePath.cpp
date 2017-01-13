#include "BuildTilePath.h"
#include "BuildingPlacer.h"

BuildTilePath::BuildTilePath()
	: isComplete(false)
{
}

void BuildTilePath::addNode(TilePosition pos)
{
	path.push_front(pos);
}

void BuildTilePath::drawPath()
{
	TilePosition previousTile = BWAPI::TilePositions::None;
	for each(const TilePosition &pos in path)
	{
		if(previousTile != BWAPI::TilePositions::None)
			BWAPI::Broodwar->drawLineMap(pos.x * 32 + 16, pos.y * 32 + 16, previousTile.x * 32 + 16, previousTile.y * 32 + 16, BWAPI::Colors::Yellow);

		previousTile = pos;
	}
}

bool BuildTilePath::isStillValid()
{
	for each(const TilePosition &pos in path)
	{
		if(!BuildingPlacer::Instance().isTileWalkable(pos))
			return false;
	}

	return true;
}