#include "BorderPosition.h"

BorderPosition::BorderPosition()
	: mType(PositionType::None)
	, mChoke()
	, mPosition(BWAPI::Positions::None)
	, mPositionSize(0)
	, mBase()
	, mRegion()
{
}

BorderPosition::BorderPosition(PositionType type, Chokepoint choke)
	: mType(type)
	, mChoke(choke)
	, mPosition(BWAPI::Positions::None)
	, mPositionSize(0)
	, mBase()
	, mRegion()
{
}

BorderPosition::BorderPosition(Position position, int size)
	: mType(PositionType::Position)
	, mChoke()
	, mPosition(position)
	, mPositionSize(size)
	, mBase()
	, mRegion()
{
}

BorderPosition::BorderPosition(Base base)
	: mType(PositionType::HoldBase)
	, mChoke()
	, mPosition(BWAPI::Positions::None)
	, mPositionSize(0)
	, mBase(base)
	, mRegion()
{
}

BorderPosition::BorderPosition(PositionType type, Region region)
	: mType(type)
	, mChoke()
	, mPosition(BWAPI::Positions::None)
	, mPositionSize(0)
	, mBase()
	, mRegion(region)
{
}

bool BorderPosition::operator==(const BorderPosition& other) const
{
	if(mType == other.mType)
	{
		switch(mType.underlying())
		{
		case PositionType::None:
			return true;
		case PositionType::ContainChokepoint:
		case PositionType::ForwardChokepoint:
		case PositionType::EnemyChokepoint:
		case PositionType::DefenseChokepoint:
		case PositionType::SharedChokepoint:
			return mChoke == other.mChoke;
		case PositionType::Position:
			return mPosition == other.mPosition;
		case PositionType::HoldBase:
			return mBase == other.mBase;
		case PositionType::EnemyControlRegion:
		case PositionType::BotControlRegion:
		case PositionType::ForwardRegion:
		case PositionType::SafeTravelRegion:
			return mRegion == other.mRegion;
		default:
			return true;
		}
	}
	else
		return false;
}

bool BorderPosition::operator<(const BorderPosition& other) const
{
	if(mType == other.mType)
	{
		switch(mType.underlying())
		{
		case PositionType::None:
			return false;
		case PositionType::ContainChokepoint:
		case PositionType::ForwardChokepoint:
		case PositionType::EnemyChokepoint:
		case PositionType::DefenseChokepoint:
		case PositionType::SharedChokepoint:
			return mChoke < other.mChoke;
		case PositionType::Position:
			return mPosition < other.mPosition;
		case PositionType::HoldBase:
			return mBase < other.mBase;
		case PositionType::EnemyControlRegion:
		case PositionType::BotControlRegion:
		case PositionType::ForwardRegion:
		case PositionType::SafeTravelRegion:
			return mRegion < other.mRegion;
		default:
			return false;
		}
	}
	else
		return mType < other.mType;
}