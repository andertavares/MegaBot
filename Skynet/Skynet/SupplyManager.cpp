#include "SupplyManager.h"

#include "BuildOrderManager.h"
#include "ResourceTracker.h"
#include "TaskManager.h"

void SupplyManagerClass::update()
{
	if(!BuildOrderManager::Instance().getOrder(Order::SupplyManager))
		return;

	const int &supplyTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->self()->getRace().getSupplyProvider().buildTime() + 150;
	if(ResourceTracker::Instance().totalSupplyAtTime(supplyTime) < 400)
	{
		const int currentSupply = BWAPI::Broodwar->self()->supplyUsed();
		const int &freeSupply = ResourceTracker::Instance().availableSupplyAtTime(supplyTime);
		if(freeSupply <= (currentSupply > 120 ? 16 : currentSupply > 60 ? 8 : 0))
		{
			if(!mLastItem || mLastItem->hasDispatched() || mLastItem->hasEnded())
				mLastItem = TaskManager::Instance().build(BWAPI::Broodwar->self()->getRace().getSupplyProvider(), TaskType::Supply);
		}
	}
}
