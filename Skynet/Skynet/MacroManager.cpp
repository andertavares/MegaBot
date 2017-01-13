#include "MacroManager.h"
#include "PlayerTracker.h"
#include "BuildOrderManager.h"
#include "TaskManager.h"
#include "UnitTracker.h"
#include "ResourceTracker.h"

MacroManagerClass::MacroManagerClass()
	: mLastStateChangeTime(0)
	, mDebugDraw(false)
{
}

void MacroManagerClass::onBegin()
{
	using namespace BWAPI::TechTypes;
	using namespace BWAPI::UpgradeTypes;

	//Terran Tech
	mTechPriorityMap[Tank_Siege_Mode] = 110;
	mTechPriorityMap[Spider_Mines] = 100;
	mTechPriorityMap[Stim_Packs] = 90;
	mTechPriorityMap[Yamato_Gun] = 90; //Battlecruiser
	mTechPriorityMap[Cloaking_Field] = 85;
	mTechPriorityMap[Irradiate] = PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Zerg) ? 89 : 0;
	mTechPriorityMap[EMP_Shockwave] = PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Protoss) ? 99 : 0;
	mTechPriorityMap[Personnel_Cloaking] = 75;
	mTechPriorityMap[Healing] = 10;
	mTechPriorityMap[Lockdown] = 10;
	mTechPriorityMap[Restoration] = 10;
	mTechPriorityMap[Optical_Flare] = 5;

	//Pre Researched
	mTechPriorityMap[Nuclear_Strike] = 0;
	mTechPriorityMap[Scanner_Sweep] = 0;
	mTechPriorityMap[Defensive_Matrix] = 0;

	//Zerg Tech
	mTechPriorityMap[Burrowing] = 100;
	mTechPriorityMap[Consume] = 100;
	mTechPriorityMap[Dark_Swarm] = 100;
	mTechPriorityMap[Ensnare] = 100;
	mTechPriorityMap[Infestation] = 100;
	mTechPriorityMap[Lurker_Aspect] = 100;
	mTechPriorityMap[Parasite] = 100;
	mTechPriorityMap[Plague] = 100;
	mTechPriorityMap[Spawn_Broodlings] = 100;

	//Protoss Tech
	mTechPriorityMap[Disruption_Web] = 0;
	mTechPriorityMap[Feedback] = 0;
	mTechPriorityMap[Hallucination] = 0;
	mTechPriorityMap[Maelstrom] = 0; //Dark Arcon freezes targeted units
	mTechPriorityMap[Mind_Control] = 0;
	mTechPriorityMap[Psionic_Storm] = 120;
	mTechPriorityMap[Recall] = 0;
	mTechPriorityMap[Stasis_Field] = 80;

	//Pre Researched
	mTechPriorityMap[Archon_Warp] = 0;
	mTechPriorityMap[Dark_Archon_Meld] = 0;

	//Terran Upgrades
	mUpgradePriorityMap[Terran_Infantry_Armor] = 85;
	mUpgradePriorityMap[Terran_Infantry_Weapons] = 90;
	mUpgradePriorityMap[Terran_Ship_Plating] = 65;
	mUpgradePriorityMap[Terran_Ship_Weapons] = 60;
	mUpgradePriorityMap[Terran_Vehicle_Plating] = 85;
	mUpgradePriorityMap[Terran_Vehicle_Weapons] = 90;
	mUpgradePriorityMap[U_238_Shells] = 85; //Marine Range
	mUpgradePriorityMap[Charon_Boosters] = 85; //Goliath Range
	mUpgradePriorityMap[Ion_Thrusters] = 95; //Vulture Speed
	mUpgradePriorityMap[Apollo_Reactor] = 40; //Wraith Energy
	mUpgradePriorityMap[Caduceus_Reactor] = 60; //Medic Energy
	mUpgradePriorityMap[Colossus_Reactor] = 40; //Battlecruiser Energy
	mUpgradePriorityMap[Moebius_Reactor] = 40; //Ghost Energy Upgrade
	mUpgradePriorityMap[Ocular_Implants] = 40; //Ghost Sight Range
	mUpgradePriorityMap[Titan_Reactor] = 70; //Science Vessel Energy

	//Zerg Upgrades
	mUpgradePriorityMap[Ventral_Sacs] = 80; //Overlord drop
	mUpgradePriorityMap[Zerg_Carapace] = 95;
	mUpgradePriorityMap[Zerg_Flyer_Attacks] = 100;
	mUpgradePriorityMap[Zerg_Flyer_Carapace] = 95;
	mUpgradePriorityMap[Zerg_Melee_Attacks] = 100;
	mUpgradePriorityMap[Zerg_Missile_Attacks] = 100;
	mUpgradePriorityMap[Metabolic_Boost] = 100; //Zergling Movement Speed
	mUpgradePriorityMap[Adrenal_Glands] = 100; //Zergling Attack Speed
	mUpgradePriorityMap[Anabolic_Synthesis] = 100; //Ultralisk Speed
	mUpgradePriorityMap[Antennae] = 100; //Overlord Sight Range
	mUpgradePriorityMap[Chitinous_Plating] = 100; //Ultralisk +2 armor
	mUpgradePriorityMap[Gamete_Meiosis] = 100; // Zerg queen Energy
	mUpgradePriorityMap[Grooved_Spines] = 100; // Hydralisk range
	mUpgradePriorityMap[Metasynaptic_Node] = 100; // Defiler Energy
	mUpgradePriorityMap[Muscular_Augments] = 100; //Hydralisk Speed
	mUpgradePriorityMap[Pneumatized_Carapace] = 100; //Overlord Speed

	//Protoss Upgrades
	mUpgradePriorityMap[Protoss_Air_Weapons] = 0;
	mUpgradePriorityMap[Protoss_Air_Armor] = 0;
	mUpgradePriorityMap[Protoss_Ground_Weapons] = 100;
	mUpgradePriorityMap[Protoss_Plasma_Shields] = 70;
	mUpgradePriorityMap[Protoss_Ground_Armor] = 95;
	mUpgradePriorityMap[Reaver_Capacity] = 85; //More scarabs
	mUpgradePriorityMap[Scarab_Damage] = 100; //Increase Reaver Scarab Damage
	mUpgradePriorityMap[Singularity_Charge] = 110; //Dragoon Range
	mUpgradePriorityMap[Carrier_Capacity] = 110; //MOAR INTERCEPTORS!
	mUpgradePriorityMap[Apial_Sensors] = 0; //Scout Sight
	mUpgradePriorityMap[Argus_Jewel] = 0; //Cosair Energy
	mUpgradePriorityMap[Argus_Talisman] = 100; //Dark Archon Energy
	mUpgradePriorityMap[Gravitic_Boosters] = 80; //Observer Speed
	mUpgradePriorityMap[Gravitic_Drive] = 97; //Shuttle Speed
	mUpgradePriorityMap[Gravitic_Thrusters] = 100; //Scout Speed
	mUpgradePriorityMap[Khaydarin_Amulet] = 90; //High Templar Energy
	mUpgradePriorityMap[Khaydarin_Core] = 80; //Arbitor Energy
	mUpgradePriorityMap[Leg_Enhancements] = 110; //Zealot Speed Upgrades
	mUpgradePriorityMap[Sensor_Array] = 60; // Observer Sight
}

