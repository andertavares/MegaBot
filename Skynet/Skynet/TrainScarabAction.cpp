#include "TrainScarabAction.h"

bool TrainScarabAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	if(mUnit->isLoaded())
		return false;

	int maxAmout = 5;
	if(BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Reaver_Capacity) > 0)
		maxAmout += 5;

	if(mUnit->getTrainingQueue().size() < 5 && mUnit->getScarabCount() + (int)mUnit->getTrainingQueue().size() < maxAmout)
	{
		if(BWAPI::Broodwar->self()->minerals() >= BWAPI::UnitTypes::Protoss_Scarab.mineralPrice())
		{
			mUnit->train(BWAPI::UnitTypes::Protoss_Scarab);
			return true;
		}
	}

	return false;
}