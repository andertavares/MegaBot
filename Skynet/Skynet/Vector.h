#pragma once

#include <math.h>

#include "Interface.h"

class Vector
{
public:
	float x, y;

	Vector()
		: x(0), y(0)
	{ }

	Vector(const float x, const float y)
		: x(x), y(y)
	{ }

	Vector(const int x, const int y)
		: x(float(x)), y(float(y))
	{ }

	Vector(const Vector &v)
		: x(v.x), y(v.y)
	{ }

	Vector(const Position &p)
		: x( float(p.x) ), y( float(p.y) )
	{ }

	Vector(const TilePosition &p)
		: x( float(p.x * 32) ), y( float(p.y * 32) )
	{ }

	inline operator Position()
	{
		return Position( int(x), int(y) );
	}

	inline operator TilePosition()
	{
		return TilePosition( int(x / 32), int(y / 32) );
	}

	inline Vector& operator=(const float scalar)
	{
		x = scalar;
		y = scalar;

		return *this;
	}

	inline bool operator==(const Vector &v) const
	{
		return (x == v.x && y == v.y);
	}

	inline bool operator!=(const Vector &v) const
	{
		return !(x == v.x && y == v.y);
	}

	inline Vector operator+(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y);
	}

	inline Vector operator-(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y);
	}

	inline Vector operator*(const float& scalar) const
	{
		return Vector(x * scalar, y * scalar);
	}

	inline Vector operator*(const Vector& v) const
	{
		return Vector(x * v.x, y * v.y);
	}

	inline Vector operator/(const float& scalar) const
	{
		return Vector(x / scalar, y / scalar);
	}

	inline Vector operator/(const Vector& v) const
	{
		return Vector(x / v.x, y / v.y);
	}

	inline Vector operator+() const
	{
		return *this;
	}

	inline Vector operator-() const
	{
		return Vector(-x, -y);
	}

	inline friend Vector operator*(const float scalar, const Vector& v)
	{
		return Vector(scalar * v.x, scalar * v.y);
	}

	inline friend Vector operator/(const float scalar, const Vector& v)
	{
		return Vector(scalar / v.x, scalar / v.y);
	}

	inline friend Vector operator+(const Vector& lhs, const float rhs)
	{
		return Vector(lhs.x + rhs, lhs.y + rhs);
	}

	inline friend Vector operator+(const float lhs, const Vector& rhs)
	{
		return Vector(lhs + rhs.x, lhs + rhs.y);
	}

	inline friend Vector operator-(const Vector& lhs, const float rhs)
	{
		return Vector(lhs.x - rhs, lhs.y - rhs);
	}

	inline friend Vector operator-(const float lhs, const Vector& rhs)
	{
		return Vector(lhs - rhs.x, lhs - rhs.y);
	}

	inline Vector& operator+=(const Vector& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	inline Vector& operator+=(const float scalar)
	{
		x += scalar;
		y += scalar;

		return *this;
	}

	inline Vector& operator-=(const Vector& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	inline Vector& operator-=(const float scalar)
	{
		x -= scalar;
		y -= scalar;

		return *this;
	}

	inline Vector& operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	inline Vector& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;

		return *this;
	}

	inline Vector& operator/=(const float scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	inline Vector& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;

		return *this;
	}

	inline float length() const
	{
		return sqrt(x * x + y * y);
	}

	inline float squaredLength() const
	{
		return x * x + y * y;
	}

	inline float distance(const Vector& v) const
	{
		return (*this - v).length();
	}

	inline float squaredDistance(const Vector& v) const
	{
		return (*this - v).squaredLength();
	}

	inline float dotProduct(const Vector& v) const
	{
		return x * v.x + y * v.y;
	}

	inline float normalise()
	{
		float iLength = sqrt( x * x + y * y );

		if(iLength > 0)
		{
			x /= iLength;
			y /= iLength;
		}

		return iLength;
	}

	inline Vector midPoint(const Vector& v) const
	{
		return Vector((x + v.x) * 0.5f, (y + v.y ) * 0.5f);
	}

	inline bool operator<(const Vector& v) const
	{
		if(x == v.x)
			return y < v.y;

		return x < v.x;
	}

	inline bool operator>(const Vector& v) const
	{
		if(x == v.x)
			return y > v.y;

		return x > v.x;
	}

	inline void makeFloor(const Vector& v)
	{
		if(v.x < x) x = v.x;
		if(v.y < y) y = v.y;
	}

	inline void makeCeil(const Vector& v)
	{
		if(v.x > x) x = v.x;
		if(v.y > y) y = v.y;
	}

	inline Vector perpendicular() const
	{
		return Vector(-y, x);
	}

	inline float crossProduct(const Vector& v) const
	{
		return x * v.y - y * v.x;
	}

	inline Vector randomDeviant(float angle) const
	{
		angle *= (float(rand()) / float(RAND_MAX)) * 3.1415f * 2.0f;
		float cosa = cos(angle);
		float sina = sin(angle);
		return Vector(cosa * x - sina * y, sina * x + cosa * y);
	}

	inline bool isZeroLength() const
	{
		return (x == 0 && y == 0);
	}

	inline Vector normalisedCopy() const
	{
		Vector ret = *this;
		ret.normalise();
		return ret;
	}

	inline Vector reflect(const Vector& normal) const
	{
		return Vector( *this - ( 2.0f * dotProduct(normal) * normal ) );
	}
};