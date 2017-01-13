#include "BuildOrderManager.h"

#include "PlayerTracker.h"

void BuildOrderManagerClass::LoadOtherBuilds()
{
	mBuildOrders[BuildOrderID::Finished] = BuildOrder(BuildOrderID::Finished, "Finished");
	mBuildOrders[BuildOrderID::None] = BuildOrder(BuildOrderID::None, "None");
}