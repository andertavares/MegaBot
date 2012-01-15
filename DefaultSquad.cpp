#include "DefaultSquad.h"

#include "UnitTracker.h"
#include "BaseTracker.h"
#include "BorderTracker.h"
#include "BuildOrderManager.h"
#include "PlayerTracker.h"
#include "UnitHelper.h"
#include "GameProgressDetection.h"

DefaultSquadTask::DefaultSquadTask(ArmyBehaviour behaviour)
	: BaseSquadTask(behaviour)
	, mEngageFull(false)
{
}

int DefaultSquadTask::getEndTime() const
{
	return Requirement::maxTime;
}

int DefaultSquadTask::getEndTime(Unit unit) const
{
	return Requirement::maxTime;
}

int DefaultSquadTask::getPriority(Unit unit) const
{
	return unit->getType().isWorker() ? 5 : 20;
}

Position DefaultSquadTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position DefaultSquadTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool DefaultSquadTask::preUpdate()
{
	updateRequirements();

	return hasEnded();
}

BorderPosition getLargestChoke(const std::set<BorderPosition> &chokes)
{
	int largestChokeSize = 0;
	BorderPosition largestChoke;
	for each(BorderPosition border in chokes)
	{
		int chokeSize = border.mChoke->getClearance();
		if(chokeSize > largestChokeSize)
		{
			largestChoke = border;
			largestChokeSize = chokeSize;
		}
	}

	return largestChoke;
}

