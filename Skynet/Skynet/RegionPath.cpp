#include "RegionPath.h"

RegionPath::RegionPath()
	: isComplete(false)
{
}

void RegionPath::addNode(Region reg)
{
	path.push_front(reg);
}

void RegionPath::drawPath()
{
	Region previousRegion;
	for each(const Region &reg in path)
	{
		if(previousRegion)
			BWAPI::Broodwar->drawLineMap(reg->getCenter().x, reg->getCenter().y, previousRegion->getCenter().x, previousRegion->getCenter().y, BWAPI::Colors::Purple);

		previousRegion = reg;
	}
}