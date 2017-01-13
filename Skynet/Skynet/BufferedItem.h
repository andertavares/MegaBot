#pragma once

#include "Interface.h"

#include "TypeSafeEnum.h"

struct BufferedCategoryDef
{
	enum type
	{
		Default,
		BuildingPlacer
	};
};
typedef SafeEnum<BufferedCategoryDef> BufferedCategory;

class BufferedItem
{
public:
	BufferedItem(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int frameTime) : mCoordType(coordType), mID(id), mFrameTime(frameTime) {}
	virtual ~BufferedItem(){}

	virtual void draw(){};

	bool isTimeUp() { return BWAPI::Broodwar->getFrameCount() >= mFrameTime; }

protected:
	BWAPI::CoordinateType::Enum mCoordType;
	BufferedCategory mID;
	int mFrameTime;
};