bool DefaultSquadTask::update()
{
	Goal squadsGoal;

	// Just attack if its not mining anywhere
	bool hasMiningBases = false;
	int techStructures = 0;
	Base baseToDefend;
	bool baseUnderAttack = false;
	const int mySupply = BWAPI::Broodwar->self()->supplyUsed();
	for each(Base base in BaseTracker::Instance().getPlayerBases())
	{
		const int techBuildings = base->getNumberOfTechBuildings();
		if(!base->getEnemyThreats().empty() && (techBuildings > 0 || (!base->isMinedOut() && base->getResourceDepot())))
		{
			bool thisUnderAttack = base->isUnderAttack();
			if((thisUnderAttack && !baseUnderAttack) || (thisUnderAttack == baseUnderAttack && techBuildings > techStructures))
			{
				baseUnderAttack = thisUnderAttack;
				baseToDefend = base;
				techStructures = techBuildings;
			}
		}

		if(!base->getMinerals().empty() && base->getResourceDepot() && base->getResourceDepot()->exists())
			hasMiningBases = true;
	}

	UnitGroup avoidGroup;
	UnitGroup engageGroup;
	for each(const UnitGroup &unitGroup in PlayerTracker::Instance().getEnemyClusters())
	{
		if(!hasMiningBases && mUnits.canMajorityAttack(unitGroup))
		{
			engageGroup += unitGroup;
			continue;
		}

		if(mUnits.minDistanceBetween(unitGroup) > 540)
			continue;

		const int ratingDiff = mUnits.ratingDifference(unitGroup);
		if(mySupply < 360 && ratingDiff < -2000)
			avoidGroup += unitGroup;
		else if(mUnits.canMajorityAttack(unitGroup) && (mUnits.isWorthEngaging(unitGroup) || unitGroup.isAnyInRange(mUnits)))
			engageGroup += unitGroup;
	}

	mEngageFull = !engageGroup.empty();
	if(mEngageFull)
		squadsGoal = Goal(ActionType::Attack, engageGroup, avoidGroup);
	else if(baseUnderAttack)
	{
		const int rating = mUnits.ratingDifference(baseToDefend->getEnemyThreats());
		if(rating > -1000)
			squadsGoal = Goal(ActionType::Defend, baseToDefend->getEnemyThreats(), avoidGroup);
	}

	if(squadsGoal.getGoalType() == GoalType::None)
	{
		for each(Base base in BaseTracker::Instance().getPlayerBases())
		{
			if(base->isContested())
			{
				for each(Unit building in base->getBuildings())
				{
					if(BWAPI::Broodwar->self()->isEnemy(building->getPlayer()))
					{
						squadsGoal = Goal(ActionType::Attack, building, engageGroup, avoidGroup);
						break;
					}
				}
			}

			if(squadsGoal.getGoalType() != GoalType::None)
				break;
		}
	}

	if(squadsGoal.getGoalType() == GoalType::None)
	{
		int enemyKnownSupply = 0;
		int enemyGuessSupply = 0;

		for each(Unit unit in UnitTracker::Instance().selectAllEnemy())
		{
			int supply = unit->getType().supplyRequired();
			if(UnitHelper::isStaticDefense(unit->getType()))
				supply = 7;

			if(unit->accessibility() != AccessType::Lost)
				enemyKnownSupply += supply;

			enemyGuessSupply += supply;
		}

		int alliesSize = 1;
		std::map<BWAPI::Force*, int> enemyForces;
		for each(Player player in BWAPI::Broodwar->getPlayers())
		{
			if(player == BWAPI::Broodwar->self())
				continue;

			if(BWAPI::Broodwar->self()->isAlly(player))
				++alliesSize;

			if(BWAPI::Broodwar->self()->isEnemy(player))
				++enemyForces[player->getForce()];
		}

		int maxEnemiesSize = 0;
		for each(std::pair<BWAPI::Force*, int> force in enemyForces)
		{
			if(force.second > maxEnemiesSize)
				maxEnemiesSize = force.second;
		}

		bool shouldAttackBase = !hasMiningBases;
		if(mUnits.size() >= 3)
		{
			if(mySupply > 380)
				shouldAttackBase = true;
			else if(GameProgressDetection::Instance().getState() != StateType::TechHigh)
			{
				// If its a terran with siege tanks don't attack a base without leg speed and also ensure we are better matched as siege tanks can hurt
				bool isTerranWithSiege = PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Terran) && PlayerTracker::Instance().enemyHasReseached(BWAPI::TechTypes::Tank_Siege_Mode);
				if(isTerranWithSiege && BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Leg_Enhancements) > 0 && mUnits.size() > 45 && mySupply > (enemyGuessSupply * 4))
					shouldAttackBase = true;
				else if(!isTerranWithSiege && mUnits.size() > 35 && mySupply > (enemyKnownSupply * 4))
					shouldAttackBase = true;
				else if(mArmyBehaviour <= ArmyBehaviour::Aggresive)
				{
					if(maxEnemiesSize <= alliesSize)
						shouldAttackBase = true;
				}

				// Agains't weak opponents it can usually attack when these pass
				// how ever against stronger opponents, or terran with siege tanks it will just lose a few units time and again then lose to a push
				// easier to just sit back and turtle alittle
				/*else if(mArmyBehaviour <= ArmyBehaviour::Default)
				{
					if(mySupply > (enemyGuessSupply * 4))
						shouldAttackBase = true;
				}
				else if(mArmyBehaviour == ArmyBehaviour::Defensive)
				{
					if(mySupply > (enemyGuessSupply * 5))
						shouldAttackBase = true;
				}*/
			}
		}

		if(shouldAttackBase)
		{
			Base bestBaseToAttack;
			if(mLastGoal.getGoalType() == GoalType::Base && mLastGoal.getActionType() == ActionType::Attack && mLastGoal.getBase()->isEnemyBase())
				bestBaseToAttack = mLastGoal.getBase();

			for each(Base base in BaseTracker::Instance().getEnemyBases())
			{
				if(!bestBaseToAttack || base->getBuildings().size() < bestBaseToAttack->getBuildings().size())
					bestBaseToAttack = base;
			}

			if(bestBaseToAttack)
				squadsGoal = Goal(ActionType::Attack, bestBaseToAttack, engageGroup, avoidGroup);
		}
	}

	if(squadsGoal.getGoalType() == GoalType::None && baseToDefend && mUnits.canMajorityAttack(baseToDefend->getEnemyThreats()))
		squadsGoal = Goal(ActionType::Defend, baseToDefend->getCenterLocation(), engageGroup, avoidGroup);

	if(squadsGoal.getGoalType() == GoalType::None)
	{
		if(!BorderTracker::Instance().getBorderPositions(PositionType::TechDefenseChokepoint).empty())
			squadsGoal = Goal(ActionType::Hold, getLargestChoke(BorderTracker::Instance().getBorderPositions(PositionType::TechDefenseChokepoint)).mChoke->getCenter(), engageGroup, avoidGroup);
	}

	if(squadsGoal.getGoalType() == GoalType::None)
	{
		if(!BorderTracker::Instance().getBorderPositions(PositionType::DefenseChokepoint).empty())
			squadsGoal = Goal(ActionType::Hold, getLargestChoke(BorderTracker::Instance().getBorderPositions(PositionType::DefenseChokepoint)).mChoke->getCenter(), engageGroup, avoidGroup);
	}

	if(squadsGoal.getGoalType() == GoalType::None)
		squadsGoal = Goal(ActionType::Attack, Position(BWAPI::Broodwar->mapWidth()*24, BWAPI::Broodwar->mapHeight()*16), engageGroup, avoidGroup);

	if(!mEngageFull && !avoidGroup.empty() && squadsGoal.getActionType() == ActionType::Attack && squadsGoal.getGoalType() == GoalType::Base)
		squadsGoal = Goal(ActionType::Attack, avoidGroup.getCenter(), engageGroup, avoidGroup);

	mLastGoal = squadsGoal;
	if(squadsGoal.getGoalType() != GoalType::None)
	{
		for(std::map<Unit, Behaviour>::iterator it = mUnitBehaviours.begin(); it != mUnitBehaviours.end(); ++it)
			it->second.update(squadsGoal, mUnits);
	}

	return hasEnded() && mUnits.empty();
}

