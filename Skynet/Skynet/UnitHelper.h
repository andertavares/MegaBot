#pragma once

#include "Interface.h"

using namespace SkynetBot;

namespace UnitHelper
{
	bool unitProducesGround(BWAPI::UnitType type);

	std::vector<UnitGroup> getClusters(const UnitGroup &units, int distance, int minSize);

	bool hasAddon(BWAPI::UnitType type);
	bool isStaticDefense(BWAPI::UnitType type);
	bool isArmyUnit(BWAPI::UnitType type);

	Position tileToPosition(TilePosition tile, BWAPI::UnitType type);

	int getDistance(const Position &pos1, const BWAPI::UnitType type1, const Position &pos2, const BWAPI::UnitType type2);
	int getDistance(const Position &pos1, const BWAPI::UnitType type1, const Position &pos2);
}