#include "DefenseSquad.h"

#include "UnitTracker.h"
#include "BaseTracker.h"
#include "BorderTracker.h"
#include "BuildOrderManager.h"
#include "PlayerTracker.h"

DefenseSquadTask::DefenseSquadTask(ArmyBehaviour behaviour)
	: BaseSquadTask(behaviour)
	, mNeededWorkers(0)
	, mDefenseGoal()
{
}

int DefenseSquadTask::getEndTime() const
{
	return Requirement::maxTime;
}

int DefenseSquadTask::getEndTime(Unit unit) const
{
	return Requirement::maxTime;
}

int DefenseSquadTask::getPriority(Unit unit) const
{
	if(!unit->getType().isWorker())
	{
		bool isNeeded = false;
		if(mNeedsAntiAir && unit->canAttackAir())
			isNeeded = true;
		
		if(mNeedsAntiGround && unit->canAttackGround())
			isNeeded = true;

		if(isNeeded)
			return 30;
		else
			return 1;
	}

	int workersToReturn = int(mWorkerDefenders.size()) - mNeededWorkers;
	if(workersToReturn > 0)
	{
		int count = 1;
		for each(Unit worker in mWorkerDefenders)
		{
			if(worker == unit)
			{
				if(count <= workersToReturn)
					return 1;
				break;
			}
			++count;
		}
	}

	return 30;
}

Position DefenseSquadTask::getStartLocation(Unit unit) const
{
	return unit->getPosition();
}

Position DefenseSquadTask::getEndLocation(Unit unit) const
{
	return unit->getPosition();
}

bool DefenseSquadTask::preUpdate()
{
	updateRequirements();

	return hasEnded();
}

bool DefenseSquadTask::update()
{
	if(mDefenseGoal.getActionType() != ActionType::None)
	{
		Goal enemiesGoal(ActionType::Attack, mDefenseGoal.getBase()->getEnemyThreats());
		for(std::map<Unit, Behaviour>::iterator it = mUnitBehaviours.begin(); it != mUnitBehaviours.end(); ++it)
			it->second.update(enemiesGoal, mUnits);
	}

	return hasEnded() && mUnits.empty();
}

bool DefenseSquadTask::waitingForUnit(Unit unit) const
{
	return false;
}

