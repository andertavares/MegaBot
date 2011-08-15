#include "Unit.h"

#include <cassert>

#include "MapHelper.h"
#include "UnitTracker.h"
#include "DrawingHelper.h"
#include "Requirement.h"
#include "LatencyTracker.h"
#include "Logger.h"
#include "UnitHelper.h"

const Unit StaticUnits::nullunit;

UnitClass::UnitClass(BWAPI::Unit* unit)
	: mUnit(unit)
	, mStoredPosition(BWAPI::Positions::Unknown)
	, mStoredTargetPosition(BWAPI::Positions::None)
	, mStoredAccessType(AccessType::Full)
	, mStoredPlayer(BWAPI::Broodwar->neutral())
	, mStoredBoolOne(false)
	, mStoredCompleted(false)
	, mStoredMorphing(false)
	, mStoredCompletedTime(0)
	, mStoredTime(0)
	, mStoredInt(0)
	, mStoredExistsTime(Requirement::maxTime)
	, mStoredHealth(0)
	, mStoredShield(0)
	, mLastOrderExecuteTime(0)
{
	if(exists())
	{
		mStoredCompleted = unit->isCompleted();
		mStoredMorphing = unit->isMorphing();
	}
	update();
	if(exists())
	{
		if(!isCompleted())
			mStoredCompletedTime = BWAPI::Broodwar->getFrameCount() + unit->getType().buildTime();
		else if(isMorphing())
			mStoredCompletedTime = BWAPI::Broodwar->getFrameCount() + unit->getBuildType().buildTime();
	}
}

UnitClass::UnitClass(Position pos, BWAPI::UnitType type, int startTime)
	: mUnit(NULL)
	, mStoredPosition(pos)
	, mStoredTargetPosition(pos)
	, mStoredType(type)
	, mStoredAccessType(AccessType::Prediction)
	, mStoredPlayer(BWAPI::Broodwar->self())
	, mStoredBoolOne(false)
	, mStoredCompleted(false)
	, mStoredMorphing(false)
	, mStoredCompletedTime(startTime + type.buildTime())
	, mStoredTime(BWAPI::Broodwar->getFrameCount())
	, mStoredInt(0)
	, mStoredExistsTime(startTime)
	, mStoredHealth(0)
	, mStoredShield(0)
	, mLastOrderExecuteTime(0)
{
}

void UnitClass::onDestroy()
{
	mStoredAccessType = AccessType::Dead;
}

TilePosition UnitClass::getTilePosition()
{
	if(exists())
	{
		if(mUnit->isLoaded())
			return mUnit->getTransport()->getTilePosition();
		else
			return mUnit->getTilePosition();
	}

	const BWAPI::UnitType &type = getType();
	const Position &pos = getPosition();

	return TilePosition(Position(pos.x() - (type.tileWidth() * 16), pos.y() - (type.tileHeight() * 16)));
}

Position UnitClass::getPosition()
{
	if(exists())
	{
		if(mUnit->isLoaded())
			return mUnit->getTransport()->getPosition();
		else
			return mUnit->getPosition();
	}

	const BWAPI::UnitType &type = getType();
	
	if(type.topSpeed() != 0.0)
	{
		int travelTime = int(mStoredPosition.getApproxDistance(mStoredTargetPosition) / type.topSpeed());
		int timeDifference = BWAPI::Broodwar->getFrameCount() - mStoredTime;

		if(timeDifference > travelTime)
			return mStoredTargetPosition;

		if(travelTime != 0)
		{
			float traveled = float(timeDifference) / float(travelTime);

			Vector currentPosition = mStoredTargetPosition - mStoredPosition;

			currentPosition *= traveled;
			currentPosition += Vector(mStoredPosition);

			return currentPosition;
		}
	}

	return mStoredPosition;
}

Position UnitClass::getPosition(int inFramesTime)
{
	const Position &currentPosition = getPosition();
	const BWAPI::UnitType &type = getType();

	if(type.topSpeed() != 0.0)
	{
		const Position &targetPosition = getTargetPosition();

		int travelTime = int(currentPosition.getApproxDistance(targetPosition) / type.topSpeed());

		if(inFramesTime > travelTime)
			return targetPosition;

		if(travelTime != 0)
		{
			float traveled = float(inFramesTime) / float(travelTime);

			Vector direction = targetPosition - currentPosition;

			direction *= traveled;
			direction += Vector(mStoredPosition);

			return direction;
		}
	}

	return currentPosition;
}

