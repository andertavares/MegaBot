#pragma once

#include "Interface.h"
#include <boost/ptr_container/ptr_map.hpp>

#include "Singleton.h"
#include "BufferedItem.h"

class DrawBufferClass
{
public:
	DrawBufferClass();

	void update();

	void drawBufferedText(BWAPI::CoordinateType::Enum ctype, int x, int y, std::string text, int frameTime, BufferedCategory ID = BufferedCategory::Default);
	void drawBufferedBox(BWAPI::CoordinateType::Enum ctype, int left, int top, int right, int bottom, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, bool isSolid = false, BufferedCategory ID = BufferedCategory::Default);
	void drawBufferedCircle(BWAPI::CoordinateType::Enum ctype, int x, int y, int radius, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, bool isSolid = false, BufferedCategory ID = BufferedCategory::Default);
	void drawBufferedEllipse(BWAPI::CoordinateType::Enum ctype, int x, int y, int radiusX, int radiusY, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, bool isSolid = false, BufferedCategory ID = BufferedCategory::Default);
	void drawBufferedDot(BWAPI::CoordinateType::Enum ctype, int x, int y, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, BufferedCategory ID = BufferedCategory::Default);
	void drawBufferedLine(BWAPI::CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, BufferedCategory ID = BufferedCategory::Default);
	void drawBufferedTriangle(BWAPI::CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int x3, int y3, int frameTime, BWAPI::Color color = BWAPI::Colors::Green, bool isSolid = false, BufferedCategory ID = BufferedCategory::Default);
	

	void clearBuffer(BufferedCategory ID);

private:
	boost::ptr_multimap<BufferedCategory, BufferedItem> m_BufferedItems;
};

typedef Singleton<DrawBufferClass> DrawBuffer;