void MacroManagerClass::update()
{
	updateTaskLists();

	if(BuildOrderManager::Instance().getOrder(Order::MacroArmyProduction))
	{
		updateObserverProduction();
		updateUnitProduction();
	}

	if(BuildOrderManager::Instance().getOrder(Order::MacroProductionFacilities))
		updateProductionProduction();

	if(BuildOrderManager::Instance().getOrder(Order::MacroCanTech))
		createTechItems();
	updateTech();

	if(mDebugDraw)
	{
		int y = 0;
		BWAPI::Broodwar->drawTextScreen(5, y+=10, "Units can produce:");
		for each(UnitToProduce unit in mLastUnitsToProduce)
		{
			BWAPI::Broodwar->drawTextScreen(5, y+=10, "%d %d %s", unit.getPriority(), unit.getUnitWeight(), unit.getUnitType().getName().c_str());
		}
		y+=10;

		BWAPI::Broodwar->drawTextScreen(5, y+=10, "Units in queue:");
		for(std::list<std::pair<TaskPointer, BWAPI::UnitType>>::iterator it = mUnitProduce.begin(); it != mUnitProduce.end(); ++it)
		{
			if(it->first->inProgress())
				BWAPI::Broodwar->drawTextScreen(5, y+=10, "In Progress : %s", it->second.getName().c_str() );
			else
				BWAPI::Broodwar->drawTextScreen(5, y+=10, "             : %s", it->second.getName().c_str() );
		}
	}
}

