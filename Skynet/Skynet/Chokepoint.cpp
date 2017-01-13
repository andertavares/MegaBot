#include "Chokepoint.h"

#include "Region.h"

ChokepointClass::ChokepointClass(Position side1, Position side2, int clearance)
	: mSides(side1, side2)
	, mClearance(clearance)
	, mCenter((side1.x + side2.x) / 2, (side1.y + side2.y) / 2)
{
}

void ChokepointClass::addChokepointTile(WeakRegion region, TilePosition tile)
{
	if(region.lock() == mRegions.first.lock())
		mChoketiles.first = tile;

	if(region.lock() == mRegions.second.lock())
		mChoketiles.second = tile;
}

std::pair<Region, Region> ChokepointClass::getRegions()
{
	return std::make_pair(mRegions.first.lock(), mRegions.second.lock());
}

void ChokepointClass::draw(BWAPI::Color color)
{
	BWAPI::Broodwar->drawLine(BWAPI::CoordinateType::Map, mSides.first.x, mSides.first.y, mSides.second.x, mSides.second.y, color);

	BWAPI::Broodwar->drawBox(BWAPI::CoordinateType::Map, mChoketiles.first.x*32, mChoketiles.first.y*32, mChoketiles.first.x*32+31, mChoketiles.first.y*32+31, color);
	BWAPI::Broodwar->drawBox(BWAPI::CoordinateType::Map, mChoketiles.second.x*32, mChoketiles.second.y*32, mChoketiles.second.x*32+31, mChoketiles.second.y*32+31, color);

	BWAPI::Broodwar->drawLine(BWAPI::CoordinateType::Map, mChoketiles.first.x*32+16, mChoketiles.first.y*32+16, mCenter.x, mCenter.y, color);
	BWAPI::Broodwar->drawLine(BWAPI::CoordinateType::Map, mChoketiles.second.x*32+16, mChoketiles.second.y*32+16, mCenter.x, mCenter.y, color);

	BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map, mCenter.x, mCenter.y, 32, color);
}