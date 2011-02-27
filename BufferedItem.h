#pragma once

#include <BWAPI.h>

namespace BufferedCategory
{
	enum Type
	{
		Default,
		BuildingPlacer
	};
}

class BufferedItem
{
public:
	BufferedItem(BWAPI::CoordinateType::Enum coordType, BufferedCategory::Type id, int frameTime) : mCoordType(coordType), mID(id), mFrameTime(frameTime) {}
	virtual ~BufferedItem(){}

	virtual void draw(){};

	bool isTimeUp() { return BWAPI::Broodwar->getFrameCount() >= mFrameTime; }

protected:
	BWAPI::CoordinateType::Enum mCoordType;
	BufferedCategory::Type mID;
	int mFrameTime;
};