void MacroManagerClass::updateUnitProduction()
{
	if(BWAPI::Broodwar->getFrameCount() >= mLastStateChangeTime + 24*4)
	{
		TrainType trainType = TrainType::Normal;

		if(BWAPI::Broodwar->self()->supplyUsed() < 360)
		{
			int mineral = ResourceTracker::Instance().availableMineralAtTime(BWAPI::Broodwar->getFrameCount() + 24*6);
			int gas = ResourceTracker::Instance().availableGasAtTime(BWAPI::Broodwar->getFrameCount() + 24*6);

			if(std::max(gas, mineral) > 500)
			{
				float LowMineralRatio = gas == 0 ? 1 : float(mineral) / float(gas);
				float LowGasRatio = mineral == 0 ? 1 : float(gas) / float(mineral);

				if(LowMineralRatio < 0.3f)
					trainType = TrainType::LowMineral;
				else if(LowGasRatio < 0.7f)
					trainType = TrainType::LowGas;
			}
		}

		std::list<UnitToProduce> unitsToProduce;
		if(trainType == TrainType::LowMineral)
			unitsToProduce = mLowMineralUnits;
		else if(trainType == TrainType::LowGas)
			unitsToProduce = mLowGasUnits;
		else if(trainType == TrainType::Normal)
			unitsToProduce = mNormalUnits;

		for(std::list<UnitToProduce>::iterator it = unitsToProduce.begin(); it != unitsToProduce.end();)
		{
			if(!hasRequirements(it->getUnitType()) || !it->canBuildUnit())
				it = unitsToProduce.erase(it);
			else
				++it;
		}

		if(mLastUnitsToProduce != unitsToProduce)
		{
			for(std::list<std::pair<TaskPointer, BWAPI::UnitType>>::iterator  it = mUnitProduce.begin(); it != mUnitProduce.end();)
			{
				if(!it->first->inProgress())
				{
					it->first->cancel();
					it = mUnitProduce.erase(it);
				}
				else
					++it;
			}

			mLastUnitsToProduce = unitsToProduce;
			mLastStateChangeTime = BWAPI::Broodwar->getFrameCount();
		}
	}

	std::map<BWAPI::UnitType, int> totalPerProductionBuilding;
	std::map<BWAPI::UnitType, int> UnitTotals;
	std::map<BWAPI::UnitType, std::map<BWAPI::UnitType, int>> UnitToBuilding;
	std::map<BWAPI::UnitType, int> totalWeightPerBuilding;

	// TODO: try to maintain some sort of balance between buildings

	for each(UnitToProduce unit in mLastUnitsToProduce)
	{
		int plannedTotalUnit = UnitTracker::Instance().selectAllUnits(unit.getUnitType()).size();
		totalPerProductionBuilding[unit.getUnitType().whatBuilds().first] += plannedTotalUnit;
		UnitTotals[unit.getUnitType()] += plannedTotalUnit;

		totalWeightPerBuilding[unit.getUnitType().whatBuilds().first] += unit.getUnitWeight();
		UnitToBuilding[unit.getUnitType().whatBuilds().first][unit.getUnitType()] = unit.getUnitWeight();
	}

	for(std::list<std::pair<TaskPointer, BWAPI::UnitType>>::iterator it = mUnitProduce.begin(); it != mUnitProduce.end(); ++it)
	{
		if(!it->first->inProgress())
		{
			++totalPerProductionBuilding[it->second.whatBuilds().first];
			++UnitTotals[it->second];
		}
	}

	for each(std::pair<BWAPI::UnitType, int> totalPair in totalPerProductionBuilding)
	{
		int buildings = UnitTracker::Instance().selectAllUnits(totalPair.first).size() * 2;
		int queued = mTasksPerProductionType[totalPair.first].size();
		int freeProductionBuildings = buildings - queued;
		while(freeProductionBuildings > 0)
		{
			BWAPI::UnitType chosenUnit = BWAPI::UnitTypes::None;
			float biggestDifference = std::numeric_limits<float>::max();
			for each(std::pair<BWAPI::UnitType, int> producePair in UnitToBuilding[totalPair.first])
			{
				float neededRatio = float(producePair.second) / float(totalWeightPerBuilding[totalPair.first]);
				float currentRatio = 0;
				if(totalPair.second != 0)
					currentRatio = float(UnitTotals[producePair.first]) / float(totalPair.second);

				float difference = currentRatio - neededRatio;

				if(difference < biggestDifference)
				{
					biggestDifference = difference;
					chosenUnit = producePair.first;
				}
			}

			if(chosenUnit != BWAPI::UnitTypes::None)
			{
				mUnitProduce.push_back(std::make_pair(TaskManager::Instance().build(chosenUnit, TaskType::Army), chosenUnit));
				++totalPerProductionBuilding[chosenUnit.whatBuilds().first];
				++UnitTotals[chosenUnit];
			}

			freeProductionBuildings--;
		}
	}
}

