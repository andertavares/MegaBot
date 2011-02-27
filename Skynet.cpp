#include "Skynet.h"

#include "DrawBuffer.h"

Skynet::Skynet()
{
}

void Skynet::onStart()
{
	BWAPI::Broodwar->sendText("Skynet Operational");

	DrawBuffer::Instance().drawBufferedCircle(BWAPI::CoordinateType::Mouse, 0, 0, 64, 64);
	DrawBuffer::Instance().drawBufferedText(BWAPI::CoordinateType::Mouse, 0, 32, "Hello World!", 64);
}

void Skynet::onEnd(bool isWinner)
{
	if(isWinner)
		BWAPI::Broodwar->sendText("grats me");
}

void Skynet::onFrame()
{
	DrawBuffer::Instance().update();
}