#include "BasicUnitAction.h"

#include <limits>

#include "UnitTracker.h"
#include "AOEThreatTracker.h"
#include "UnitHelper.h"

void stayAtRange(Unit unit, Position targetPositon, int maxRange, int currentRange)
{
	Position current = unit->getPosition();

	Vector direction = current - targetPositon;

	direction.normalise();
	direction *= float(maxRange - currentRange);
	direction += Vector(current);

	unit->move(direction);
}

bool BasicUnitAction::update(const Goal &squadGoal, const UnitGroup &squadUnitGroup)
{
	const int seekRange = 448;
	// TODO: Implement a better system, maybe some PFs and other goodies
	// currently a copy paste of 2010 micro

	if(mUnit->isAttackFrame() || mUnit->isStasised() || mUnit->isLockedDown())
		return true;

	const bool isTargetOfThreat = AOEThreatTracker::Instance().isTargetOfThreat(mUnit);
	if(!isTargetOfThreat)
	{
		const AOEThreat &closestThreat = AOEThreatTracker::Instance().getClosestThreat(mUnit);
		if(closestThreat)
		{
			const int distanceToThreat = mUnit->getDistance(closestThreat->getPosition());
			if(distanceToThreat < closestThreat->getRadius()+32)
			{
				stayAtRange(mUnit, closestThreat->getPosition(), closestThreat->getRadius() + 64, distanceToThreat);
				return true;
			}
		}
	}

	const BWAPI::UnitType &actionUnitType = mUnit->getType();

	const bool shouldEngage = squadGoal.getActionType() == ActionType::Attack || squadGoal.getActionType() == ActionType::Defend || squadGoal.getActionType() == ActionType::Hold;

	int currentTargetPriority = std::numeric_limits<int>::max();
	Unit currentTargetUnit;

	int goalTartgetDistance = std::numeric_limits<int>::max();
	int goalTargetPriority = std::numeric_limits<int>::max();
	Unit goalTargetUnit;

	int closestUnitDistance = std::numeric_limits<int>::max();
	int closestUnitRange = 0;
	Unit closestUnit;

	UnitGroup possibleTargets;
	for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
	{
		if(unit->isStasised())
			continue;

		const BWAPI::UnitType &type = unit->getType();
		if(type == BWAPI::UnitTypes::Zerg_Egg || type == BWAPI::UnitTypes::Zerg_Larva || type == BWAPI::UnitTypes::Protoss_Scarab)
			continue;

		if(unit->accessibility() == AccessType::Lost)
			continue;

		if(mUnit->getPosition().getApproxDistance(unit->getPosition()) > seekRange)
			continue;

		if(unit->canAttack(mUnit) || type == BWAPI::UnitTypes::Terran_Science_Vessel)
		{
			const int weaponRange = type == BWAPI::UnitTypes::Terran_Science_Vessel ? BWAPI::TechTypes::EMP_Shockwave.getWeapon().maxRange() : type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine ? BWAPI::UnitTypes::Terran_Vulture_Spider_Mine.seekRange() : unit->getWeaponMaxRange(mUnit);
			const int distance = mUnit->getDistance(unit) - weaponRange;
			if(distance < closestUnitDistance)
			{
				closestUnitRange = weaponRange;
				closestUnitDistance = distance;
				closestUnit = unit;
			}
		}

		if(type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine && actionUnitType == BWAPI::UnitTypes::Protoss_Zealot)
			continue;

		if(!mUnit->canAttackNow(unit))
			continue;

		//TODO: health tracker of some type, if the unit will be dead by the time the bullet hits, continue

		const bool isActionUnitSlower = actionUnitType.topSpeed()*0.7 < type.topSpeed();
		const bool isUnitGoalWorthy = (shouldEngage && squadGoal.getEngageUnits().count(unit) != 0) || unit->isInRange(mUnit) || mUnit->isInRange(unit);

		int unitPriority = isUnitGoalWorthy ? 0 : isActionUnitSlower ? 4 : 2;

		std::list<std::set<BWAPI::UnitType>>::const_iterator it = mTargetPriorities.begin();
		while(it != mTargetPriorities.end())
		{
			if(it->count(type) != 0)
				break;
			else
				++unitPriority;

			++it;
		}

		if(it == mTargetPriorities.end())//Unit not found in the priorities list
		{
			if(UnitHelper::isArmyUnit(type) || UnitHelper::isStaticDefense(type) || type == BWAPI::UnitTypes::Terran_Bunker)
				unitPriority += 1;
			else if(type.isWorker())
				unitPriority += 2;
			else if(type.supplyProvided() > 0)
				unitPriority += 3;
			else
				unitPriority += 4;

			if(!unit->isCompleted())
				unitPriority += 1;
		}

		if(squadGoal.getActionType() != ActionType::Retreat && mUnit->isInRange(unit))
		{
			if(unitPriority < currentTargetPriority || (unitPriority == currentTargetPriority && unit->totalHitPointFraction() < currentTargetUnit->totalHitPointFraction()))
			{
				currentTargetPriority = unitPriority;
				currentTargetUnit = unit;
			}
		}

		if(!isUnitGoalWorthy && isActionUnitSlower)
			continue;

		const int distance = mUnit->getDistance(unit);
		if(unitPriority < goalTargetPriority || (unitPriority == goalTargetPriority && distance < goalTartgetDistance))
		{
			goalTartgetDistance = distance;
			goalTargetPriority = unitPriority;
			goalTargetUnit = unit;
		}
	}

	// If the target of a threat, dont do anything to cause it to move
	if(isTargetOfThreat)
	{
		if(currentTargetUnit)
			mUnit->attack(currentTargetUnit);
		else
			mUnit->stop();
		return true;
	}
	
	const int closestDistance = closestUnit ? mUnit->getDistance(closestUnit) : 0;
	if(closestUnit)
	{
		bool doNotEngageClosest = false;
		if(closestDistance < closestUnitRange + 250)
		{
			if(squadGoal.getAvoidUnits().count(closestUnit) != 0)
				doNotEngageClosest = true;
			else
				doNotEngageClosest = !goalTargetUnit && mUnit->getDistance(squadGoal.getPositionForMerge()) < 140;
		}

		if(doNotEngageClosest)
		{
			stayAtRange(mUnit, closestUnit->getPosition(), closestUnitRange + 290, closestDistance);
			return true;
		}

		bool canAttack = mUnit->canAttackAir() || mUnit->canAttackGround();
		if(actionUnitType == BWAPI::UnitTypes::Protoss_Arbiter)
			canAttack = false;

		if(!canAttack && actionUnitType.isSpellcaster())
		{
			for each(BWAPI::TechType tech in mUnit->getType().abilities())
			{
				if(BWAPI::Broodwar->self()->hasResearched(tech) && mUnit->getEnergy() >= tech.energyUsed())
					canAttack = true;
			}
		}

		int rangeToStayAt = 0;
		if(!canAttack)
			rangeToStayAt = closestUnitRange+32;

		if(actionUnitType.maxEnergy() > 0 && closestUnit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel)
			rangeToStayAt = std::max(rangeToStayAt, BWAPI::TechTypes::EMP_Shockwave.getWeapon().maxRange()+32);

		if(rangeToStayAt != 0 && closestDistance < rangeToStayAt)
		{
			stayAtRange(mUnit, closestUnit->getPosition(), rangeToStayAt, closestDistance);
			return true;
		}
	}

	const bool canAttackCurrentTarget = currentTargetUnit && mUnit->getRemainingCooldown(currentTargetUnit) <= BWAPI::Broodwar->getRemainingLatencyFrames();
	const bool isCurrentTargetImportant = currentTargetUnit && (UnitHelper::isArmyUnit(currentTargetUnit->getType()) || UnitHelper::isStaticDefense(currentTargetUnit->getType()));
	const bool goalBetterThanCurrent = canAttackCurrentTarget && goalTargetPriority < currentTargetPriority && !isCurrentTargetImportant;

	if(goalBetterThanCurrent)
	{
		mUnit->attack(goalTargetUnit);
		return true;
	}
	else if(canAttackCurrentTarget)
	{
		mUnit->attack(currentTargetUnit);
		return true;
	}

	if(closestUnit && !goalTargetUnit && closestUnit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
	{
		if(closestDistance < BWAPI::UnitTypes::Terran_Vulture_Spider_Mine.seekRange()+32)
		{
			stayAtRange(mUnit, closestUnit->getPosition(), BWAPI::UnitTypes::Terran_Vulture_Spider_Mine.seekRange()+32, closestDistance);
			return true;
		}
	}

	if(closestUnit && closestUnit->exists() && currentTargetUnit)
	{
		if(mUnit->canAttack(closestUnit))
		{
			const int myRange = mUnit->getWeaponMaxRange(closestUnit);
			if(closestDistance < myRange && myRange > 60)
			{
				stayAtRange(mUnit, closestUnit->getPosition(), myRange, closestDistance);
				return true;
			}
		}
	}

	if(goalTargetUnit && goalTargetPriority < currentTargetPriority)
	{
		mUnit->attack(goalTargetUnit);
		return true;
	}

	if(!canAttackCurrentTarget)
	{
		if(squadGoal.getActionType() == ActionType::Attack)
		{
			if(!isCurrentTargetImportant && mUnit->getDistance(currentTargetUnit) > 5)
			{
				mUnit->move(currentTargetUnit->getPosition());
				return true;
			}
		}
	}

	if(currentTargetUnit)
	{
		mUnit->attack(currentTargetUnit);
		return true;
	}

	if(closestUnit && mUnit->canAttack(closestUnit))
	{
		mUnit->attack(closestUnit);
		return true;
	}

	return false;
}