bool macroCompare(const MacroItem &first, const MacroItem &second)
{
	return first.getPriority() > second.getPriority();
}

void MacroManagerClass::onChangeBuild()
{
	mNormalUnits.clear();
	mLowMineralUnits.clear();
	mLowGasUnits.clear();

	mNormalUnits = BuildOrderManager::Instance().getCurrentBuild().getUnitsToProduce();

	for each(UnitToProduce unit in mNormalUnits)
	{
		double mineralToGasRatio = unit.getUnitType().gasPrice() == 0 ? 1 : double(unit.getUnitType().mineralPrice())/double(unit.getUnitType().gasPrice());
		double gasToMineralRatio = unit.getUnitType().mineralPrice() == 0 ? 1 : double(unit.getUnitType().gasPrice())/double(unit.getUnitType().mineralPrice());

		if(gasToMineralRatio < 0.4)
			mLowGasUnits.push_back(unit);
		else if(mineralToGasRatio < 0.4)
			mLowMineralUnits.push_back(unit);
	}

	for(std::list<std::pair<MacroItem, TaskPointer>>::iterator it = mTechItems.begin(); it != mTechItems.end(); ++it)
	{
		if(!it->second->inProgress())
			it->second->cancel();
	}
	mTechItems.clear();
	mTechItemsToCreate.clear();

	updateTaskLists();

	std::list<UnitToProduce> normalWithExtra = mNormalUnits;

	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
	{
		normalWithExtra.push_back(UnitToProduce(BWAPI::UnitTypes::Protoss_Observer, 1, 115));
		normalWithExtra.push_back(UnitToProduce(BWAPI::UnitTypes::Protoss_Photon_Cannon, 1, 95));
	}

	std::set<BWAPI::TechType> techSet;
	std::set<BWAPI::UpgradeType> upgradeSet;

	for each(UnitToProduce unit in normalWithExtra)
	{
		for each(BWAPI::UnitType type in getNeededUnits(unit.getUnitType()))
		{
			mTechItemsToCreate.push_back(MacroItem(type, unit.getPriority()));
		}

		for each(BWAPI::TechType tech in unit.getUnitType().abilities())
		{
			techSet.insert(tech);
		}

		for each(BWAPI::UpgradeType upgrade in unit.getUnitType().upgrades())
		{
			upgradeSet.insert(upgrade);
		}

		if(unit.getUnitType() == BWAPI::UnitTypes::Protoss_Reaver)
		{
			// Scarab damage doesn't pick up under its upgrades
			upgradeSet.insert(BWAPI::UpgradeTypes::Scarab_Damage);

			// Reaver is usually used with shuttle, speed upgrade will probably help
			upgradeSet.insert(BWAPI::UpgradeTypes::Gravitic_Drive);
		}
		else if(unit.getUnitType() == BWAPI::UnitTypes::Protoss_Carrier)
			upgradeSet.insert(BWAPI::UpgradeTypes::Protoss_Air_Weapons);
	}

	for each(BWAPI::TechType tech in techSet)
	{
		if(mTechPriorityMap[tech] < 30)
			continue;

		mTechItemsToCreate.push_back(MacroItem(tech, mTechPriorityMap[tech]));

		for each(BWAPI::UnitType type in getNeededUnits(tech))
		{
			mTechItemsToCreate.push_back(MacroItem(type, mTechPriorityMap[tech]));
		}
	}

	for each(BWAPI::UpgradeType upgrade in upgradeSet)
	{
		if(mUpgradePriorityMap[upgrade] < 30)
			continue;

		for(int i = 1; i <= upgrade.maxRepeats(); ++i)
		{
			int priority = mUpgradePriorityMap[upgrade] - ((i-1)*10);
			mTechItemsToCreate.push_back(MacroItem(upgrade, i, priority));

			for each(BWAPI::UnitType type in getNeededUnits(upgrade, i))
			{
				mTechItemsToCreate.push_back(MacroItem(type, priority));
			}
		}
	}

	mTechItemsToCreate.sort(macroCompare);

	//remove duplicates now so the highest is kept
	std::set<BWAPI::UnitType> unitSet;
	for(std::list<MacroItem>::iterator it = mTechItemsToCreate.begin(); it != mTechItemsToCreate.end();)
	{
		if(it->isUnitType())
		{
			if(unitSet.count(it->getUnitType()) != 0)
			{
				mTechItemsToCreate.erase(it++);
				continue;
			}
			else
				unitSet.insert(it->getUnitType());
		}

		++it;
	}

	createTechItems();
}

