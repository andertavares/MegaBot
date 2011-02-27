#pragma once

#include <BWAPI.h>
#include <boost/ptr_container/ptr_list.hpp>

#include "Singleton.h"
#include "BufferedItem.h"

class DrawBufferClass
{
public:
	DrawBufferClass();

	void update();

	void drawBufferedLine(BWAPI::CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, BufferedCategory::Type ID = BufferedCategory::Default);
	void drawBufferedCircle(BWAPI::CoordinateType::Enum ctype, int x, int y, int radius, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, bool isSolid = false, BufferedCategory::Type ID = BufferedCategory::Default);

	void clearBuffer(BufferedCategory::Type ID);

private:
	std::map<BufferedCategory::Type, boost::ptr_list<BufferedItem>> m_BufferedItems;
};

typedef Singleton<DrawBufferClass> DrawBuffer;