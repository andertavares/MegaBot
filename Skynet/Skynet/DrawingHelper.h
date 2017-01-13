#pragma once

#include "Interface.h"

#include "Singleton.h"

class DrawingHelperClass
{
public:
	DrawingHelperClass(){}

	void drawProgressBar(Position bottomLeft, int width, int height, double progressFraction, BWAPI::Color innerBar = BWAPI::Colors::Green, BWAPI::Color outerBar = BWAPI::Colors::Black);
};

typedef Singleton<DrawingHelperClass> DrawingHelper;