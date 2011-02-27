#include "DrawBuffer.h"

#include "BufferedCircle.h"
#include "BufferedLine.h"

DrawBufferClass::DrawBufferClass()
{
}

void DrawBufferClass::update()
{
	for(std::map<BufferedCategory::Type, boost::ptr_list<BufferedItem>>::iterator categoriesIT = m_BufferedItems.begin(); categoriesIT != m_BufferedItems.end(); ++categoriesIT)
	{
		for(boost::ptr_list<BufferedItem>::iterator itemsIT = categoriesIT->second.begin(); itemsIT != categoriesIT->second.end();)
		{
			if(itemsIT->isTimeUp())
			{
				itemsIT = categoriesIT->second.erase(itemsIT);
			}
			else
			{
				itemsIT->draw();
				++itemsIT;
			}
		}
	}
}

void DrawBufferClass::drawBufferedLine(BWAPI::CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, int frameTime, BWAPI::Color color, BufferedCategory::Type ID)
{
	m_BufferedItems[ID].push_back(new BufferedLine(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount() - 1, x1, y1, x2, y2, color));
}

void DrawBufferClass::drawBufferedCircle(BWAPI::CoordinateType::Enum ctype, int x, int y, int radius, int frameTime, BWAPI::Color color, bool isSolid, BufferedCategory::Type ID)
{
	m_BufferedItems[ID].push_back(new BufferedCircle(ctype, ID, frameTime + BWAPI::Broodwar->getFrameCount() - 1, x, y, radius, color, isSolid));
}

void DrawBufferClass::clearBuffer(BufferedCategory::Type ID)
{
	m_BufferedItems[ID].clear();
}