#include "ArbiterAction.h"

#include <limits>

#include "UnitTracker.h"
#include "UnitInformation.h"
#include "UnitHelper.h"
#include "LatencyTracker.h"

using namespace SkynetBot;

bool ArbiterAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	if(mUnit->getOrder() == BWAPI::Orders::CastStasisField)
		return true;

	if(mUnit->getOrder() == BWAPI::Orders::CastRecall)
		return true;

	if(mUnit->getEnergy() >= 100 && BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stasis_Field))
	{
		const int stasisSize = 48;

		bool stasisUrgently = false;
		if(UnitInformation::Instance().getUnitsTargetting(mUnit).size() >= 6 || (UnitInformation::Instance().getUnitsTargetting(mUnit).size() >= 1 && mUnit->totalHitPointFraction() < 0.3))
			stasisUrgently = true;

		UnitGroup stasisChoices;
		for each(Unit enemy in UnitTracker::Instance().selectAllEnemy())
		{
			if(!UnitHelper::isArmyUnit(enemy->getType()))
				continue;

			if(enemy->isUnderStorm() || enemy->isStasised())
				continue;

			const int distance = mUnit->getDistance(enemy);
			if(distance > 250 || distance < stasisSize)
				continue;

			stasisChoices.insert(enemy);
		}

		if(stasisChoices.size() > 4 || (!stasisChoices.empty() && stasisUrgently))
		{
			UnitGroup stasisTargets = stasisChoices.getBestFittingToCircle(stasisSize);

			if(stasisTargets.size() > 4 || (!stasisTargets.empty() && stasisUrgently))
			{
				const Position &stasisLocation = stasisTargets.getCenter();
				if(mUnit->getDistance(stasisLocation) <= BWAPI::TechTypes::Stasis_Field.getWeapon().maxRange())
				{
					mUnit->useTech(BWAPI::TechTypes::Stasis_Field, stasisLocation);
					LatencyTracker::Instance().placingStasis(mUnit, stasisLocation);
					return true;
				}
				else
				{
					mUnit->move(stasisLocation);
					return true;
				}
			}
		}
	}

	UnitGroup unitsToCloak;
	for each(Unit unit in squadUnitGroup)
	{
		if(!UnitHelper::isArmyUnit(unit->getType()))
			continue;

		if(unit->getType() == BWAPI::UnitTypes::Protoss_Arbiter || unit->getType().isBuilding())
			continue;

		if(mUnit->getDistance(unit) > 250)
			continue;

		unitsToCloak.insert(unit);
	}

	if(!unitsToCloak.empty())
	{
		unitsToCloak = unitsToCloak.getBestFittingToCircle(136);
		if(!unitsToCloak.empty())
		{
			Position cloakLocation = unitsToCloak.getCenter();
			if(mUnit->getDistance(cloakLocation) > 110)
			{
				mUnit->move(cloakLocation);
				return true;
			}
		}
	}

	return false;
}