#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedLine : public BufferedItem
{
public:
	BufferedLine(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int x1, int y1, int x2, int y2, BWAPI::Color color)
		: BufferedItem(coordType, id, time)
		, mX1(x1)
		, mY1(y1)
		, mX2(x2)
		, mY2(y2)
		, mColor(color)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawLine(mCoordType, mX1, mY1, mX2, mY2, mColor);
	}

private:
	int mX1;
	int mY1;
	int mX2;
	int mY2;
	BWAPI::Color mColor;
};