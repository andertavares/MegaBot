#include "ReservedLocation.h"

#include <algorithm>
#include <limits>

#include "BuildingPlacer.h"
#include "UnitPredictor.h"

ReservedLocationClass::ReservedLocationClass(BWAPI::UnitType type, BuildingLocation location)
	: mType(ReservationType::BuildingPosition)
	, mUnitType(type)
	, mLocation(location)
	, mTilePosition(BWAPI::TilePositions::None)
	, mPosition(BWAPI::Positions::None)
	, mUnit()
	, mNextCalculationAttempt(0)
	, mBase()
	, mUnitPrediction()
	, mForcedCompleted(false)
	, mStartTime(std::numeric_limits<int>::max())
{
}

ReservedLocationClass::ReservedLocationClass(Unit unit)
	: mType(ReservationType::LandingPosition)
	, mUnitType(unit->getType())
	, mLocation(BuildingLocation::None)
	, mTilePosition(BWAPI::TilePositions::None)
	, mPosition(BWAPI::Positions::None)
	, mUnit(unit)
	, mNextCalculationAttempt(0)
	, mBase()
	, mUnitPrediction()
	, mForcedCompleted(false)
	, mStartTime(std::numeric_limits<int>::max())
{
}

void ReservedLocationClass::setTilePosition(TilePosition position)
{
	mTilePosition = position;
	if(position != BWAPI::TilePositions::None)
		mPosition = Position(position.x*32+(mUnitType.tileWidth()*16), position.y*32+(mUnitType.tileHeight()*16));
	else
		mPosition = BWAPI::Positions::None;

	if(mUnitPrediction)
	{
		if(mPosition == BWAPI::Positions::None)
		{
			UnitPredictor::Instance().deletePrediction(mUnitPrediction);
			mUnitPrediction = StaticUnits::nullunit;
		}
		else
			mUnitPrediction->setPosition(mPosition);
	}
	else if(mPosition != BWAPI::Positions::None)
	{
		mUnitPrediction = UnitPredictor::Instance().createPrediction(mPosition, mUnitType, mStartTime);
		UnitPredictor::Instance().savePrediction(mUnitPrediction);
	}
}

bool ReservedLocationClass::update()
{
	bool isFinished = mUnitPrediction && mUnitPrediction->exists();
	if(shouldFree())
	{
		BuildingPlacer::Instance().free(shared_from_this());
		setTilePosition(BWAPI::TilePositions::None);
	}

	if(isFinished || mForcedCompleted)
		return true;

	if(mTilePosition == BWAPI::TilePositions::None && mNextCalculationAttempt <= BWAPI::Broodwar->getFrameCount())
	{
		std::pair<TilePosition, Base> locationPair;
		if(mType == ReservationType::BuildingPosition)
			locationPair = BuildingPlacer::Instance().buildingLocationToTile(mLocation, mUnitType);
		else if(mType == ReservationType::LandingPosition)
			locationPair = BuildingPlacer::Instance().getlandingLocation(mUnit);

		if(locationPair.first == BWAPI::TilePositions::None || BuildingPlacer::Instance().isLocationBuildable(locationPair.first, mUnitType, true))
		{
			setTilePosition(locationPair.first);
			mBase = locationPair.second;

			if(mTilePosition == BWAPI::TilePositions::None)
				mNextCalculationAttempt = BWAPI::Broodwar->getFrameCount() + 24;
			else
				BuildingPlacer::Instance().reserve(shared_from_this());
		}
	}

	return false;
}

bool ReservedLocationClass::shouldFree()
{
	if(mTilePosition != BWAPI::TilePositions::None)
	{
		if(mForcedCompleted || (mUnitPrediction && mUnitPrediction->exists()))
			return true;

		if(!BuildingPlacer::Instance().isLocationBuildable(mTilePosition, mUnitType, true))
			return true;

		if(mLocation == BuildingLocation::Base)
		{
			const std::vector<Base> &bases = BuildingPlacer::Instance().baseToBuildAtOrder(mUnitType);
			if(std::find(bases.begin(), bases.end(), mBase) == bases.end())
				return true;
		}
	}

	return false;
}

void ReservedLocationClass::setStartTime(int time)
{
	mStartTime = time;

	if(mUnitPrediction)
		mUnitPrediction->setBuildTime(time);
}