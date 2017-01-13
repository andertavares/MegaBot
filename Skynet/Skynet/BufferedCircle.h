#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedCircle : public BufferedItem
{
public:
	BufferedCircle(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int x, int y, int radius, BWAPI::Color color, bool solid)
		: BufferedItem(coordType, id, time)
		, mX(x)
		, mY(y)
		, mRadius(radius)
		, mColor(color)
		, mIsSolid(solid)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawCircle(mCoordType, mX, mY, mRadius, mColor, mIsSolid);
	}

private:
	int mX;
	int mY;
	int mRadius;
	BWAPI::Color mColor;
	bool mIsSolid;
};