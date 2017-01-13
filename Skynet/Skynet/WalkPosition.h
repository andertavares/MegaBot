#pragma once

#include "Interface.h"

class WalkPosition
{
public:
	int x, y;

	WalkPosition()
		: x(0), y(0)
	{ }

	WalkPosition(int x, int y)
		: x(x), y(y)
	{ }

	WalkPosition(const Position &p)
		: x(p.x / 8), y(p.y / 8)
	{ }

	WalkPosition(const TilePosition &p)
		: x(p.x * 4), y(p.y * 4)
	{ }

	inline operator Position()
	{
		return Position(x * 8, y * 8);
	}

	inline operator TilePosition()
	{
		return TilePosition(x / 4, y / 4);
	}

	inline bool operator==(const WalkPosition &v) const
	{
		return (x == v.x && y == v.y);
	}

	inline bool operator!=(const WalkPosition &v) const
	{
		return !(x == v.x && y == v.y);
	}

	inline bool operator<(const WalkPosition& v) const
	{
		if(x == v.x)
			return y < v.y;
		
		return x < v.x;
	}

	inline bool operator>(const WalkPosition& v) const
	{
		if(x == v.x)
			return y > v.y;

		return x > v.x;
	}

	inline Position getCenterPosition() const
	{
		return Position(x * 8 + 4, y * 8 + 4);
	}
};