void MacroManagerClass::updateObserverProduction()
{
	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
	{
		if(hasRequirements(BWAPI::UnitTypes::Protoss_Observer))
		{
			TaskType type = TaskType::Defense;
			int num = getPlannedTotal(BWAPI::UnitTypes::Protoss_Observer);
			if(num > 3)
				return;
			else if(num == 0)
				type = TaskType::MacroUrgent;
			else if(num == 1)
				type = TaskType::Army;

			// Cancel and recreate if its the wrong type incase it was create when we had more but are now low on observers
			if(!mObserver || mObserver->inProgress() || mObserver->hasEnded() || mObserver->getType() != type)
			{
				if(mObserver && !mObserver->inProgress())
					mObserver->cancel();

				mObserver = TaskManager::Instance().build(BWAPI::UnitTypes::Protoss_Observer, type);
			}
		}
	}
}

void MacroManagerClass::updateProductionProduction()
{
	for each(UnitToProduce unit in mNormalUnits)
	{
		if(hasRequirements(unit.getUnitType()) && unit.canBuildFactory())
		{
			BWAPI::UnitType whatBuilds = unit.getUnitType().whatBuilds().first;
			if(whatBuilds == BWAPI::UnitTypes::Zerg_Larva)
				whatBuilds = BWAPI::UnitTypes::Zerg_Hatchery;

			bool unstartedBuild = false;
			for each(TaskPointer task in mTasksPerProducedType[whatBuilds])
			{
				if(!task->inProgress())
					unstartedBuild = true;
			}
			if(unstartedBuild)
				continue;

			int idleOfThis = 0;
			for each(Unit building in UnitTracker::Instance().selectAllUnits(whatBuilds))
			{
				if(building->isCompleted() && !building->isTraining())
					++idleOfThis;
			}

			if(idleOfThis == 0)
				TaskManager::Instance().build(whatBuilds, TaskType::MacroExtraProduction);
		}
	}
}
bool MacroManagerClass::hasRequirements(BWAPI::UnitType type)
{
	if(BWAPI::Broodwar->self()->completedUnitCount(type.whatBuilds().first) == 0)
		return false;

	for(std::map<BWAPI::UnitType, int>::const_iterator it = type.requiredUnits().begin(); it != type.requiredUnits().end(); ++it)
	{
		if(BWAPI::Broodwar->self()->completedUnitCount(it->first) == 0)
			return false;
	}

	return true;
}

