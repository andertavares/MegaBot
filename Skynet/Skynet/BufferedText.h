#pragma once

#include "Interface.h"

#include "BufferedItem.h"

class BufferedText : public BufferedItem
{
public:
	BufferedText(BWAPI::CoordinateType::Enum coordType, BufferedCategory id, int time, int x, int y, std::string text)
		: BufferedItem(coordType, id, time)
		, mX(x)
		, mY(y)
		, mText(text)
	{}

	virtual void draw()
	{
		BWAPI::Broodwar->drawText(mCoordType, mX, mY, mText.c_str());
	}

private:
	int mX;
	int mY;
	std::string mText;
};