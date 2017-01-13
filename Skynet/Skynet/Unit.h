#pragma once

#include "Interface.h"

#include "TypeSafeEnum.h"

struct AccessTypeDef
{
	enum type
	{
		Dead = 0,
		Lost,
		Guess,
		Prediction,
		Partial,
		Full
	};

	static std::string getName(type t)
	{
		switch(t)
		{
		case Dead:
			return "Dead";
		case Lost:
			return "Lost";
		case Guess:
			return "Guess";
		case Prediction:
			return "Prediction";
		case Partial:
			return "Partial";
		case Full:
			return "Full";
		}
		return "None";
	}
};
typedef SafeEnum<AccessTypeDef> AccessType;

class UnitClass : public std::tr1::enable_shared_from_this<UnitClass>
{
public:
	UnitClass(BWAPI::Unit* unit);
	UnitClass(Position pos, BWAPI::UnitType type, int completeTime);

	void onDestroy();

	TilePosition getTilePosition();
	Position getPosition();
	Position getPosition(int inFramesTime);
	Position getTargetPosition();

	BWAPI::UnitType getType();

	Player getPlayer();

	Unit getTarget();

	bool exists();
	int getExistTime();

	bool isMorphing();
	bool isCompleted();
	int getCompletedTime();

	bool isConstructing();
	bool isBeingConstructed();
	bool isTraining();
	bool isResearching();
	bool isUpgrading();

	int getRemainingTrainTime();
	int getRemainingUpgradeTime();
	int getRemainingResearchTime();

	int getResources();

	BWAPI::UpgradeType getUpgrade();
	BWAPI::TechType getTech();
	BWAPI::UnitType getBuildType();

	int getLastSeenTime();

	AccessType accessibility();

	void drawUnitPosition();
	void drawUnitTilePosition();

	BWAPI::Order getOrder();
	BWAPI::Order getSecondaryOrder();

	bool isLifted();
	bool isSieged();

	bool isCarryingGas();
	bool isCarryingMinerals();

	bool isLockedDown();
	bool isStasised();
	bool isCloaked();
	bool isBurrowed();
	bool isDetected();
	bool isStuck();
	bool isUnderStorm();
	bool isUnderDarkSwarm();
	bool isLoaded();

	bool hasAddon();

	bool canAttackNow(Unit unit);
	bool canAttack(Unit unit);
	bool isInRange(Unit unit);
	bool isAttackFrame();

	int getGroundWeaponMaxRange();
	int getAirWeaponMaxRange();
	int getWeaponMaxRange(Unit unit);
	int getWeaponMinRange(Unit unit);
	int getWeaponCooldown(Unit unit);
	int getRemainingCooldown(Unit unit);
	int getRemainingGroundCooldown();
	int getRemainingAirCooldown();
	int getSpellCooldown();

	int getTimeToKill(Unit unit);
	BWAPI::WeaponType getWeapon(Unit unit);

	bool canAttackGround();
	bool canAttackAir();

	int getAcidSporeCount();
	int getInterceptorCount();
	int getScarabCount();
	int getSpiderMineCount();

	int getID();

	double totalHitPointFraction();
	int totalHitPoints();
	int getHealth();
	int getShield();
	int getDefenseMatrixPoints();

	bool isRepairing();
	bool isHealing();
	bool isBeingHealed();
	bool isBeingRepaired();

	int getEnergy();
	std::list<BWAPI::UnitType> getTrainingQueue();

	int getDistance(BWAPI::UnitType targType, Position position);
	int getDistance(Unit unit);
	int getDistance(Position position);
	int getDistance(Position position, int inFramesTime);

	bool isSelected();

	bool hasPath(Position position);

	BWAPI::UnitCommand getLastCommand();

	bool hasOrder(BWAPI::Order order);

	//Commands
	void train(BWAPI::UnitType type);
	Unit getBuildUnit();
	void gather(Unit unit);
	void returnCargo();
	void returnCargo(Unit unit);
	void build(TilePosition target, BWAPI::UnitType type);
	void build(Unit unit);
	void morph(BWAPI::UnitType type);
	void stop();
	bool cancel(int slot = -2);
	void upgrade(BWAPI::UpgradeType type);
	void research(BWAPI::TechType mType);
	void move(Position target, int accuracy = 8);
	void attack(Unit unit);
	void attack(Position target, int accuracy = 8);
	void useTech(BWAPI::TechType tech, Position target);
	void useTech(BWAPI::TechType tech, Unit target);

	//Prediction
	void promote(BWAPI::Unit* unit);
	void setBuildTime(int time);
	void setPosition(Position position);

	//save
	void update();

private:
	BWAPI::Unit* mUnit;

	Position mStoredPosition;
	Position mStoredTargetPosition;
	BWAPI::UnitType mStoredType;
	Player mStoredPlayer;
	bool mStoredCompleted;
	bool mStoredMorphing;
	int mStoredTime;

	int mStoredCompletedTime;
	int mStoredExistsTime;

	int mStoredHealth;
	int mStoredShield;

	// getResources
	int mStoredInt;

	// isLifted, isSieged
	bool mStoredBoolOne;

	AccessType mStoredAccessType;
	int mDeleteTimer;
	
	int mLastOrderExecuteTime;
};

typedef std::tr1::shared_ptr<UnitClass> Unit;

class StaticUnits
{
public:
	static const Unit nullunit;
};