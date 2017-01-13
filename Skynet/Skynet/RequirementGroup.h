#pragma once

#include "Interface.h"

#include "Requirement.h"
#include "UnitGroup.h"

using namespace SkynetBot;

class RequirementGroup
{
public:
	RequirementGroup();

	std::pair<int, int> earliestTime();

	bool operator==(const RequirementGroup& other) const;
	bool operator<(const RequirementGroup& other) const;

	void reserve(int time);

	bool empty() const;
	void clear();

	SkynetBot::UnitGroup getUnits() const;

	inline void addMineralRequirement(int minerals) { mOtherRequirements.push_back(Requirement(RequirementType(RequirementType::Mineral), minerals)); }
	inline void addGasRequirement(int gas) { mOtherRequirements.push_back(Requirement(RequirementType(RequirementType::Gas), gas)); }
	inline void addSupplyRequirement(int supply) { mOtherRequirements.push_back(Requirement(RequirementType(RequirementType::Supply), supply)); }
	inline void addTimeRequirement(int time) { mOtherRequirements.push_back(Requirement(RequirementType(RequirementType::Time), time)); }

	inline void addUnitRequirement(Unit unit, int priority, int duration, Position position = BWAPI::Positions::None) { mUnitRequirements.push_back(Requirement(priority, duration, unit, position)); }
	inline void addUnitFilterRequirement(int priority, int duration, UnitFilter unitFilter, Position position = BWAPI::Positions::None) { mUnitRequirements.push_back(Requirement(priority, duration, unitFilter, position)); }
	void addUnitFilterRequirement(int priority, int duration, UnitFilter unitFilter, int quantity, Position position = BWAPI::Positions::None);

	inline void addTechForTypeRequirement(BWAPI::UnitType unitType) { mOtherRequirements.push_back(Requirement(unitType)); }
	inline void addTechForTypeRequirement(BWAPI::UpgradeType upgradeType, int level) { mOtherRequirements.push_back(Requirement(upgradeType, level)); }
	inline void addTechForTypeRequirement(BWAPI::TechType techType) { mOtherRequirements.push_back(Requirement(techType)); }

private:
	bool recurseForUnitTime(int &latestTime, std::vector<Requirement>::iterator currentRequirement, int currentStartTime, int currentEndTime, std::set<Unit> &currentUnits);

	std::vector<Requirement> mOtherRequirements;

	// ass units are reserved in slots, they need to be calculated recursively so keep them separated
	std::vector<Requirement> mUnitRequirements;
};