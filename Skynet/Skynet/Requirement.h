#pragma once

#include "Interface.h"

#include <limits>

#include "TypeSafeEnum.h"
#include "UnitFilter.h"
#include "Unit.h"

struct RequirementTypeDef
{
	enum type
	{
		Mineral,
		Gas,
		Supply,
		Unit,
		UnitFilter,
		RequiredForUnit,
		RequiredForUpgrade,
		RequiredForTech,
		Time,
		Task,
	};
};
typedef SafeEnum<RequirementTypeDef> RequirementType;

// Forward declare Task and TaskPointer
class Task;
typedef std::tr1::shared_ptr<Task> TaskPointer;

class Requirement
{
public:
	Requirement(RequirementType type, int amount);
	Requirement(int priority, int duration, Unit unit, Position position);
	Requirement(int priority, int duration, UnitFilter unitFilter, Position position);
	Requirement(TaskPointer task);
	Requirement(BWAPI::UnitType unit);
	Requirement(BWAPI::TechType tech);
	Requirement(BWAPI::UpgradeType upgrade, int level);

	int earliestTime();

	std::map<int, int> earliestUnitTime(int startTime, int endTime, std::set<Unit> &currentUnits);

	void reserve(int frameTime);

	RequirementType getType() { return mType; }

	bool unitRequirement() { return mType == RequirementType::Unit || mType == RequirementType::UnitFilter; }

	Unit getUnit() const { return mUnit; }

	int getDelay() { return mDelay; }

	bool operator==(const Requirement& other) const;
	bool operator<(const Requirement& other) const;

	static const int maxTime;

private:
	RequirementType mType;
	int mAmount;

	UnitFilter mUnitFilter;
	Unit mUnit;

	Position mPosition;
	int mPriority;
	int mDuration;
	int mDelay;

	TaskPointer mTask;

	BWAPI::UnitType mUnitType;
	BWAPI::TechType mTechType;
	BWAPI::UpgradeType mUpgradeType;

	int earliestTimeForType(BWAPI::UnitType unitType);
};