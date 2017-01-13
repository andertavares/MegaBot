#include "DrawBuffer.h"

#include "BufferedText.h"
#include "BufferedBox.h"
#include "BufferedCircle.h"
#include "BufferedEllipse.h"
#include "BufferedDot.h"
#include "BufferedLine.h"
#include "BufferedTriangle.h"

DrawBufferClass::DrawBufferClass()
{
}

void DrawBufferClass::update()
{
	for(boost::ptr_multimap<BufferedCategory, BufferedItem>::iterator it = m_BufferedItems.begin(); it != m_BufferedItems.end();)
	{
		if((*it).second->isTimeUp())
		{
			m_BufferedItems.erase(it++);
		}
		else
		{
			(*it).second->draw();
			++it;
		}
	}
}


void DrawBufferClass::drawBufferedText(BWAPI::CoordinateType::Enum ctype, int x, int y, std::string text, int frameTime, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedText(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), x, y, text));
}

void DrawBufferClass::drawBufferedBox(BWAPI::CoordinateType::Enum ctype, int left, int top, int right, int bottom, int frameTime, BWAPI::Color color, bool isSolid, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedBox(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), left, top, right, bottom, color, isSolid));
}

void DrawBufferClass::drawBufferedCircle(BWAPI::CoordinateType::Enum ctype, int x, int y, int radius, int frameTime, BWAPI::Color color, bool isSolid, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedCircle(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), x, y, radius, color, isSolid));
}

void DrawBufferClass::drawBufferedEllipse(BWAPI::CoordinateType::Enum ctype, int x, int y, int radiusX, int radiusY, int frameTime, BWAPI::Color color, bool isSolid, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedEllipse(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), x, y, radiusX, radiusY, color, isSolid));
}

void DrawBufferClass::drawBufferedDot(BWAPI::CoordinateType::Enum ctype, int x, int y, int frameTime, BWAPI::Color color, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedDot(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), x, y, color));
}

void DrawBufferClass::drawBufferedLine(BWAPI::CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int frameTime, BWAPI::Color color, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedLine(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), x1, y1, x2, y2, color));
}

void DrawBufferClass::drawBufferedTriangle(BWAPI::CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int x3, int y3, int frameTime, BWAPI::Color color, bool isSolid, BufferedCategory ID)
{
	m_BufferedItems.insert(ID, new BufferedTriangle(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount(), x1, y1, x2, y2, x3, y3, color, isSolid));
}

void DrawBufferClass::clearBuffer(BufferedCategory ID)
{
	m_BufferedItems.erase(ID);
}
