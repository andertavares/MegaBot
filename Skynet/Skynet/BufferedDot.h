#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedDot : public BufferedItem
{
public:
	BufferedDot(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int x, int y, BWAPI::Color color)
		: BufferedItem(coordType, id, time)
		, mX(x)
		, mY(y)
		, mColor(color)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawDot(mCoordType, mX, mY, mColor);
	}

private:
	int mX;
	int mY;
	BWAPI::Color mColor;
};