bool MacroManagerClass::hasRequirements(BWAPI::TechType type)
{
	if(BWAPI::Broodwar->self()->completedUnitCount(type.whatResearches()) == 0)
		return false;

	return true;
}

bool MacroManagerClass::hasRequirements(BWAPI::UpgradeType type, int level)
{
	if(BWAPI::Broodwar->self()->completedUnitCount(type.whatUpgrades()) == 0)
		return false;

	if(level != 1 && BWAPI::Broodwar->self()->getUpgradeLevel(type) != (level - 1))
		return false;

	if(level > 1)
	{
		if((type == BWAPI::UpgradeTypes::Protoss_Ground_Weapons || type == BWAPI::UpgradeTypes::Protoss_Ground_Armor) && BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Templar_Archives) == 0)
			return false;
		else if(type == BWAPI::UpgradeTypes::Protoss_Plasma_Shields && BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core) == 0)
			return false;
		else if((type == BWAPI::UpgradeTypes::Protoss_Air_Armor || type == BWAPI::UpgradeTypes::Protoss_Air_Weapons) && BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Fleet_Beacon) == 0)
			return false;
	}

	return true;
}

std::set<BWAPI::UnitType> MacroManagerClass::getNeededUnits(BWAPI::UnitType type)
{
	std::set<BWAPI::UnitType> neededUnits;

	BWAPI::UnitType whatBuilds = type.whatBuilds().first;
	if(whatBuilds == BWAPI::UnitTypes::Zerg_Larva)
		whatBuilds = BWAPI::UnitTypes::Zerg_Hatchery;

	if(!isPlanningUnit(whatBuilds))
		neededUnits.insert(whatBuilds);

	for(std::map<BWAPI::UnitType, int>::const_iterator it = type.requiredUnits().begin(); it != type.requiredUnits().end(); ++it)
	{
		if(it->first == BWAPI::UnitTypes::Zerg_Larva)
			continue;

		if(!isPlanningUnit(it->first))
			neededUnits.insert(it->first);

	}

	addNeeded(neededUnits);

	return neededUnits;
}

std::set<BWAPI::UnitType> MacroManagerClass::getNeededUnits(BWAPI::TechType type)
{
	std::set<BWAPI::UnitType> neededUnits;
	if(!isPlanningUnit(type.whatResearches()))
		neededUnits.insert(type.whatResearches());

	addNeeded(neededUnits);

	return neededUnits;
}

std::set<BWAPI::UnitType> MacroManagerClass::getNeededUnits(BWAPI::UpgradeType type, int level)
{
	std::set<BWAPI::UnitType> neededUnits;
	if(!isPlanningUnit(type.whatUpgrades()))
		neededUnits.insert(type.whatUpgrades());

	if(level > 1)
	{
		if((type == BWAPI::UpgradeTypes::Protoss_Ground_Weapons || type == BWAPI::UpgradeTypes::Protoss_Ground_Armor) && !isPlanningUnit(BWAPI::UnitTypes::Protoss_Templar_Archives))
			neededUnits.insert(BWAPI::UnitTypes::Protoss_Templar_Archives);
		else if(type == BWAPI::UpgradeTypes::Protoss_Plasma_Shields && !isPlanningUnit(BWAPI::UnitTypes::Protoss_Cybernetics_Core))
			neededUnits.insert(BWAPI::UnitTypes::Protoss_Cybernetics_Core);
		else if((type == BWAPI::UpgradeTypes::Protoss_Air_Armor || type == BWAPI::UpgradeTypes::Protoss_Air_Weapons) && !isPlanningUnit(BWAPI::UnitTypes::Protoss_Fleet_Beacon))
			neededUnits.insert(BWAPI::UnitTypes::Protoss_Fleet_Beacon);
	}

	addNeeded(neededUnits);

	return neededUnits;
}

