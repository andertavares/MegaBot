#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedTriangle : public BufferedItem
{
public:
	BufferedTriangle(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int x1, int y1, int x2, int y2, int x3, int y3, BWAPI::Color color, bool solid)
		: BufferedItem(coordType, id, time)
		, mX1(x1)
		, mY1(y1)
		, mX2(x2)
		, mY2(y2)
		, mX3(x3)
		, mY3(y3)
		, mColor(color)
		, mIsSolid(solid)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawTriangle(mCoordType, mX1, mY1, mX2, mY3, mX2, mY3, mColor, mIsSolid);
	}

private:
	int mX1;
	int mY1;
	int mX2;
	int mY2;
	int mX3;
	int mY3;
	BWAPI::Color mColor;
	bool mIsSolid;
};