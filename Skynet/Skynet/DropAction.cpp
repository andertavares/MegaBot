#include "DropAction.h"

using namespace SkynetBot;

bool DropAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	return false;
}

void DropAction::removeUnit(Unit unit)
{
	mUnits.erase(unit);

	if(unit != mShuttle)
		mFreeSpace += unit->getType().spaceRequired();
	else
		mShuttle = StaticUnits::nullunit;
}

void DropAction::addUnit(Unit unit)
{
	mUnits.insert(unit);

	mFreeSpace -= unit->getType().spaceRequired();
}

bool DropAction::requestingExtraUnits()
{
	return mFreeSpace > 0;
}

std::vector<std::map<BWAPI::UnitType, int>> DropAction::getRequestedUnits()
{
	std::vector<std::map<BWAPI::UnitType, int>> returnData;

	int maxNumOfBig = mFreeSpace / BWAPI::UnitTypes::Protoss_Reaver.spaceRequired();
	int maxNumOfSmall = mFreeSpace / BWAPI::UnitTypes::Protoss_Zealot.spaceRequired();

	int currentBig = maxNumOfBig;
	int currentSmall = 0;

	for(;;)
	{
		std::map<BWAPI::UnitType, int> currentData;

		currentData[BWAPI::UnitTypes::Protoss_Reaver] = currentBig;
		currentData[BWAPI::UnitTypes::Protoss_Zealot] = currentSmall;

		int spaceForSmall = mFreeSpace;
		spaceForSmall -= currentBig * BWAPI::UnitTypes::Protoss_Reaver.spaceRequired();
		spaceForSmall -= currentBig * BWAPI::UnitTypes::Protoss_Zealot.spaceRequired();

		if(spaceForSmall >= BWAPI::UnitTypes::Protoss_Zealot.spaceRequired())
			++currentSmall;
		else if(currentBig == 0)
			break;
		else
		{
			currentSmall = 0;
			--currentBig;
		}
	}

	return returnData;
}

bool DropAction::finishedWithAUnit()
{
	return false;
}

UnitGroup DropAction::getFinishedUnits()
{
	return UnitGroup();
}