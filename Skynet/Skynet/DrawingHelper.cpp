#include "DrawingHelper.h"

void DrawingHelperClass::drawProgressBar(Position bottomLeft, int width, int height, double progressFraction, BWAPI::Color innerBar, BWAPI::Color outerBar)
{
	BWAPI::Broodwar->drawBoxMap(bottomLeft.x, bottomLeft.y - height, bottomLeft.x + width, bottomLeft.y, outerBar, true);

	int progressWidth = static_cast<int>(progressFraction * double(width));

	BWAPI::Broodwar->drawBoxMap(bottomLeft.x + 1, bottomLeft.y - height + 1, bottomLeft.x + progressWidth - 1, bottomLeft.y - 1, innerBar, true);
}