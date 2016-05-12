#include "PsiStormAction.h"

#include "UnitTracker.h"
#include "UnitInformation.h"
#include "LatencyTracker.h"
#include "Logger.h"

bool PsiStormAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	if(mUnit->getOrder() == BWAPI::Orders::CastPsionicStorm)
		return true;

	//TODO : Morph arcon temp in here with some non ideal code till group micro actions is complete, checks if it can actually morph the archon,
	// ie not using 3.6
	static bool allowsArchonMorph = true;
	if(allowsArchonMorph)
	{
		if(mUnit->getOrder() == BWAPI::Orders::ArchonWarp)
			return true;

		bool warpPriorityLow = squadGoal.getActionType() != ActionType::Attack || squadGoal.getGoalType() != GoalType::Base || mUnit->getPosition().getApproxDistance(squadGoal.getBase()->getCenterLocation()) > 32*20;
		const int energyToTransformAt = warpPriorityLow ? 48 : 65;
		if(mUnit->getEnergy() <= energyToTransformAt)
		{
			Unit otherTemp;
			int closestDistance = std::numeric_limits<int>::max();
			for each(Unit ht in UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_High_Templar))
			{
				if(ht == mUnit)
					continue;

				if(ht->getOrder() == BWAPI::Orders::ArchonWarp)
					continue;

				if(ht->getEnergy() > energyToTransformAt)
					continue;

				int distance = mUnit->getDistance(ht);
				if(distance < closestDistance)
				{
					closestDistance = distance;
					otherTemp = ht;
				}
			}

			if(otherTemp)
			{
				Unit otherClosestTemp;
				int otherClosestDistance = std::numeric_limits<int>::max();
				for each(Unit ht in UnitTracker::Instance().selectAllUnits(BWAPI::UnitTypes::Protoss_High_Templar))
				{
					if(ht == otherTemp)
						continue;

					if(ht->getOrder() == BWAPI::Orders::ArchonWarp)
						continue;

					if(ht->getEnergy() > energyToTransformAt)
						continue;

					int distance = otherTemp->getDistance(ht);
					if(distance < otherClosestDistance)
					{
						otherClosestDistance = distance;
						otherClosestTemp = ht;
					}
				}

				if(otherClosestTemp == mUnit)
				{
					mUnit->useTech(BWAPI::TechTypes::Archon_Warp, otherTemp);
					otherTemp->useTech(BWAPI::TechTypes::Archon_Warp, mUnit);

					if(BWAPI::Broodwar->getLastError() == BWAPI::Errors::Incompatible_TechType)
					{
						LOGMESSAGEERROR("Archon Warp Failed, Old BWAPI Version?");
						allowsArchonMorph = false;
					}

					return true;
				}
			}
		}
	}

	if(mUnit->getSpellCooldown() > BWAPI::Broodwar->getRemainingLatencyFrames())
		return false;

	if(BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Psionic_Storm) && mUnit->getEnergy() >= BWAPI::TechTypes::Psionic_Storm.energyUsed())
	{
		int numTargetting = UnitInformation::Instance().getUnitsTargetting(mUnit).size();

		UnitGroup targetsToChooseFrom;

		for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
		{
			if(!unit->exists() || unit->isStasised() || unit->isUnderStorm())
				continue;

			const BWAPI::UnitType &type = unit->getType();
			if(type.isBuilding() || type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine || type == BWAPI::UnitTypes::Zerg_Egg || type == BWAPI::UnitTypes::Protoss_Interceptor || type == BWAPI::UnitTypes::Zerg_Larva || type == BWAPI::UnitTypes::Protoss_Scarab)
				continue;

			if(mUnit->getDistance(unit) > BWAPI::TechTypes::Psionic_Storm.getWeapon().maxRange())
				continue;

			targetsToChooseFrom.insert(unit);
		}

		UnitGroup targets(targetsToChooseFrom);

		bool castUrgenty = (numTargetting > 2 || (numTargetting > 0 && mUnit->totalHitPointFraction() < 0.6));

		int tries = 0;
		bool tryAgain = true;
		while(tryAgain)
		{
			tryAgain = false;

			/* There are 3 targets or there are targets and we need to cast urgently */
			if(targets.size() >= 3 || (castUrgenty && !targets.empty()))
			{
				targets = targets.getBestFittingToCircle(64, BWAPI::Broodwar->getRemainingLatencyFrames()+BWAPI::Broodwar->getLatencyFrames());
				if(targets.size() >= 3 || (castUrgenty && !targets.empty()))
				{
					BWAPI::Position castLocation = targets.getCenter();

					int inStormCount = 0;
					for each(Unit unit in UnitTracker::Instance().selectAllUnits())
					{
						if(!unit->isUnderStorm() && unit->getDistance(castLocation) < 64)
							++inStormCount;

						if(inStormCount > 1)
							break;
					}

					if(inStormCount <= 1)
					{
						int distanceToStorm = mUnit->getDistance(castLocation);
						if(distanceToStorm <= BWAPI::TechTypes::Psionic_Storm.getWeapon().maxRange())
						{
							mUnit->useTech(BWAPI::TechTypes::Psionic_Storm, castLocation);
							LatencyTracker::Instance().placingStorm(mUnit, castLocation);
						}
						else
						{
							Vector newCastLocation = mUnit->getPosition() - castLocation;

							newCastLocation.normalise();

							newCastLocation *= float(distanceToStorm - BWAPI::TechTypes::Psionic_Storm.getWeapon().maxRange());

							newCastLocation += Vector(castLocation);

							mUnit->useTech(BWAPI::TechTypes::Psionic_Storm, newCastLocation);
							LatencyTracker::Instance().placingStorm(mUnit, newCastLocation);
						}

						return true;
					}
					else
					{
						++tries;
						if(tries < 3)
						{
							targets = targetsToChooseFrom;
							for each(Unit unit in targetsToChooseFrom)
							{
								if(unit->getDistance(castLocation) < 64)
									targets.erase(unit);
							}

							if(targets.size() >= 3 || (castUrgenty && !targets.empty()))
								tryAgain = true;
						}
					}
				}
			}
		}
	}

	return false;
}