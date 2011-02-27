#pragma once

#include <BWAPI.h>

namespace BufferedCategory
{
	enum Enum
	{
		Default,
		BuildingPlacer
	};
}

class BufferedItem
{
public:
	BufferedItem(BWAPI::CoordinateType::Enum coordType, BufferedCategory::Enum id, int frameTime) : mCoordType(coordType), mID(id), mFrameTime(frameTime) {}
	virtual ~BufferedItem(){}

	virtual void draw(){};

	bool isTimeUp() { return BWAPI::Broodwar->getFrameCount() >= mFrameTime; }

protected:
	BWAPI::CoordinateType::Enum mCoordType;
	BufferedCategory::Enum mID;
	int mFrameTime;
};