bool DefaultSquadTask::waitingForUnit(Unit unit) const
{
	return false;
}

void DefaultSquadTask::giveUnit(Unit unit)
{
	if(unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
		mObserver = unit;
	
	mUnits.insert(unit);

	mUnitBehaviours[unit] = Behaviour(unit);
}

void DefaultSquadTask::returnUnit(Unit unit)
{
	mUnits.erase(unit);

	mUnitBehaviours[unit].onDeleted();
	mUnitBehaviours.erase(unit);

	if(unit == mObserver)
		mObserver = StaticUnits::nullunit;
}

bool DefaultSquadTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return false;
}

UnitGroup DefaultSquadTask::getFinishedUnits()
{
	return mUnits;
}

void DefaultSquadTask::updateRequirements()
{
	clearRequirements();

	if(!hasEnded())
	{
		for each(Unit unit in UnitTracker::Instance().selectAllUnits())
		{
			if(!unit->isCompleted())
				continue;

			if(mUnits.count(unit) != 0)
				continue;

			const BWAPI::UnitType &type = unit->getType();
			if(type.isBuilding())
				continue;
			if(type.isAddon())
				continue;
			if(type == BWAPI::Broodwar->self()->getRace().getSupplyProvider())
				continue;
			if(type == BWAPI::UnitTypes::Zerg_Egg)
				continue;
			if(type == BWAPI::UnitTypes::Protoss_Interceptor)
				continue;
			if(type == BWAPI::UnitTypes::Zerg_Larva)
				continue;
			if(type == BWAPI::UnitTypes::Protoss_Scarab)
				continue;
			if(type == BWAPI::UnitTypes::Protoss_Observer)
				continue;

			RequirementGroup req;

			req.addUnitRequirement(unit, type.isWorker() ? 5 : 20, Requirement::maxTime);

			addRequirement(req);
		}

		if(!mObserver)
		{
			RequirementGroup req;

			req.addUnitFilterRequirement(20, Requirement::maxTime, UnitFilter(BWAPI::UnitTypes::Protoss_Observer) && UnitFilter(UnitFilterFlags::IsComplete));

			addRequirement(req);
		}
	}
}