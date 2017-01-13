#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedEllipse : public BufferedItem
{
public:
	BufferedEllipse(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int x, int y, int radiusX, int radiusY, BWAPI::Color color, bool solid)
		: BufferedItem(coordType, id, time)
		, mX(x)
		, mY(y)
		, mRadiusX(radiusX)
		, mRadiusY(radiusY)
		, mColor(color)
		, mIsSolid(solid)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawEllipse(mCoordType, mX, mY, mRadiusX, mRadiusY, mColor, mIsSolid);
	}

private:
	int mX;
	int mY;
	int mRadiusX;
	int mRadiusY;
	BWAPI::Color mColor;
	bool mIsSolid;
};