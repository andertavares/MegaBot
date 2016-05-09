#pragma once
#include <Arbitrator.h>
#include <BWAPI.h>
#include <BuildManager.h>
#include <BuildingPlacer.h>
#include <BuildOrderManager.h>
#include <UnitGroupManager.h>
#include <MoodManager.h>
#include <ArmyManager.h>

class SpendManager
{
public:
	class Count
	{
	public:
		int		zealot;
		int		dragoon;
		int		corsair;
		int		darkTemplar;
		int		numberUnitTypes;

		float zealotRatio;
		float	dragoonRatio;
		float	corsairRatio;
		float	darkTemplarRatio;

		float zealotIdealRatio;
		float	dragoonIdealRatio;
		float	corsairIdealRatio;
		float	darkTemplarIdealRatio;
	};
	SpendManager();
	~SpendManager();

	void				setBuildManager				(BuildManager*);
	void				setBuildOrderManager	(BuildOrderManager*);
	void				setMoodManager				(MoodManager*);
	void				setArmyManager				(ArmyManager*);
	void				update								();
	std::string getName								() const;
	std::string getShortName					() const;
	Count*			getUnitCount					();
	int					getTotalUnits					();
	int					getTotalIdealUnits		();

private:
	BuildManager				*buildManager;
	BuildOrderManager		*buildOrderManager;
	MoodManager					*moodManager;
	ArmyManager					*armyManager;
	Count								*unitCount;
	int									lastFrameCheck;
	int									weaponUpgradeLevel;
	int									armorUpgradeLevel;
	int									totalUnits;
	int									totalIdealUnits;
	int									minerals;
	int									gas;
	int									coeff;
};