void MacroManagerClass::addNeeded(std::set<BWAPI::UnitType> &neededUnits)
{
	bool addedUnits = false;
	std::set<BWAPI::UnitType> tempList;
	for each(BWAPI::UnitType type in neededUnits)
	{
		BWAPI::UnitType whatBuilds = type.whatBuilds().first;
		if(whatBuilds == BWAPI::UnitTypes::Zerg_Larva)
			whatBuilds = BWAPI::UnitTypes::Zerg_Hatchery;

		if(!isPlanningUnit(whatBuilds) && neededUnits.count(whatBuilds) == 0)
		{
			addedUnits = true;
			tempList.insert(whatBuilds);
		}

		for(std::map<BWAPI::UnitType, int>::const_iterator it = type.requiredUnits().begin(); it != type.requiredUnits().end(); ++it)
		{
			if (it->first == BWAPI::UnitTypes::Zerg_Larva)
				continue;

			if(!isPlanningUnit(it->first) && neededUnits.count(it->first) == 0)
			{
				addedUnits = true;
				tempList.insert(it->first);
			}
		}
	}

	neededUnits.insert(tempList.begin(), tempList.end());

	if(addedUnits)
		addNeeded(neededUnits);
}

int MacroManagerClass::getPlannedCount(BWAPI::UnitType unitType)
{
	int count = 0;
	for each(TaskPointer task in mTasksPerProducedType[unitType])
	{
		if(!task->inProgress())
			++count;
	}

	return count;
}

int MacroManagerClass::getPlannedTotal(BWAPI::UnitType unitType)
{
	return getPlannedCount(unitType) + BWAPI::Broodwar->self()->allUnitCount(unitType);
}

bool MacroManagerClass::isPlanningUnit(BWAPI::UnitType unitType)
{
	return getPlannedTotal(unitType) != 0;
}

int MacroManagerClass::getPlannedCount(BWAPI::TechType techType)
{
	int count = 0;
	for each(TaskPointer task in mTasksPerTechType[techType])
	{
		if(!task->inProgress())
			++count;
	}

	return count;
}

int MacroManagerClass::getPlannedTotal(BWAPI::TechType techType)
{
	return getPlannedCount(techType) + (BWAPI::Broodwar->self()->hasResearched(techType) ? 1 : 0);
}

bool MacroManagerClass::isPlanningTech(BWAPI::TechType techType)
{
	return getPlannedTotal(techType) != 0;
}

int MacroManagerClass::getPlannedCount(BWAPI::UpgradeType upgradeType, int level)
{
	int count = 0;
	for each(TaskPointer task in mTasksPerUpgradeType[upgradeType][level])
	{
		if(!task->inProgress())
			++count;
	}

	return count;
}

int MacroManagerClass::getPlannedTotal(BWAPI::UpgradeType upgradeType, int level)
{
	return getPlannedCount(upgradeType, level) + (BWAPI::Broodwar->self()->getUpgradeLevel(upgradeType) >= level ? 1 : 0);
}

bool MacroManagerClass::isPlanningUpgrade(BWAPI::UpgradeType upgradeType, int level)
{
	return getPlannedTotal(upgradeType, level) != 0;
}

