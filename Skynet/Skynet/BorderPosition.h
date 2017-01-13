#pragma once

#include "Interface.h"

#include "Region.h"
#include "Chokepoint.h"
#include "Base.h"
#include "TypeSafeEnum.h"

struct PositionTypeDef
{
	enum type
	{
		None,
		ContainChokepoint,
		ForwardChokepoint,
		EnemyChokepoint,
		TechDefenseChokepoint,
		DefenseChokepoint,
		SharedChokepoint,
		Position,
		HoldBase,
		EnemyControlRegion,
		BotControlRegion,
		ForwardRegion,
		SafeTravelRegion
	};
};
typedef SafeEnum<PositionTypeDef> PositionType;

class BorderPosition
{
public:
	BorderPosition();
	BorderPosition(PositionType type, Chokepoint choke);
	BorderPosition(Position position, int size);
	BorderPosition(Base base);
	BorderPosition(PositionType type, Region region);

	bool operator==(const BorderPosition& other) const;
	bool operator<(const BorderPosition& other) const;

	PositionType mType;

	Chokepoint mChoke;

	Position mPosition;
	int mPositionSize;

	Base mBase;

	Region mRegion;
};