void DefenseSquadTask::giveUnit(Unit unit)
{
	if(unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
		mObserver = unit;
	else if(unit->getType().isWorker())
		mWorkerDefenders.insert(unit);

	mUnits.insert(unit);

	mUnitBehaviours[unit] = Behaviour(unit);
}

void DefenseSquadTask::returnUnit(Unit unit)
{
	mUnits.erase(unit);

	mUnitBehaviours[unit].onDeleted();
	mUnitBehaviours.erase(unit);

	mWorkerDefenders.erase(unit);

	if(unit == mObserver)
		mObserver = StaticUnits::nullunit;
}

bool DefenseSquadTask::morph(Unit unit, BWAPI::UnitType previousType)
{
	return false;
}

UnitGroup DefenseSquadTask::getFinishedUnits()
{
	return mUnits;
}

void DefenseSquadTask::updateRequirements()
{
	clearRequirements();
	mNeededWorkers = 0;
	mNeedsAntiAir = false;
	mNeedsAntiGround = false;

	// Basic idea is to have a defense goal for each base that will take only the amount of units it needs to take out the threat
	// and also take workers if it cannot take out the threat with the number of units it has

	// This should make it do important things such as kill the threat while the rest of the army in the default squad stay at the choke
	// stopping any other units from entering
	if(!hasEnded() && mDefenseGoal.getActionType() != ActionType::None)
	{
		//TODO: currently picks units based on it getting units with a score higher than 100, if its zerg it could get zerglings in which case it wont have enough
		// TODO: maybe do some modification based on current units health
		int AirThreatScore = 0;
		int GroundThreatScore = 0;
		int GroundThreatCount = 0;
		bool needsDetection = false;
		for each(Unit enemy in mDefenseGoal.getBase()->getEnemyThreats())
		{
			if(!needsDetection && (enemy->getType().isCloakable() || enemy->getType().hasPermanentCloak() || enemy->isBurrowed() || enemy->getType() == BWAPI::UnitTypes::Zerg_Lurker))
				needsDetection = true;

			if(enemy->isLifted() || enemy->getType().isFlyer())
			{
				if(enemy->canAttackGround())
					AirThreatScore += enemy->getType().buildScore();
				else if(enemy->getType().spaceProvided() > 0)
				{
					if(enemy->getPlayer()->getRace() != BWAPI::Races::Zerg)
						AirThreatScore += 400;
					else if(BWAPI::Broodwar->getFrameCount() > 24*60*6) //TODO: has the player researched drop?
						AirThreatScore += 250;
					else
						AirThreatScore += 50;
				}
				else
					AirThreatScore += 50;
			}
			else
			{
				++GroundThreatCount;
				if(enemy->canAttackGround())
					GroundThreatScore += enemy->getType().buildScore();
				else
					GroundThreatScore += 50;
			}
		}

		// Temp till defense squad is good enough to use army units and not just workers
		if(needsDetection)
			return;

		if(GroundThreatScore > 0)
			mNeedsAntiGround = true;
		if(AirThreatScore > 0)
			mNeedsAntiAir = true;

		int AntiAirScore = 0;
		int AntiGroundScore = 0;
		for each(Unit unit in mUnits)
		{
			if(unit->getType().isWorker())
				continue;

			if(unit->canAttackGround())
			{
				--GroundThreatCount;
				AntiGroundScore += unit->getType().buildScore();
			}
			if(unit->canAttackAir())
				AntiAirScore += unit->getType().buildScore();
		}

		mNeededWorkers = std::max((GroundThreatScore - AntiGroundScore) / 50, 0);
		if(mNeededWorkers == 0 && (GroundThreatScore - AntiGroundScore) > 0)
			mNeededWorkers = 1;

		if(mNeededWorkers < GroundThreatCount)
			mNeededWorkers = GroundThreatCount;

		int canAttackGroundNeeded = mNeededWorkers / 2;

		if(mNeededWorkers > 14) mNeededWorkers = 0; //If we cannot defend with 14 workers, don't try, probably not worth possible losses
		if(mNeededWorkers > 1) mNeededWorkers += 1;
		if(mNeededWorkers > 2) mNeededWorkers += 1;
		if(mNeededWorkers > 14) mNeededWorkers = 14;

		int workersNeeded = std::max(mNeededWorkers - int(mWorkerDefenders.size()), 0);

		canAttackGroundNeeded -= (int(mWorkerDefenders.size()) / 2);

		// Get as many as we can for now
		// TODO: maybe check if its our only mining base or do something better
		for(int i = 0; i < workersNeeded; ++i)
		{
			RequirementGroup reqWorkers;
			reqWorkers.addUnitFilterRequirement(30, Requirement::maxTime, UnitFilter(UnitFilterFlags::type(UnitFilterFlags::IsWorker | UnitFilterFlags::IsComplete)), mDefenseGoal.getBase()->getCenterLocation());
			addRequirement(reqWorkers);
		}

		// Temp till defense squad is good enough to use army units and not just workers
		// in the below state can cause the bots army to get too seperated and cause the main army to get possibly destroyed engaging a group elsewhere
		return;

		RequirementGroup reqMain;
		if(canAttackGroundNeeded > 0)
			reqMain.addUnitFilterRequirement(30, Requirement::maxTime, UnitFilter(UnitFilterFlags::type(UnitFilterFlags::IsArmyUnit | UnitFilterFlags::CanAttackGround | UnitFilterFlags::IsComplete)), canAttackGroundNeeded, mDefenseGoal.getBase()->getCenterLocation());

		int canAttackAirNeeded = std::max((AirThreatScore - AntiAirScore) / 100, 0);
		if(canAttackAirNeeded > 0)
			reqMain.addUnitFilterRequirement(30, Requirement::maxTime, UnitFilter(UnitFilterFlags::type(UnitFilterFlags::IsArmyUnit | UnitFilterFlags::CanAttackAir | UnitFilterFlags::IsComplete)), canAttackAirNeeded, mDefenseGoal.getBase()->getCenterLocation());

		if(!mObserver && needsDetection)
			reqMain.addUnitFilterRequirement(30, Requirement::maxTime, UnitFilter(BWAPI::UnitTypes::Protoss_Observer) && UnitFilter(UnitFilterFlags::IsComplete), mDefenseGoal.getBase()->getCenterLocation());

		addRequirement(reqMain);

		Position goalpos = mDefenseGoal.getBase()->getCenterLocation();
		const int x = goalpos.x();
		const int y = goalpos.y();

		BWAPI::Broodwar->drawTextMap(x, y, "canAttackGroundNeeded : %d", canAttackGroundNeeded);
		BWAPI::Broodwar->drawTextMap(x, y+10, "canAttackAirNeeded : %d", canAttackAirNeeded);
		BWAPI::Broodwar->drawTextMap(x, y+20, "workersNeeded : %d", workersNeeded);
		BWAPI::Broodwar->drawTextMap(x, y+30, "workers Have : %d", int(mWorkerDefenders.size()));
		BWAPI::Broodwar->drawTextMap(x, y+40, "Units Have : %d", int(mUnits.size()));
		if(!mObserver && needsDetection)
			BWAPI::Broodwar->drawTextMap(x, y+50, "Needs Observer");
	}
}