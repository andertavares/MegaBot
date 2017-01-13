#pragma once

#include "Interface.h"

#include "TypeSafeEnum.h"
#include "Unit.h"
#include "Base.h"

struct ReservationTypeDef
{
	enum type
	{
		LandingPosition,
		BuildingPosition
	};
};
typedef SafeEnum<ReservationTypeDef> ReservationType;

struct BuildingLocationDef
{
	enum type
	{
		None,
		Base, //best base choice, usually main
		BaseParimeter, //place evenly around main base, for turrets etc
		BaseChoke, //place at the main bases choke, for bunkers etc
		ForwardChoke, //place at a forward choke position, such as an expansion if there is one
		ForwardParimeter, //place evenly around all buildings, for turrets etc
		Expansion, //best expansion location
		ExpansionGas, //best expansion location, only chooses a gas only
		Proxy, //proxy location, places it closest to possible enemy base locations that arn't on paths between the bases
		Manner //place in enemy base, if pylon block enemy units, if gas, steal gas if possible
	};
};
typedef SafeEnum<BuildingLocationDef> BuildingLocation;

class ReservedLocationClass : public std::tr1::enable_shared_from_this<ReservedLocationClass>
{
public:
	ReservedLocationClass(BWAPI::UnitType type, BuildingLocation location);
	ReservedLocationClass(Unit unit);

	TilePosition getTilePosition() { return mTilePosition; }
	Position getPosition() { return mPosition; }

	Base getBase() { return mBase; }

	BWAPI::UnitType getUnitType() { return mUnitType; }

	bool update();

	BuildingLocation getBuildLocation() { return mLocation; }
	Unit getUnitPrediction() { return mUnitPrediction; }

	bool isCalculated() { return mTilePosition != BWAPI::TilePositions::None; }

	void setStartTime(int time);

	void forceCompleted() { mForcedCompleted = true; }

private:
	bool shouldFree();
	void setTilePosition(TilePosition position);

	ReservationType mType;
	BWAPI::UnitType mUnitType;

	BuildingLocation mLocation;

	Base mBase;
	TilePosition mTilePosition;
	Position mPosition;

	Unit mUnit;

	int mNextCalculationAttempt;

	int mStartTime;

	Unit mUnitPrediction;

	bool mForcedCompleted;
};

typedef std::tr1::shared_ptr<ReservedLocationClass> ReservedLocation;