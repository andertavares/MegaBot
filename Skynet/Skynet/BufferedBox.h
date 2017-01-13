#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedBox : public BufferedItem
{
public:
	BufferedBox(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int left, int top, int right, int bottom, BWAPI::Color color, bool solid)
		: BufferedItem(coordType, id, time)
		, mLeft(left)
		, mTop(top)
		, nRight(right)
		, mBottom(bottom)
		, mColor(color)
		, mIsSolid(solid)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawBox(mCoordType, mLeft, mTop, nRight, mBottom, mColor, mIsSolid);
	}

private:
	int mLeft;
	int mTop;
	int nRight;
	int mBottom;
	BWAPI::Color mColor;
	bool mIsSolid;
};