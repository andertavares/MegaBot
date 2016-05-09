#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include <map>
class InformationManager
{
public:
	class UnitData
	{
	public:
		UnitData();
		BWAPI::Position position;
		BWAPI::UnitType type;
		BWAPI::Player		*player;
		int							lastSeenTime;
		bool						exists;
	};

	const std::set<BWTA::BaseLocation*>&		getEnemyBases	() const;
	const std::map<BWAPI::Unit*, UnitData>& getSavedData	() const;

	static InformationManager*	create();
	static void									destroy();
	void												onUnitDiscover				(BWAPI::Unit*);
	void												onUnitEvade						(BWAPI::Unit*);
	void												onUnitDestroy					(BWAPI::Unit*);
	BWAPI::Player*							getPlayer							(BWAPI::Unit*) const;
	BWAPI::UnitType							getType								(BWAPI::Unit*) const;
	BWAPI::Position							getLastPosition				(BWAPI::Unit*) const;
	int													getLastSeenTime				(BWAPI::Unit*) const;
	bool												exists								(BWAPI::Unit*) const;
	bool												enemyHasBuilt					(BWAPI::UnitType) const;
	int													getBuildTime					(BWAPI::UnitType) const;
	void												setBaseEmpty					(BWTA::BaseLocation*);
	void												setEnemyStartLocation	(BWTA::BaseLocation*);
	BWTA::BaseLocation*					getEnemyStartLocation	();

private:
	InformationManager();
	~InformationManager();

	void updateBuildTime(BWAPI::UnitType, int);

	std::map<BWAPI::Unit*, UnitData>						savedData;
	std::map<BWAPI::UnitType, int>							buildTime;
	std::set<BWTA::BaseLocation*>								enemyBases;
	std::map<BWTA::BaseLocation*,BWAPI::Unit*>	enemyBaseCenters;
	std::set<BWTA::BaseLocation*>								startLocationCouldContainEnemy;
	BWTA::BaseLocation*													enemyStartLocation;
	bool																				scoutedAnEnemyBase;
};
extern InformationManager* TheInformationManager;