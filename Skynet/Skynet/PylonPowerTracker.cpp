#include "PylonPowerTracker.h"

#include "BuildingPlacer.h"

void PylonPowerTrackerClass::update()
{
	if(mPylons.empty())
		return;

	for each(Unit pylon in mPylons)
	{
		if(pylon->isCompleted() && mPylonSmallPowerSites.count(pylon) == 0)
			addToPowerField(pylon);
	}
}

void PylonPowerTrackerClass::onDiscover(Unit unit)
{
	if(unit->getPlayer() == BWAPI::Broodwar->self() && unit->getType() == BWAPI::UnitTypes::Protoss_Pylon)
		onAddPylon(unit);
}

void PylonPowerTrackerClass::onMorphRenegade(Unit unit, Player previousPlayer, BWAPI::UnitType previousType)
{
	onRemovePylon(unit);
}

void PylonPowerTrackerClass::onDestroy(Unit unit)
{
	onRemovePylon(unit);
}

void PylonPowerTrackerClass::onAddPylon(Unit unit)
{
	if(mPylons.count(unit) != 0)
		return;

	mPylons.insert(unit);

	if(unit->isCompleted())
		addToPowerField(unit);
}

void PylonPowerTrackerClass::onRemovePylon(Unit unit)
{
	if(mPylons.count(unit) == 0)
		return;

	mPylons.erase(unit);

	removeFromPowerField(unit);
}

void PylonPowerTrackerClass::addToPowerField(Unit unit)
{
	const TilePosition &pylonTile = unit->getTilePosition();
	for(int x = 0; x <= 15; ++x)
	{
		for(int y = 0; y <= 9; ++y)
		{
			bool inRangeSmall = false;
			bool inRangeMedium = false;
			bool inRangeLarge = false;

			switch(y)
			{
			case 0:
				if(x >= 4 && x <= 9)
					inRangeLarge = true;
				break;
			case 1:
			case 8:
				if(x >= 2 && x <= 13)
				{
					inRangeSmall = true;
					inRangeMedium = true;
				}
				if(x >= 1 && x <= 12)
					inRangeLarge = true;
				break;
			case 2:
			case 7:
				if(x >= 1 && x <= 14)
				{
					inRangeSmall = true;
					inRangeMedium = true;
				}
				if(x <= 13)
					inRangeLarge = true;
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				if(x >= 1)
					inRangeSmall = true;
				inRangeMedium = true;
				if(x <= 14)
					inRangeLarge = true;
				break;
			case 9:
				if(x >= 5 && x <= 10) 
				{
					inRangeSmall = true;
					inRangeMedium = true;
				}
				if(x >= 4 && x <= 9)
					inRangeLarge = true;
				break;
			}

			const TilePosition tile(pylonTile.x + x - 8, pylonTile.y + y - 5);

			if(inRangeSmall)
			{
				mSmallPowerSites[tile]++;
				mPylonSmallPowerSites[unit].insert(tile);
			}

			if(inRangeMedium)
			{
				mMediumPowerSites[tile]++;
				mPylonMediumPowerSites[unit].insert(tile);
			}

			if(inRangeLarge)
			{
				mLargePowerSites[tile]++;
				mPylonLargePowerSites[unit].insert(tile);
			}
		}
	}
}

void PylonPowerTrackerClass::removeFromPowerField(Unit unit)
{
	for each(TilePosition tile in mPylonSmallPowerSites[unit])
	{
		std::map<TilePosition, int>::iterator it = mSmallPowerSites.find(tile);
		--it->second;

		if(it->second == 0)
			mSmallPowerSites.erase(it);
	}
	mPylonSmallPowerSites.erase(unit);

	for each(TilePosition tile in mPylonMediumPowerSites[unit])
	{
		std::map<TilePosition, int>::iterator it = mMediumPowerSites.find(tile);
		--it->second;

		if(it->second == 0)
			mMediumPowerSites.erase(it);
	}
	mPylonMediumPowerSites.erase(unit);

	for each(TilePosition tile in mPylonLargePowerSites[unit])
	{
		std::map<TilePosition, int>::iterator it = mLargePowerSites.find(tile);
		--it->second;

		if(it->second == 0)
			mLargePowerSites.erase(it);
	}
	mPylonLargePowerSites.erase(unit);
}

bool PylonPowerTrackerClass::hasPower(TilePosition tile, BWAPI::UnitType unit)
{
	if(unit.tileHeight() == 2 && unit.tileWidth() == 2)
		return mSmallPowerSites.count(tile) != 0;
	else if(unit.tileHeight() == 2 && unit.tileWidth() == 3)
		return mMediumPowerSites.count(tile) != 0;
	else if(unit.tileHeight() == 3 && unit.tileWidth() == 4)
		return mLargePowerSites.count(tile) != 0;

	return false;
}