void MacroManagerClass::updateTaskLists()
{
	for(std::list<std::pair<TaskPointer, BWAPI::UnitType>>::iterator it = mUnitProduce.begin(); it != mUnitProduce.end();)
	{
		if(it->first->hasEnded()) mUnitProduce.erase(it++);
		else ++it;
	}

	for(std::map<BWAPI::UnitType, std::list<TaskPointer>>::iterator it = mTasksPerProductionType.begin(); it != mTasksPerProductionType.end(); ++it)
	{
		for(std::list<TaskPointer>::iterator it2 = it->second.begin(); it2 != it->second.end();)
		{
			if((*it2)->hasEnded()) it->second.erase(it2++);
			else ++it2;
		}
	}

	for(std::map<BWAPI::UnitType, std::list<TaskPointer>>::iterator it = mTasksPerProducedType.begin(); it != mTasksPerProducedType.end(); ++it)
	{
		for(std::list<TaskPointer>::iterator it2 = it->second.begin(); it2 != it->second.end();)
		{
			if((*it2)->hasEnded()) it->second.erase(it2++);
			else ++it2;
		}
	}

	for(std::map<BWAPI::TechType, std::list<TaskPointer>>::iterator it = mTasksPerTechType.begin(); it != mTasksPerTechType.end(); ++it)
	{
		for(std::list<TaskPointer>::iterator it2 = it->second.begin(); it2 != it->second.end();)
		{
			if((*it2)->hasEnded()) it->second.erase(it2++);
			else ++it2;
		}
	}

	for(std::map<BWAPI::UpgradeType, std::map<int, std::list<TaskPointer>>>::iterator it = mTasksPerUpgradeType.begin(); it != mTasksPerUpgradeType.end(); ++it)
	{
		for(std::map<int, std::list<TaskPointer>>::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1)
		{
			for(std::list<TaskPointer>::iterator it2 = it1->second.begin(); it2 != it1->second.end();)
			{
				if((*it2)->hasEnded()) it1->second.erase(it2++);
				else ++it2;
			}
		}
	}
}

void MacroManagerClass::updateTech()
{
	for(std::list<std::pair<MacroItem, TaskPointer>>::iterator it = mTechItems.begin(); it != mTechItems.end();)
	{
		// if its ended or something else is creating this, cancel it to avoid duplicates
		if(it->second->hasEnded() || (!it->second->inProgress() && it->first.inProgress()))
		{
			it->second->cancel();
			mTechItems.erase(it++);
		}
		else
			++it;
	}
}

void MacroManagerClass::onBuildTask(TaskPointer task, BWAPI::UnitType unitType)
{
	mTasksPerProductionType[unitType.whatBuilds().first].push_back(task);
	mTasksPerProducedType[unitType].push_back(task);
}

void MacroManagerClass::onTechTask(TaskPointer task,BWAPI::TechType techType)
{
	mTasksPerProductionType[techType.whatResearches()].push_back(task);
	mTasksPerTechType[techType].push_back(task);
}

void MacroManagerClass::onUpgradeTask(TaskPointer task, BWAPI::UpgradeType upgradeType, int level)
{
	mTasksPerProductionType[upgradeType.whatUpgrades()].push_back(task);
	mTasksPerUpgradeType[upgradeType][level].push_back(task);
}

void MacroManagerClass::createTechItems()
{
	if(!mTechItemsToCreate.empty() && BuildOrderManager::Instance().getOrder(Order::MacroCanTech))
	{
		for(std::list<MacroItem>::iterator it = mTechItemsToCreate.begin(); it != mTechItemsToCreate.end(); ++it)
		{
			TaskPointer task = it->createTask(TaskType::MacroTech);
			if(task)
				mTechItems.push_back(std::make_pair(*it, task));
		}

		mTechItemsToCreate.clear();
	}
}

bool MacroItem::inProgress() const
{
	if(isUnitType())
		return MacroManager::Instance().getPlannedTotal(mUnit) > 1;
	else if(isTechType())
		return MacroManager::Instance().getPlannedTotal(mTech) > 1;
	else if(isUpgradeType())
		return MacroManager::Instance().getPlannedTotal(mUpgrade, mLevel) > 1;
	else
		return true;
}

TaskPointer MacroItem::createTask(TaskType taskType) const
{
	if(isUnitType())
		return TaskManager::Instance().build(mUnit, taskType);
	else if(isTechType())
		return TaskManager::Instance().research(mTech, taskType);
	else if(isUpgradeType())
		return TaskManager::Instance().upgrade(mUpgrade, mLevel, taskType);
	else
		return TaskPointer();
}