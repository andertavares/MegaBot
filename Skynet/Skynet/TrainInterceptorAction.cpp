#include "TrainInterceptorAction.h"

using namespace SkynetBot;

bool TrainInterceptorAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	int maxAmout = 4;
	if(BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Carrier_Capacity) > 0)
		maxAmout += 4;

	if(mUnit->getTrainingQueue().size() < 5 && mUnit->getInterceptorCount() + (int)mUnit->getTrainingQueue().size() < maxAmout)
	{
		if(BWAPI::Broodwar->self()->minerals() >= BWAPI::UnitTypes::Protoss_Interceptor.mineralPrice())
		{
			mUnit->train(BWAPI::UnitTypes::Protoss_Interceptor);
			return true;
		}
	}

	return false;
}