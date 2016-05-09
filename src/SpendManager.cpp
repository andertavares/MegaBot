#include <SpendManager.h>

SpendManager::SpendManager()
{
	buildOrderManager									= NULL;
	lastFrameCheck										= 0;
	weaponUpgradeLevel								= 0;
	armorUpgradeLevel									= 0;

	unitCount													= new Count();

	unitCount->zealot									= 0;
	unitCount->dragoon								= 0;
	unitCount->corsair								= 0;
	unitCount->darkTemplar						= 0;
	unitCount->numberUnitTypes				= 4;

	unitCount->zealotRatio						= 0.;
	unitCount->dragoonRatio						= 0.;
	unitCount->corsairRatio						= 0.;
	unitCount->darkTemplarRatio				= 0.;

	unitCount->zealotIdealRatio				= 0.;
	unitCount->dragoonIdealRatio			= 0.;
	unitCount->corsairIdealRatio			= 0.;
	unitCount->darkTemplarIdealRatio	= 0.;

	totalUnits												= 0;
	totalIdealUnits										= 0;

	minerals													=	0;
	gas																=	0;

	coeff															= 2;
}

SpendManager::~SpendManager()
{
	delete unitCount;
}

void SpendManager::setBuildManager(BuildManager *buildManager)
{
	this->buildManager = buildManager;
}

void SpendManager::setBuildOrderManager(BuildOrderManager *buildOrderManager)
{
	this->buildOrderManager = buildOrderManager;
}

void SpendManager::setMoodManager(MoodManager *moodManager)
{
	this->moodManager = moodManager;
}

void SpendManager::setArmyManager(ArmyManager *armyManager)
{
	this->armyManager = armyManager;
}

std::string SpendManager::getName() const
{
	return "SpendManager";
}

std::string SpendManager::getShortName() const
{
	return "Spend";
}

SpendManager::Count* SpendManager::getUnitCount()
{
	return unitCount;
}

int SpendManager::getTotalUnits()
{
	return totalUnits;
}

int SpendManager::getTotalIdealUnits()
{
	return totalIdealUnits;
}

