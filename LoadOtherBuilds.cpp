#include "BuildOrderManager.h"

#include "PlayerTracker.h"

void BuildOrderManagerClass::LoadOtherBuilds()
{
	using namespace BWAPI::Races;
	using namespace BWAPI::UnitTypes;
	using namespace BWAPI::UpgradeTypes;
	using namespace BWAPI::TechTypes;

	int ID_1 = 0;
	int ID_2 = 0;
	int ID_3 = 0;

	mBuildOrders[BuildOrderID::Unknown] = BuildOrder(BuildOrderID::Unknown, "Unknown");
	mBuildOrders[BuildOrderID::Unknown].setArmyBehaviour(ArmyBehaviour::Default);
	mBuildOrders[BuildOrderID::Unknown].addSquad(SquadType::DefaultSquad, 1);

	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::TrainWorkers);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::Scout);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::SupplyManager);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::RefineryManager);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::MacroArmyProduction);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::CanRemoveSquads);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::ExpansionManager);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::MacroProductionFacilities);
	mBuildOrders[BuildOrderID::Unknown].addOrder(Order::MacroCanTech);

	if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
	{
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Protoss_Zealot, 2);
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Protoss_Dragoon, 3);
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Protoss_High_Templar, 1);
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Protoss_Arbiter, 1);
	}
	else if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran)
	{
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Terran_Marine, 5);
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Terran_Medic, 1);
		if(PlayerTracker::Instance().isEnemyRace(BWAPI::Races::Zerg))
			mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Terran_Firebat, 1);
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Terran_Science_Vessel, 1);
	}
	else if(BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg)
	{
		mBuildOrders[BuildOrderID::Unknown].addProduce(BWAPI::UnitTypes::Zerg_Zergling, 1);
	}

	mBuildOrders[BuildOrderID::None] = BuildOrder(BuildOrderID::None, "None");

// 	BuildOrder testBuild(Protoss, BuildOrderID::Test, "Test Build");
// 
// 	testBuild.addStartingCondition(Condition(ConditionType::defaultCondition));
// 
// 	mBuildOrders[BuildOrderID::Test] = testBuild;
}