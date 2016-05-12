#include "UnitHelper.h"

#include <algorithm>

bool UnitHelper::unitProducesGround(BWAPI::UnitType type)
{
	static std::set<BWAPI::UnitType> unitData;
	if(unitData.empty())
	{
		for each(BWAPI::UnitType type in BWAPI::UnitTypes::allUnitTypes())
		{
			if(!type.isFlyer() && type.whatBuilds().first.isBuilding())
				unitData.insert(type.whatBuilds().first);
		}
	}

	return unitData.count(type) != 0;
}

class ClusterCompare
{
public:
	ClusterCompare(size_t minSize)
		: mMinSize(minSize)
	{}

	bool operator()(const UnitGroup &set)
	{
		return set.empty() || set.size() < mMinSize;
	}

	size_t mMinSize;
};

std::vector<UnitGroup> UnitHelper::getClusters(const UnitGroup &units, int distance, int minSize)
{
	std::vector<UnitGroup> clusters;

	for each(Unit unit in units)
	{
		std::vector<size_t> clustersInRange;
		for(size_t i = 0; i < clusters.size(); ++i)
		{
			for each(Unit clusterUnit in clusters[i])
			{
				if(unit->getPosition().getApproxDistance(clusterUnit->getPosition()) <= distance)
				{
					clustersInRange.push_back(i);
					break;
				}
			}
		}

		if(clustersInRange.empty())
		{
			UnitGroup newCluster;
			newCluster.insert(unit);
			clusters.push_back(newCluster);
		}
		else
		{
			clusters[clustersInRange[0]].insert(unit);

			for(size_t i = 1; i < clustersInRange.size(); ++i)
			{
				for each(Unit clusterUnit in clusters[clustersInRange[i]])
				{
					clusters[clustersInRange[0]].insert(clusterUnit);
				}
				clusters[clustersInRange[i]].clear();
			}
		}
	}

	clusters.erase(std::remove_if(clusters.begin(), clusters.end(), ClusterCompare(minSize)), clusters.end());

	return clusters;
}

bool UnitHelper::hasAddon(BWAPI::UnitType type) 
{
	return(type == BWAPI::UnitTypes::Terran_Command_Center ||
		type == BWAPI::UnitTypes::Terran_Factory || 
		type == BWAPI::UnitTypes::Terran_Starport ||
		type == BWAPI::UnitTypes::Terran_Science_Facility);
}

Position UnitHelper::tileToPosition(TilePosition tile, BWAPI::UnitType type)
{
	return Position(tile.x()*32+(type.tileWidth()*16), tile.y()*32+(type.tileHeight()*16));
}

bool UnitHelper::isStaticDefense(BWAPI::UnitType type)
{
	return(type == BWAPI::UnitTypes::Protoss_Photon_Cannon ||
		type == BWAPI::UnitTypes::Zerg_Creep_Colony ||
		type == BWAPI::UnitTypes::Zerg_Spore_Colony ||
		type == BWAPI::UnitTypes::Zerg_Sunken_Colony ||
		type == BWAPI::UnitTypes::Terran_Bunker ||
		type == BWAPI::UnitTypes::Terran_Missile_Turret);
}

bool UnitHelper::isArmyUnit(BWAPI::UnitType type)
{
	if(type.isBuilding())
		return false;
	if(type.isSpell())
		return false;
	if(type.isWorker())
		return false;
	if(type == BWAPI::Broodwar->self()->getRace().getSupplyProvider())
		return false;
	if(type == BWAPI::UnitTypes::Zerg_Egg)
		return false;
	if(type == BWAPI::UnitTypes::Protoss_Interceptor)
		return false;
	if(type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		return false;
	if(type == BWAPI::UnitTypes::Zerg_Larva)
		return false;
	if(type == BWAPI::UnitTypes::Protoss_Scarab)
		return false;
	if(type == BWAPI::UnitTypes::Protoss_Observer)
		return false;

	return true;
}

int UnitHelper::getDistance(const Position &pos1, const BWAPI::UnitType type1, const Position &pos2, const BWAPI::UnitType type2)
{
	const int uLeft       = pos1.x() - type1.dimensionLeft();
	const int uTop        = pos1.y() - type1.dimensionUp();
	const int uRight      = pos1.x() + type1.dimensionRight() + 1;
	const int uBottom     = pos1.y() + type1.dimensionDown() + 1;

	const int targLeft    = pos2.x() - type2.dimensionLeft();
	const int targTop     = pos2.y() - type2.dimensionUp();
	const int targRight   = pos2.x() + type2.dimensionRight() + 1;
	const int targBottom  = pos2.y() + type2.dimensionDown() + 1;

	int xDist = uLeft - targRight;
	if(xDist < 0)
	{
		xDist = targLeft - uRight;
		if(xDist < 0)
			xDist = 0;
	}

	int yDist = uTop - targBottom;
	if(yDist < 0)
	{
		yDist = targTop - uBottom;
		if(yDist < 0)
			yDist = 0;
	}

	return Position(0, 0).getApproxDistance(Position(xDist, yDist));
}

int UnitHelper::getDistance(const Position &pos1, const BWAPI::UnitType type1, const Position &pos2)
{
	const int uLeft       = pos1.x() - type1.dimensionLeft();
	const int uTop        = pos1.y() - type1.dimensionUp();
	const int uRight      = pos1.x() + type1.dimensionRight() + 1;
	const int uBottom     = pos1.y() + type1.dimensionDown() + 1;

	int xDist = uLeft - (pos2.x() + 1);
	if(xDist < 0)
	{
		xDist = pos2.x() - uRight;
		if(xDist < 0)
			xDist = 0;
	}

	int yDist = uTop - (pos2.y() + 1);
	if(yDist < 0)
	{
		yDist = pos2.y() - uBottom;
		if(yDist < 0)
			yDist = 0;
	}

	return Position(0, 0).getApproxDistance(Position(xDist, yDist));
}