Position UnitClass::getTargetPosition()
{
	if(exists())
	{
		const BWAPI::UnitType &type = getType();
		if(type.canMove() || type.isFlyingBuilding())
		{
			if(mUnit->isLoaded())
				return mUnit->getTransport()->getTargetPosition();
			else
				return mUnit->getTargetPosition();
		}
		else
			return mUnit->getPosition();
	}

	return mStoredTargetPosition;
}

Player UnitClass::getPlayer()
{
	if(exists())
		return mUnit->getPlayer();

	return mStoredPlayer;
}

BWAPI::UnitType UnitClass::getType()
{
	if(exists())
		return mUnit->getType();

	return mStoredType;
}

BWAPI::Order UnitClass::getOrder()
{
	if(exists())
		return mUnit->getOrder();

	return BWAPI::Orders::None;
}

BWAPI::Order UnitClass::getSecondaryOrder()
{
	if(exists())
		return mUnit->getSecondaryOrder();

	return BWAPI::Orders::None;
}

bool UnitClass::isLifted()
{
	if(exists())
		return mUnit->isLifted();

	return getType().isFlyingBuilding() && mStoredBoolOne;
}

bool UnitClass::isSieged()
{
	if(exists())
		return mUnit->isSieged();

	return (getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) && mStoredBoolOne;
}

bool UnitClass::isCarryingGas()
{
	if(exists())
		return mUnit->isCarryingGas();

	return false;
}

bool UnitClass::isCarryingMinerals()
{
	if(exists())
		return mUnit->isCarryingMinerals();

	return false;
}

int UnitClass::getLastSeenTime()
{
	if(exists())
		return BWAPI::Broodwar->getFrameCount();

	return mStoredTime;
}

