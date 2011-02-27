#include "Skynet.h"

Skynet::Skynet()
{
}

void Skynet::onStart()
{
	BWAPI::Broodwar->sendText("Skynet Operational");
}