void SpendManager::update()
{
	if (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 24)
	{
		lastFrameCheck					= BWAPI::Broodwar->getFrameCount();
		weaponUpgradeLevel			= BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Protoss_Ground_Weapons);
		armorUpgradeLevel				= BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Protoss_Ground_Armor);

		unitCount->zealot				= BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot);
		unitCount->dragoon			=	BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon);
		unitCount->corsair			=	BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Corsair);
		unitCount->darkTemplar	= BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dark_Templar);

		totalUnits							= unitCount->zealot + unitCount->dragoon + unitCount->corsair + unitCount->darkTemplar;
		totalIdealUnits					= moodManager->getPoints()->zealot + moodManager->getPoints()->dragoon + 
															moodManager->getPoints()->corsair + moodManager->getPoints()->darkTemplar;

		minerals								= BWAPI::Broodwar->self()->minerals();
		gas											= BWAPI::Broodwar->self()->gas();

		/* When our army size is very small, make an army must be a top priority */
		if (armyManager->myDPS() < 5)
			coeff = 2;
		else
			coeff = 1;


		if (totalUnits != 0)
		{
			unitCount->zealotRatio			=	((float)unitCount->zealot)			/ totalUnits;
			unitCount->dragoonRatio			=	((float)unitCount->dragoon)			/ totalUnits;
			unitCount->corsairRatio			=	((float)unitCount->corsair)			/ totalUnits;
			unitCount->darkTemplarRatio	=	((float)unitCount->darkTemplar)	/ totalUnits;
		}

		unitCount->zealotIdealRatio				=	((float)moodManager->getPoints()->zealot)				/ totalIdealUnits;
		unitCount->dragoonIdealRatio			=	((float)moodManager->getPoints()->dragoon)			/ totalIdealUnits;
		unitCount->corsairIdealRatio			=	((float)moodManager->getPoints()->corsair)			/ totalIdealUnits;
		unitCount->darkTemplarIdealRatio	=	((float)moodManager->getPoints()->darkTemplar)	/ totalIdealUnits;

		/* chc Áö¿ò
		if ((minerals > 300) && (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Forge) != 0) && 
										(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Photon_Cannon) == 0))
		{
			this->buildOrderManager->build(1, BWAPI::UnitTypes::Protoss_Photon_Cannon, 85, 
				buildManager->getBuildingPlacer()->getBuildLocationNear(BWAPI::Broodwar->self()->getStartLocation(),BWAPI::UnitTypes::Protoss_Photon_Cannon));
		}

		if (minerals > 150)
		{
			if ((unitCount->zealotRatio <= unitCount->zealotIdealRatio) && (unitCount->zealotIdealRatio != 0))
				this->buildOrderManager->build(unitCount->zealot + 1, BWAPI::UnitTypes::Protoss_Zealot, 
				(50 + (int)(50 * (1 - unitCount->zealotRatio / unitCount->zealotIdealRatio))) * coeff);
		}

		if (minerals > 400)
		{
			this->buildOrderManager->build(unitCount->zealot + 3, BWAPI::UnitTypes::Protoss_Zealot, 75 * coeff);
		}

		if (minerals > 1000)
		{
			UnitGroup myGateways = SelectAll(BWAPI::UnitTypes::Protoss_Gateway);
			this->buildOrderManager->build(myGateways.size() + 1, BWAPI::UnitTypes::Protoss_Gateway, 95);
			this->buildOrderManager->build(unitCount->zealot + 2, BWAPI::UnitTypes::Protoss_Zealot, 95);
			this->buildOrderManager->build(unitCount->dragoon + 2, BWAPI::UnitTypes::Protoss_Dragoon, 95);
		}

		if ((minerals > 200) && (gas > 75))
		{
			if ((unitCount->dragoonRatio <= unitCount->dragoonIdealRatio) && (unitCount->dragoonIdealRatio != 0))
				this->buildOrderManager->build(unitCount->dragoon + 1, BWAPI::UnitTypes::Protoss_Dragoon, 
				(50 + (int)(50 * (1 - unitCount->dragoonRatio / unitCount->dragoonIdealRatio))) * coeff);
		}

		if ((minerals > 230) && (gas > 150))
		{
			if ((unitCount->corsairRatio <= unitCount->corsairIdealRatio) && (unitCount->corsairIdealRatio != 0))
				this->buildOrderManager->build(unitCount->corsair + 1, BWAPI::UnitTypes::Protoss_Corsair, 
				(50 + (int)(50 * (1 - unitCount->corsairRatio / unitCount->corsairIdealRatio))) * coeff);
		}

		if (moodManager->getPoints()->corsair > 50 && BWAPI::Broodwar->self()->visibleUnitCount(BWAPI::UnitTypes::Protoss_Stargate) < 2 && 
			minerals > 200 && gas > 150)
		{
			this->buildOrderManager->build(2, BWAPI::UnitTypes::Protoss_Stargate, 85);
		}

		if ((minerals > 200) && (gas > 150))
		{
			if ((unitCount->darkTemplarRatio <= unitCount->darkTemplarIdealRatio) && (unitCount->darkTemplarIdealRatio != 0))
				this->buildOrderManager->build(unitCount->darkTemplar + 1, BWAPI::UnitTypes::Protoss_Dark_Templar, 
				(50 + (int)(50 * (1 - unitCount->darkTemplarRatio / unitCount->darkTemplarIdealRatio))) * coeff);
		}

		//upgrade weapon
		if ((minerals > (150 * weaponUpgradeLevel)) && (gas > (100 * weaponUpgradeLevel)) && (weaponUpgradeLevel < 3))
		{
			if (weaponUpgradeLevel == 0 && BWAPI::Broodwar->self()->visibleUnitCount(BWAPI::UnitTypes::Protoss_Citadel_of_Adun) == 0)
				this->buildOrderManager->build(1, BWAPI::UnitTypes::Protoss_Citadel_of_Adun, 75);
			if (weaponUpgradeLevel == 1 && BWAPI::Broodwar->self()->visibleUnitCount(BWAPI::UnitTypes::Protoss_Templar_Archives) == 0)
				this->buildOrderManager->build(1, BWAPI::UnitTypes::Protoss_Templar_Archives, 75);

			this->buildOrderManager->upgrade(weaponUpgradeLevel+1, BWAPI::UpgradeTypes::Protoss_Ground_Weapons, 85); 
		}

		//upgrade armor
		if (minerals > (150 * armorUpgradeLevel) && gas > (100 * armorUpgradeLevel) && armorUpgradeLevel < 3 && 
			(weaponUpgradeLevel == 3 || weaponUpgradeLevel > armorUpgradeLevel || 
			(weaponUpgradeLevel == armorUpgradeLevel && BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Protoss_Ground_Weapons))))
		{
			if (armorUpgradeLevel == 0 && BWAPI::Broodwar->self()->visibleUnitCount(BWAPI::UnitTypes::Protoss_Forge) < 2 && 
				BWAPI::Broodwar->getFrameCount() > 7000 && BWAPI::Broodwar->self()->visibleUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core) > 0)
				this->buildOrderManager->build(2, BWAPI::UnitTypes::Protoss_Forge, 75);
			this->buildOrderManager->upgrade(armorUpgradeLevel+1, BWAPI::UpgradeTypes::Protoss_Ground_Armor, 85); 
		}

		//upgrade observer sight
		if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Sensor_Array) == 0) && (BWAPI::Broodwar->getFrameCount() > 11000) && (minerals > 200) && (gas > 150))
		{
			this->buildOrderManager->upgrade(1, BWAPI::UpgradeTypes::Sensor_Array, 95);
		}

		//upgrade dragoon range
		if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge) == 0) && (BWAPI::Broodwar->getFrameCount() > 6000) && (minerals > 200) && (gas > 150))
		{
			this->buildOrderManager->upgrade(1, BWAPI::UpgradeTypes::Singularity_Charge, 100);
		}

		//upgrade zealot speed
		if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Leg_Enhancements) == 0) && (BWAPI::Broodwar->getFrameCount() > 7000) && (minerals > 200) && (gas > 150))
		{
			this->buildOrderManager->upgrade(1, BWAPI::UpgradeTypes::Leg_Enhancements, 100);
		}

		if(minerals > 400)
		{
			UnitGroup myGateways = SelectAll(BWAPI::UnitTypes::Protoss_Gateway);
			bool allTraining = true;
			for each (BWAPI::Unit *gateway in myGateways)
			{
				if (!gateway->isTraining())
				{
					allTraining = false;
					break;
				}
			}
			if (allTraining)
			{
				if (myGateways.size() < 10)
					this->buildOrderManager->build(myGateways.size()+1, BWAPI::UnitTypes::Protoss_Gateway,60);
			}
		}
		*/
	}
}
