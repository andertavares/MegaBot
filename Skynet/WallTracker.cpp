#include "WallTracker.h"

#include <regex>

#include "UnitTracker.h"
#include "BuildingPlacer.h"
#include "DrawBuffer.h"
#include "MapHelper.h"
#include "Logger.h"

void WallTrackerClass::onBegin()
{
	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
	{
		if(MapHelper::mapIs("Python"))
		{
			LOGMESSAGE("Python Detected");
			if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(83, 6))//12 o'clock
			{
				LOGMESSAGE("12 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(58, 12));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(57, 14));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(54, 11));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(51, 10));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(58, 10));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(116, 40))//3 o'clock
			{
				LOGMESSAGE("3 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(114, 62));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(111, 62));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(108, 59));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(117, 64));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(112, 60));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(42, 119))//6 o'clock
			{
				LOGMESSAGE("6 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(73, 117));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(66, 116));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(68, 113));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(69, 116));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(73, 119));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(8, 85))//9 o'clock
			{
				LOGMESSAGE("9 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(7, 63));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(12, 65));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(11, 62));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(9, 60));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(11, 59));
			}
			else
			{
				LOGMESSAGE("Error 1");
			}
		}
		else if(MapHelper::mapIs("Destination"))
		{
			LOGMESSAGE("Destination Detected");
			if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(31, 7))//11 o'clock
			{
				LOGMESSAGE("11 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(60, 18));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(59, 22));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(56, 19));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(64, 17));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(60, 20));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(64, 118))//5 o'clock
			{
				LOGMESSAGE("5 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(35, 110));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(39, 110));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(36, 107));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(31, 110));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(37, 110));
			}
			else
			{
				LOGMESSAGE("Error 1");
			}
		}
		else if(MapHelper::mapIs("Heartbreak"))
		{
			LOGMESSAGE("Heartbreak Ridge Detected");
			if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(117, 56))//2 o'clock
			{
				LOGMESSAGE("2 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(108, 29));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(104, 27));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(106, 24));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(108, 31));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(107, 27));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(7, 37))//8 o'clock
			{
				LOGMESSAGE("8 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(18, 66));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(18, 70));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(20, 67));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(18, 64));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(18, 68));
			}
			else
			{
				LOGMESSAGE("Error 1");
			}
		}
		else if(MapHelper::mapIs("Tau"))
		{
			LOGMESSAGE("Tau Cross Detected");
			if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(117, 9))//2 o'clock
			{
				LOGMESSAGE("2 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(102, 43));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(104, 44));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(108, 43));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(105, 42));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(108, 41));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(93, 118))//5 o'clock
			{
				LOGMESSAGE("5 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(55, 108));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(50, 108));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(52, 105));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(53, 108));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(55, 110));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(7, 44))//10 o'clock
			{
				LOGMESSAGE("10 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(24, 15));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(26, 19));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(26, 16));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(24, 17));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(22, 15));
			}
			else
			{
				LOGMESSAGE("Error 1");
			}
		}
		else if(MapHelper::mapIs("Andromeda"))
		{
			LOGMESSAGE("Andromeda Detected");
			if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(117, 7))//1 o'clock
			{
				LOGMESSAGE("1 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(98, 28));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(103, 31));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(100, 28));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(104, 29));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(103, 24));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(117, 119))//5 o'clock
			{
				LOGMESSAGE("5 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(99, 100));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(101, 99));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(102, 96));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(104, 99));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(104, 103));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(7, 118))//7 o'clock
			{
				LOGMESSAGE("7 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(27, 100));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(24, 99));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(22, 96));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(22, 99));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(22, 103));
			}
			else if(BWAPI::Broodwar->self()->getStartLocation() == TilePosition(7, 6))//11 o'clock
			{
				LOGMESSAGE("11 o'clock");
				mWallPositions[BWAPI::UnitTypes::Protoss_Pylon].push_back(TilePosition(28, 28));
				mWallPositions[BWAPI::UnitTypes::Protoss_Forge].push_back(TilePosition(22, 31));
				mWallPositions[BWAPI::UnitTypes::Protoss_Gateway].push_back(TilePosition(24, 28));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(22, 29));
				mWallPositions[BWAPI::UnitTypes::Protoss_Photon_Cannon].push_back(TilePosition(23, 24));
			}
			else
			{
				LOGMESSAGE("Error 1");
			}
		}
		else
		{
			LOGMESSAGE("Error 2");
		}

		if(!mWallPositions.empty())
		{
			bool oneHasFailed = false;
			for(std::map<BWAPI::UnitType, std::list<TilePosition>>::iterator it = mWallPositions.begin(); it != mWallPositions.end(); ++it)
			{
				for(std::list<TilePosition>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
				{
					if(!BuildingPlacer::Instance().isTileBuildable(*it2, it->first, true))
						oneHasFailed = true;
				}
			}

			if(oneHasFailed)
			{
				LOGMESSAGE("Error 3");
				mWallPositions.clear();
			}
		}
	}
}

TilePosition WallTrackerClass::getWallPosition(BWAPI::UnitType type)
{
	for each(TilePosition tile in mWallPositions[type])
	{
		if(BuildingPlacer::Instance().isReserved(tile))
			continue;

		int isUsed = false;
		for each(Unit unit in UnitTracker::Instance().getUnitsOnTile(tile.x(), tile.y()))
		{
			if(unit->getType().isBuilding())
				isUsed = true;
		}

		if(!isUsed)
			return tile;
	}

	return BWAPI::TilePositions::None;
}

bool WallTrackerClass::canForgeExpand()
{
	return !mWallPositions.empty();
}

std::map<TilePosition, BWAPI::UnitType> WallTrackerClass::getWallTiles()
{
	std::map<TilePosition, BWAPI::UnitType> tiles;

	for each(std::pair<BWAPI::UnitType, std::list<TilePosition>> wallPair in mWallPositions)
	{
		for each(TilePosition tile in wallPair.second)
		{
			for(int x = tile.x(); x < tile.x()+wallPair.first.tileWidth(); ++x)
				for(int y = tile.y(); y < tile.y()+wallPair.first.tileHeight(); ++y)
					tiles[TilePosition(x, y)] = wallPair.first;
		}
	}

	return tiles;
}