AccessType UnitClass::accessibility()
{
	if(mUnit)
	{
		if(mUnit->exists())
		{
			if(mUnit->getPlayer() == BWAPI::Broodwar->self() || (!mUnit->isCloaked() && !mUnit->isBurrowed()) || mUnit->isDetected())
				return AccessType::Full;
			else
				return AccessType::Partial;
		}

		// If it doesn't exist but its ours it must be dead, is also possible to check if the player has left the game
		// but in some game modes they don't disappear when player->leftGame() returns true, noticed in protoss champaign
		if(getPlayer() == BWAPI::Broodwar->self())
			return AccessType::Dead;

		const BWAPI::UnitType &type = getType();
		if(MapHelper::isAnyVisible(getTilePosition(), type))
		{
			if(type.canMove() || type.isFlyingBuilding() || type == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
				mStoredAccessType = AccessType::Lost;
			else
				mStoredAccessType = AccessType::Dead;
		}
		else if(mStoredAccessType != AccessType::Lost)
			mStoredAccessType = AccessType::Guess;
	}

	return mStoredAccessType;
}

void UnitClass::drawUnitPosition()
{
	const Position &pos = getPosition();
	const BWAPI::UnitType &type = getType();
	Player player = getPlayer();

	const int barHeight = 4;

	if((!isCompleted() || isMorphing()) && accessibility() != AccessType::Prediction)
	{
		double progress = getCompletedTime() - BWAPI::Broodwar->getFrameCount();

		if(isMorphing())
			progress /= getBuildType().buildTime();
		else
			progress /= type.buildTime();

		progress = 1.0 - progress;

		BWAPI::Color barColour = isMorphing() ? BWAPI::Colors::Red : BWAPI::Colors::Purple;

		Position bottomLeft(pos.x() - type.dimensionLeft(), pos.y() + type.dimensionDown() + barHeight - 1);

		DrawingHelper::Instance().drawProgressBar(bottomLeft, type.dimensionLeft()+type.dimensionRight(), barHeight, progress, barColour, player->getColor());
	}

	if(type.maxShields() > 0)
	{
		double progress = getShield();
		progress /= type.maxShields();

		Position bottomLeft(pos.x() - type.dimensionLeft(), pos.y() - type.dimensionUp() - barHeight + 2);

		DrawingHelper::Instance().drawProgressBar(bottomLeft, type.dimensionLeft()+type.dimensionRight(), barHeight, progress, BWAPI::Colors::Blue, player->getColor());
	}

	if(type.maxHitPoints() > 0)
	{
		double progress = getHealth();
		progress /= type.maxHitPoints();

		Position bottomLeft(pos.x() - type.dimensionLeft(), pos.y() - type.dimensionUp() + 1);

		DrawingHelper::Instance().drawProgressBar(bottomLeft, type.dimensionLeft()+type.dimensionRight(), barHeight, progress, BWAPI::Colors::Green, player->getColor());
	}

	BWAPI::Broodwar->drawBox(BWAPI::CoordinateType::Map, pos.x() - type.dimensionLeft(), pos.y() - type.dimensionUp(), pos.x() + type.dimensionRight(), pos.y() + type.dimensionDown(), player->getColor());

	BWAPI::Broodwar->drawTextMap(pos.x() + type.dimensionRight(), pos.y(), "%s", player->getName().c_str());

	AccessType access = accessibility();
	BWAPI::Broodwar->drawTextMap(pos.x() + type.dimensionRight(), pos.y()+10, "%s", AccessType::getName(access.underlying()).c_str());

	BWAPI::Broodwar->drawTextMap(pos.x() + type.dimensionRight(), pos.y()+20, "%d", (getExistTime() - BWAPI::Broodwar->getFrameCount()));
	BWAPI::Broodwar->drawTextMap(pos.x() + type.dimensionRight(), pos.y()+30, "%d", (getCompletedTime() - BWAPI::Broodwar->getFrameCount()));

	if(isMorphing())
		BWAPI::Broodwar->drawTextMap(pos.x() + type.dimensionRight(), pos.y()+40, "Morphing");

	if(isCompleted())
		BWAPI::Broodwar->drawTextMap(pos.x() + type.dimensionRight(), pos.y()+50, "Completed");

	Position target = getTargetPosition();
	BWAPI::Broodwar->drawLine(BWAPI::CoordinateType::Map, pos.x(), pos.y(), target.x(), target.y(), player->getColor());
}

void UnitClass::drawUnitTilePosition()
{
	TilePosition tile = getTilePosition();
	BWAPI::UnitType type = getType();
	Player player = getPlayer();

	BWAPI::Broodwar->drawBox(BWAPI::CoordinateType::Map, tile.x()*32, tile.y()*32, (tile.x()+type.tileWidth())*32, (tile.y()+type.tileHeight())*32, player->getColor());
}

bool UnitClass::exists()
{
	return mUnit && mUnit->exists();
}

int UnitClass::getExistTime()
{
	if(mUnit)
		return BWAPI::Broodwar->getFrameCount();

	return std::max(mStoredExistsTime, BWAPI::Broodwar->getFrameCount() + 1);
}

bool UnitClass::isMorphing()
{
	if(exists())
	{
		if(!mUnit->isMorphing())
			return false;

		if(mUnit->getPlayer() == BWAPI::Broodwar->self())
			return mUnit->getRemainingBuildTime() > 0;

		return true;
	}

	if(!mStoredMorphing)
		return false;

	return mStoredCompletedTime > BWAPI::Broodwar->getFrameCount();
}

bool UnitClass::isCompleted()
{
	if(exists())
	{
		if(mUnit->isCompleted())
			return true;

		if(mUnit->getPlayer() == BWAPI::Broodwar->self())
			return mUnit->getRemainingBuildTime() == 0;

		return false;
	}

	if(accessibility() == AccessType::Prediction)
		return false;

	if(mStoredCompleted)
		return true;

	return mStoredCompletedTime <= BWAPI::Broodwar->getFrameCount();
}

int UnitClass::getCompletedTime()
{
	if(isCompleted())
		return BWAPI::Broodwar->getFrameCount();

	if(exists())
	{
		if(mUnit->getPlayer() == BWAPI::Broodwar->self())
			return BWAPI::Broodwar->getFrameCount() + mUnit->getRemainingBuildTime();
	}
	else
	{
		const int existsTime = getExistTime();
		const int completeTime = existsTime + getType().buildTime();

		if(completeTime < existsTime)
			return std::numeric_limits<int>::max();
		else
			return completeTime;
	}

	return std::max(mStoredCompletedTime, BWAPI::Broodwar->getFrameCount() + 1);
}

bool UnitClass::isTraining()
{
	if(exists())
		return mUnit->isTraining();

	return false;
}

bool UnitClass::isResearching()
{
	if(exists())
		return mUnit->isResearching();

	return false;
}

bool UnitClass::isConstructing()
{
	if(exists())
		return mUnit->isConstructing();

	return false;
}

bool UnitClass::isBeingConstructed()
{
	if(exists())
		return mUnit->isBeingConstructed();

	return false;
}

int UnitClass::getRemainingTrainTime()
{
	if(exists() && mUnit->getPlayer() == BWAPI::Broodwar->self())
		return mUnit->getRemainingTrainTime();

	return 0;
}

int UnitClass::getRemainingUpgradeTime()
{
	if(exists() && mUnit->getPlayer() == BWAPI::Broodwar->self())
		return mUnit->getRemainingUpgradeTime();

	return 0;
}

int UnitClass::getRemainingResearchTime()
{
	if(exists() && mUnit->getPlayer() == BWAPI::Broodwar->self())
		return mUnit->getRemainingResearchTime();

	return 0;
}

void UnitClass::train(BWAPI::UnitType type)
{
	if(exists())
	{
		if(mUnit->getSecondaryOrder() == BWAPI::Orders::Train)
		{
			const std::list<BWAPI::UnitType> &queue = mUnit->getTrainingQueue();
			if(queue.size() > 1 || (queue.size() == 1 && *queue.begin() != type))
			{
				cancel();
				return;
			}

			return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Train && mUnit->getLastCommand().getUnitType() == type)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->train(type))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

Unit UnitClass::getBuildUnit()
{
	if(exists())
		return UnitTracker::Instance().getUnit(mUnit->getBuildUnit());

	return StaticUnits::nullunit;
}

Unit UnitClass::getTarget()
{
	if(exists())
	{
		if(mUnit->getOrderTarget())
			return UnitTracker::Instance().getUnit(mUnit->getOrderTarget());
		else
			return UnitTracker::Instance().getUnit(mUnit->getTarget());
	}

	return StaticUnits::nullunit;
}

void UnitClass::build(TilePosition target, BWAPI::UnitType type)
{
	if(exists())
	{
		const Position targetPosition(target.x()*32+type.tileWidth()*16, target.y()*32+type.tileHeight()*16);
		if(getDistance(type, targetPosition) > 48 || !MapHelper::isAllVisible(target, type))
		{
			move(targetPosition, 0);
			return;
		}

		if(mUnit->getOrder() == BWAPI::Orders::PlaceBuilding)
		{
			if(mUnit->getBuildType() == type && mUnit->getOrderTargetPosition() == targetPosition)
				return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Build && mUnit->getLastCommand().getUnitType() == type && mUnit->getLastCommand().getTargetTilePosition() == target)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->build(target, type))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
		else
			move(targetPosition, 0);
	}
}

void UnitClass::build(Unit unit)
{
	if(exists() && unit && unit->exists())
	{
		mUnit->rightClick(unit->mUnit);
		unit->mUnit->rightClick(mUnit);
	}
}

bool UnitClass::hasAddon()
{
	if(exists())
		return mUnit->getAddon() != NULL;

	return false;
}

void UnitClass::morph(BWAPI::UnitType type)
{
	if(exists())
		mUnit->morph(type);
}

BWAPI::UnitType UnitClass::getBuildType()
{
	if(mUnit)
	{
		if(mUnit->exists())
			return mUnit->getBuildType();
		else
			return BWAPI::UnitTypes::Unknown;
	}

	return BWAPI::UnitTypes::None;
}

int UnitClass::getID()
{
	if(mUnit)
		return mUnit->getID();

	return 0;
}

int UnitClass::getResources()
{
	if(exists())
		return mUnit->getResources();

	return mStoredInt;
}

bool UnitClass::isUpgrading()
{
	if(exists())
		return mUnit->isUpgrading();

	return false;
}

BWAPI::UpgradeType UnitClass::getUpgrade()
{
	if(exists())
		return mUnit->getUpgrade();
	
	return BWAPI::UpgradeTypes::None;
}

void UnitClass::upgrade(BWAPI::UpgradeType type)
{
	if(exists())
		mUnit->upgrade(type);
}

BWAPI::TechType UnitClass::getTech()
{
	if(exists())
		return mUnit->getTech();

	return BWAPI::TechTypes::None;
}

bool UnitClass::cancel(int slot)
{
	if(exists())
	{
		if(mUnit->isResearching())
			mUnit->cancelResearch();
		else if(mUnit->isUpgrading())
			mUnit->cancelUpgrade();
		else if(mUnit->isTraining())
			mUnit->cancelTrain(slot);
		else if(mUnit->isMorphing())
			mUnit->cancelMorph();
		else if(mUnit->isBeingConstructed() || !mUnit->isCompleted())
			mUnit->cancelConstruction();
		else if(mUnit->isConstructing())
			mUnit->haltConstruction();
		else
			return false;

		return true;
	}

	return false;
}

void UnitClass::research(BWAPI::TechType mType)
{
	if(exists())
		mUnit->research(mType);
}

void UnitClass::move(Position target, int accuracy)
{
	if(exists())
	{
		if(mUnit->getOrder() == BWAPI::Orders::Move)
		{
			if(mUnit->getOrderTargetPosition().getApproxDistance(target) <= accuracy)
				return;
		}
		
		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Move && mUnit->getLastCommand().getTargetPosition().getApproxDistance(target) <= accuracy)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
			else if((mUnit->getOrder() == BWAPI::Orders::Guard || mUnit->getOrder() == BWAPI::Orders::PlayerGuard) && getPosition().getApproxDistance(target) <= accuracy*4)
				return;
		}

		if(mUnit->move(target))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::attack(Unit unit)
{
	if(exists() && unit)
	{
		if(!unit->exists())
		{
			move(unit->getPosition());
			return;
		}

		if(mUnit->getOrder() == BWAPI::Orders::AttackUnit)
		{
			if(mUnit->getOrderTarget() == unit->mUnit)
				return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit && mUnit->getLastCommand().getTarget() == unit->mUnit)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->attack(unit->mUnit))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::attack(Position target, int accuracy)
{
	if(exists())
	{
		if(mUnit->getOrder() == BWAPI::Orders::AttackMove)
		{
			if(mUnit->getOrderTargetPosition().getApproxDistance(target) <= accuracy)
				return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Move && mUnit->getLastCommand().getTargetPosition().getApproxDistance(target) <= accuracy)
		{
			if(mUnit->getOrder() == BWAPI::Orders::Guard || mUnit->getOrder() == BWAPI::Orders::PlayerGuard)
				return;
			else if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->attack(target))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::gather(Unit unit)
{
	if(exists() && unit)
	{
		if(!unit->exists())
		{
			move(unit->getPosition());
			return;
		}
	
		if(unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field)
		{
			if(mUnit->getOrder() == BWAPI::Orders::MoveToMinerals || mUnit->getOrder() == BWAPI::Orders::WaitForMinerals || mUnit->getOrder() == BWAPI::Orders::MiningMinerals)
			{
				if(mUnit->getOrderTarget() == unit->mUnit)
					return;
			}
		}
		else if(unit->getType().isRefinery())
		{
			if(mUnit->getOrder() == BWAPI::Orders::Harvest1)
				return;
			else if(mUnit->getOrder() == BWAPI::Orders::MoveToGas || mUnit->getOrder() == BWAPI::Orders::WaitForGas || mUnit->getOrder() == BWAPI::Orders::HarvestGas)
			{
				if(mUnit->getOrderTarget() == unit->mUnit)
					return;
			}
		}
		else
			return;

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Gather && mUnit->getLastCommand().getTarget() == unit->mUnit)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->gather(unit->mUnit))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::returnCargo()
{
	if(exists())
	{
		if(mUnit->getOrder() == BWAPI::Orders::ReturnGas || mUnit->getOrder() == BWAPI::Orders::ReturnMinerals)
			return;

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Return_Cargo)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->returnCargo())
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::returnCargo(Unit unit)
{
	if(!unit)
		return;

	if(!unit->exists() || !unit->isCompleted())
	{
		move(unit->getPosition());
		return;
	}

	if(exists())
	{
		if(mUnit->getOrder() == BWAPI::Orders::ResetCollision)
			return;
		
		if(mUnit->getOrder() == BWAPI::Orders::ReturnGas || mUnit->getOrder() == BWAPI::Orders::ReturnMinerals)
		{
			if(mUnit->getOrderTarget() == unit->mUnit)
				return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Right_Click_Unit && mUnit->getLastCommand().getTarget() == unit->mUnit)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->rightClick(unit->mUnit))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::stop()
{
	if(exists())
	{
		if(mUnit->getOrder() == BWAPI::Orders::Stop)
			return;

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Stop)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->stop())
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::promote(BWAPI::Unit* unit)
{
	mUnit = unit;
	mStoredAccessType = AccessType::Full;
	update();
}

void UnitClass::setBuildTime(int time)
{
	mStoredExistsTime = time;

	mStoredCompletedTime = time + getType().buildTime();
	if(mStoredCompletedTime < time)
		mStoredCompletedTime = std::numeric_limits<int>::max();
}

double UnitClass::totalHitPointFraction()
{
	double currentHealth = totalHitPoints();

	return currentHealth / double(getType().maxHitPoints() + getType().maxShields());
}

int UnitClass::totalHitPoints()
{
	return getHealth() + getShield() + getDefenseMatrixPoints(); 
}

int UnitClass::getHealth()
{
	if(exists())
		return mUnit->getHitPoints();

	return mStoredHealth;
}

int UnitClass::getShield()
{
	if(exists())
		return mUnit->getShields();

	return mStoredShield;
}

int UnitClass::getDefenseMatrixPoints()
{
	if(exists())
		return mUnit->getDefenseMatrixPoints();

	return 0;
}

void UnitClass::setPosition(Position position)
{
	mStoredPosition = position;
	mStoredTargetPosition = position;
}

bool UnitClass::isSelected()
{
	if(exists())
		return mUnit->isSelected();

	return false;
}

bool UnitClass::hasPath(Position position)
{
	if(exists())
		return mUnit->hasPath(position);

	return BWAPI::Broodwar->hasPath(getPosition(), position);
}

bool UnitClass::isStasised()
{
	if(exists())
		if(mUnit->isStasised())
			return true;

	return LatencyTracker::Instance().isStasisInRange(shared_from_this());
}

bool UnitClass::isLockedDown()
{
	if(exists())
		return mUnit->isLockedDown();

	return false;
}

bool UnitClass::isCloaked()
{
	if(exists())
		return mUnit->isCloaked();

	return false;
}

bool UnitClass::isBurrowed()
{
	if(exists())
		return mUnit->isBurrowed();

	return false;
}

bool UnitClass::isDetected()
{
	if(exists())
		return mUnit->isDetected();

	return false;
}

bool UnitClass::isStuck()
{
	if(exists())
		return mUnit->isStuck();

	return false;
}

bool UnitClass::isUnderStorm()
{
	if(exists())
		if(mUnit->isUnderStorm())
			return true;

	return LatencyTracker::Instance().isStormInRange(shared_from_this());
}

bool UnitClass::isUnderDarkSwarm()
{
	if(exists())
		if(mUnit->isUnderDarkSwarm())
			return true;

	return false;
}

bool UnitClass::isLoaded()
{
	if(exists())
		return mUnit->isLoaded();

	return false;
}

bool UnitClass::canAttackNow(Unit unit)
{
	if(!unit || !exists() || !unit->exists())
		return false;

	return canAttack(unit);
}

bool UnitClass::canAttack(Unit unit)
{
	if(!unit)
		return false;

	if(mUnit->isLockedDown() || mUnit->isStasised() || unit->isStasised())
		return false;

	if((unit->isCloaked() || unit->isBurrowed()) && !unit->isDetected())
		return false;

	if(unit->getType().isFlyer() || unit->isLifted())
		return canAttackAir();
	else
		return canAttackGround();
}

bool UnitClass::isInRange(Unit unit)
{
	int weaponMaxRange = getWeaponMaxRange(unit);

	if(weaponMaxRange <= 0)
		return false;

	int distance = getDistance(unit);
	int weaponMinRange = getWeaponMinRange(unit);
	
	if(weaponMinRange != 0 && distance <= weaponMinRange)
		return false;

	return weaponMaxRange >= distance;
}


int UnitClass::getGroundWeaponMaxRange()
{
	if(getType() == BWAPI::UnitTypes::Protoss_Reaver)
		return 256;
	else
		return getPlayer()->weaponMaxRange(getType().groundWeapon());
}

int UnitClass::getAirWeaponMaxRange()
{
	return getPlayer()->weaponMaxRange(getType().airWeapon());
}

int UnitClass::getWeaponMaxRange(Unit unit)
{
	if(!unit)
		return 0;

	if(unit->getType().isFlyer() || unit->isLifted())
		return getAirWeaponMaxRange();
	else
		return getGroundWeaponMaxRange();
}

int UnitClass::getWeaponMinRange(Unit unit)
{
	if(!unit)
		return 0;

	if(unit->getType().isFlyer() || unit->isLifted())
		return getType().airWeapon().minRange();
	else
		return getType().groundWeapon().minRange();
}

int UnitClass::getWeaponCooldown(Unit unit)
{
	if(!unit)
		return 0;

	if(getType() == BWAPI::UnitTypes::Protoss_Reaver)
		return 60;

	if(unit->getType().isFlyer() || unit->isLifted())
		return getType().airWeapon().damageCooldown();
	else
		return getType().groundWeapon().damageCooldown();
}

int UnitClass::getRemainingCooldown(Unit unit)
{
	if(!unit)
		return 0;

	if(unit->getType().isFlyer() || unit->isLifted())
		return getRemainingAirCooldown();
	else
		return getRemainingGroundCooldown();
}

int UnitClass::getRemainingGroundCooldown()
{
	if(exists())
		return mUnit->getGroundWeaponCooldown();

	return 0;
}

int UnitClass::getRemainingAirCooldown()
{
	if(exists())
		return mUnit->getAirWeaponCooldown();

	return 0;
}

int UnitClass::getSpellCooldown()
{
	if(exists())
		return mUnit->getSpellCooldown();

	return 0;
}

int UnitClass::getTimeToKill(Unit unit)
{
	int health = unit->totalHitPoints();

	BWAPI::WeaponType weapon = getWeapon(unit);

	int weaponDamage = weapon.damageAmount() + (weapon.damageFactor() * getPlayer()->getUpgradeLevel(weapon.upgradeType()));
	if(weaponDamage == 0)
		return 5000;

	int thisNumberOfShots = health / weaponDamage;

	if(weapon.damageType() == BWAPI::DamageTypes::Concussive)
	{
		if(unit->getType().size() == BWAPI::UnitSizeTypes::Large)
			thisNumberOfShots *= 4;
		else if(unit->getType().size() == BWAPI::UnitSizeTypes::Medium)
			thisNumberOfShots += thisNumberOfShots;
	}
	else if(weapon.damageType() == BWAPI::DamageTypes::Explosive)
	{
		if(unit->getType().size() == BWAPI::UnitSizeTypes::Small)
			thisNumberOfShots += thisNumberOfShots;
		else if(unit->getType().size() == BWAPI::UnitSizeTypes::Medium)
			thisNumberOfShots += thisNumberOfShots / 2;
	}

	if((BWAPI::Broodwar->getGroundHeight(getTilePosition()) - BWAPI::Broodwar->getGroundHeight(unit->getTilePosition())) < 0)
		thisNumberOfShots += thisNumberOfShots;

	return thisNumberOfShots * weapon.damageCooldown();
}

BWAPI::WeaponType UnitClass::getWeapon(Unit unit)
{
	if(!unit)
		return BWAPI::WeaponTypes::None;

	if(unit->getType().isFlyer() || unit->isLifted())
		return getType().airWeapon();
	else
	{
		if(getType() == BWAPI::UnitTypes::Protoss_Reaver)
			return BWAPI::WeaponTypes::Scarab;
		else
			return getType().groundWeapon();
	}
}

bool UnitClass::canAttackGround()
{
	const BWAPI::UnitType &type = getType();
	if(type == BWAPI::UnitTypes::Protoss_Reaver)
		return getScarabCount() != 0;

	if(type == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		return true;

	return type.groundWeapon() != BWAPI::WeaponTypes::None;
}

bool UnitClass::canAttackAir()
{
	return getType().airWeapon() != BWAPI::WeaponTypes::None;
}

bool UnitClass::isAttackFrame()
{
	if(exists())
		return mUnit->isAttackFrame();

	return false;
}

int UnitClass::getScarabCount()
{
	if(exists())
		return mUnit->getScarabCount();

	return 0;
}

bool UnitClass::isRepairing()
{
	if(exists())
		return mUnit->isRepairing();

	return false;
}

bool UnitClass::isHealing()
{
	if(exists())
		return mUnit->getOrder() == BWAPI::Orders::MedicHeal1 || mUnit->getOrder() == BWAPI::Orders::MedicHeal2;

	return false;
}

BWAPI::UnitCommand UnitClass::getLastCommand()
{
	if(exists())
		return mUnit->getLastCommand();

	return BWAPI::UnitCommand();
}

bool UnitClass::isBeingHealed()
{
	if(exists())
		return mUnit->isBeingHealed();

	return false;
}

bool UnitClass::isBeingRepaired()
{
	//TODO: implement
	return false;
}

int UnitClass::getDistance(BWAPI::UnitType targType, Position position)
{
	return UnitHelper::getDistance(getPosition(), getType(), position, targType);
}

int UnitClass::getDistance(Unit unit)
{
	if(!unit)
		return 0;

	return getDistance(unit->getType(), unit->getPosition());
}

int UnitClass::getDistance(Position position)
{
	return UnitHelper::getDistance(getPosition(), getType(), position);
}

int UnitClass::getDistance(Position position, int inFramesTime)
{
	return UnitHelper::getDistance(getPosition(inFramesTime), getType(), position);
}

std::list<BWAPI::UnitType> UnitClass::getTrainingQueue()
{
	if(exists())
		return mUnit->getTrainingQueue();

	return std::list<BWAPI::UnitType>();
}

int UnitClass::getEnergy()
{
	if(exists())
		return mUnit->getEnergy();

	return 0;
}

BWAPI::Order getTechCastOrder(BWAPI::TechType tech)
{
	if(tech == BWAPI::TechTypes::Psionic_Storm)
		return BWAPI::Orders::CastPsionicStorm;
	else if(tech == BWAPI::TechTypes::Archon_Warp)
		return BWAPI::Orders::ArchonWarp;
	else if(tech == BWAPI::TechTypes::Stasis_Field)
		return BWAPI::Orders::CastStasisField;
	else if(tech == BWAPI::TechTypes::Recall)
		return BWAPI::Orders::CastRecall;

	return BWAPI::Orders::None;
}

void UnitClass::useTech(BWAPI::TechType tech, BWAPI::Position target)
{
	if(exists())
	{
		if(mUnit->getOrder() == getTechCastOrder(tech))
		{
			if(mUnit->getOrderTargetPosition() == target)
				return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Use_Tech_Position && mUnit->getLastCommand().getTechType() == tech && mUnit->getLastCommand().getTargetPosition() == target)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->useTech(tech, target))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

void UnitClass::useTech(BWAPI::TechType tech, Unit target)
{
	if(exists() && target)
	{
		if(!target->exists())
		{
			move(target->getPosition());
			return;
		}

		if(mUnit->getOrder() == getTechCastOrder(tech))//TODO: in latest revision, the tech knows the relating order
		{
			if(mUnit->getOrderTarget() == target->mUnit)
				return;
		}

		if(mUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Use_Tech_Unit && mUnit->getLastCommand().getTechType() == tech && mUnit->getLastCommand().getTarget() == target->mUnit)
		{
			if(mLastOrderExecuteTime >= BWAPI::Broodwar->getFrameCount())
				return;
		}

		if(mUnit->useTech(tech, target->mUnit))
			mLastOrderExecuteTime = BWAPI::Broodwar->getFrameCount() + BWAPI::Broodwar->getRemainingLatencyFrames();
	}
}

bool UnitClass::hasOrder(BWAPI::Order order)
{
	if(getOrder() == order)
		return true;
	else if(getSecondaryOrder() == order)
		return true;
	else
		return false;
}

void UnitClass::update()
{
	if(exists())
	{
		mStoredTime = BWAPI::Broodwar->getFrameCount();

		mStoredPosition = mUnit->getPosition();
		mStoredType = mUnit->getType();
		mStoredPlayer = mUnit->getPlayer();
		mStoredInt = mUnit->getResources();

		if(mStoredCompleted && !mUnit->isCompleted())
			mStoredCompletedTime = BWAPI::Broodwar->getFrameCount() + mUnit->getType().buildTime();
		mStoredCompleted = mUnit->isCompleted();

		if(!mStoredMorphing && mUnit->isMorphing())
			mStoredCompletedTime = BWAPI::Broodwar->getFrameCount() + mUnit->getBuildType().buildTime();
		mStoredMorphing = mUnit->isMorphing();

		if(mUnit->getPlayer() == BWAPI::Broodwar->self())
			mStoredCompletedTime = BWAPI::Broodwar->getFrameCount() + mUnit->getRemainingBuildTime();
		else if(mUnit->isCompleted() && !mUnit->isMorphing())
			mStoredCompletedTime = BWAPI::Broodwar->getFrameCount();

		mStoredTargetPosition = mUnit->getTargetPosition();

		mStoredHealth = mUnit->getHitPoints();
		mStoredShield = mUnit->getShields();

		if(mUnit->getType().isFlyingBuilding())
			mStoredBoolOne = mUnit->isLifted();
		else if(mUnit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || mUnit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode)
			mStoredBoolOne = mUnit->isSieged();

		mStoredAccessType = accessibility();
	}
}