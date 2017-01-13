#pragma once

#include "Interface.h"

#include "Base.h"
#include "Unit.h"
#include "TypeSafeEnum.h"

struct ScoutTypeDef
{
	enum type
	{
		None,
		FailedWithGroundLow,
		FailedWithGroundHigh,
		FailedWithAir,
		CouldNotReachWithGround,
		InitialBaseLocationScout,
		BaseScout,
		TechSearch,
		BaseSearch,
		PerimeterSearch,
		ThoroughSearch
	};

	static std::string getName(type t)
	{
		switch(t)
		{
		case None:
			return std::string("None");
		case FailedWithGroundLow:
			return std::string("Failed With Ground Low");
		case FailedWithGroundHigh:
			return std::string("Failed With Ground High");
		case FailedWithAir:
			return std::string("Failed With Air");
		case CouldNotReachWithGround:
			return std::string("Failed To Reach Ground");
		case InitialBaseLocationScout:
			return std::string("Initial BaseLocation Scout");
		case BaseScout:
			return std::string("Base Scout");
		case TechSearch:
			return std::string("Tech Search");
		case BaseSearch:
			return std::string("Base Search");
		case ThoroughSearch:
			return std::string("Thorough Search");
		case PerimeterSearch:
			return std::string("Perimeter Search");
		}
		return std::string("Unknown");
	}
};
typedef SafeEnum<ScoutTypeDef> ScoutType;

struct ScoutUnitTypeDef
{
	enum type
	{
		Worker,
		Observer,
		Overlord,
		Scan
	};
};
typedef SafeEnum<ScoutUnitTypeDef> ScoutUnitType;

class ScoutDataClass
{
public:
	ScoutDataClass();
	ScoutDataClass(Base base, ScoutType type);

	void update();

	bool isAchieved();

	Position getNextPosition(Position pos = BWAPI::Positions::None);
	Position getLastPosition(Position pos = BWAPI::Positions::None);
	double getDistance(Unit unit);

	Base getBase() { return mBase; }
	ScoutType getType() { return mType; }

	void drawDebug(BWAPI::Color colour);

private:
	ScoutType mType;
	Base mBase;

	std::set<Position> mPositions;

	void loadPositions();
	void checkPositions();
};

typedef std::tr1::shared_ptr<ScoutDataClass> ScoutData;