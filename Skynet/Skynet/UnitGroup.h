#pragma once

#include "Interface.h"

class RegionClass;
typedef std::tr1::shared_ptr<RegionClass> Region;

namespace SkynetBot {
	class UnitGroup
	{
	public:
		UnitGroup();

		//Utility functions
		int countCompletedBy(int time) const;

		// 0 is equality, less than 0 for lose, greater than 0 for a win
		int ratingDifference(const UnitGroup &other) const;
		bool isWorthEngaging(const UnitGroup &other) const;

		Position getCenter() const;

		UnitGroup getBestFittingToCircle(int circleSize, int inFramesTime = 0) const;

		Unit getClosestUnit(Unit unit) const;
		Unit getClosestUnit(Position position) const;

		bool isAnyInRange(const Unit &unit) const;
		bool isAnyInRange(const UnitGroup &otherGroup) const;
		int minDistanceBetween(const UnitGroup &otherGroup) const;

		int getBuildScore() const;

		int getAverageTerrainHeight() const;
		double getAverageSpeed() const;
		Region getMajorityRegion() const;
		bool canAttackAir() const;
		bool canAttackGround() const;

		bool canMajorityAttack(const UnitGroup &other) const;

		bool hasDetection() const;
		std::set<Unit>::size_type getCloakedCount() const;
		std::set<Unit>::size_type getFlyingCount() const;

		// Functions to encapsulate the set, not derived as sets destructor is not virtual
		typedef std::set<Unit>::iterator iterator;
		typedef std::set<Unit>::const_iterator const_iterator;

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		void clear();
		std::set<Unit>::size_type count(Unit const &unit) const;
		bool empty() const;

		void erase(iterator position);
		std::set<Unit>::size_type erase(Unit const &unit);
		void erase(iterator first, iterator last);

		iterator find(Unit const &unit);
		std::set<Unit>::const_iterator find(Unit const &unit) const;

		std::pair<iterator, bool> insert(Unit const &unit);
		iterator insert(iterator position, Unit const &unit);
		void insert(const_iterator first, const_iterator last);

		std::set<Unit>::size_type size() const;

		bool operator==(const UnitGroup &other) const;
		bool operator!=(const UnitGroup &other) const;

		operator std::set<Unit>() const;

		UnitGroup &operator+=(const UnitGroup &other);
		UnitGroup operator+(const UnitGroup &other) const;
	
	private:
		std::set<Unit> mUnits;
	};
}
