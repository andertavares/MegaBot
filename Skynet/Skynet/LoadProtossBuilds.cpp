#include "BuildOrderManager.h"

void BuildOrderManagerClass::LoadProtossBuilds()
{
	using namespace BWAPI::Races;
	using namespace BWAPI::UnitTypes;
	using namespace BWAPI::UpgradeTypes;
	using namespace BWAPI::TechTypes;

	int ID_1 = 0;
	int ID_2 = 0;
	int ID_3 = 0;

	/**************************************************************************************************/
	/* Against Zerg                                                                                   */
	/**************************************************************************************************/

	Condition corsairBuildCondition(Condition(ConditionTest::myUnitTotalBuildCountLessThan, Protoss_Corsair, 12) || (Condition(ConditionTest::myUnitCountLessThan, Protoss_Corsair, 12) && Condition(ConditionTest::enemyUnitCountGreaterEqualThan, Zerg_Mutalisk, 12)));
	Condition corsairStargateCondition(ConditionTest::myPlannedUnitTotalLessThan, Protoss_Stargate, 2);

	/************************************************************************/
	/* Forge Expand                                                         */
	/************************************************************************/

	BuildOrder forgeExpand(Protoss, BuildOrderID::ForgeExpand, "Forge Expand");

	//Starting Conditions
	forgeExpand.setStartingCondition(Condition(ConditionTest::numberOfEnemies, 1) && Condition(ConditionTest::isEnemyZerg) && Condition(ConditionTest::canForgeExpand));
	//Condition(ConditionType::minDistanceBetweenMainsGreaterThan, double(BWAPI::UnitTypes::Protoss_Photon_Cannon.buildTime()*BWAPI::UnitTypes::Zerg_Zergling.topSpeed())); //meh, if we know how to forge expand, must be viable

	forgeExpand.addArmyBehaviour(ArmyBehaviour::Defensive);

	forgeExpand.addNextBuild(BuildOrderID::StargateArcon);

	//Units to Produce
	forgeExpand.addProduce(Protoss_Zealot, 1);

	// Build Order
			forgeExpand.addItem(Protoss_Probe, 4, TaskType::Highest);
	ID_1 =	forgeExpand.addItem(Protoss_Pylon, 1, BuildingLocation::BaseChoke, TaskType::Highest); //Pylon on 8
			forgeExpand.addOrder(Order::Scout, CB(ID_1, CallBackType::onStarted));

			forgeExpand.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	forgeExpand.addItem(Protoss_Forge, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::BaseChoke); //Forge on 10
			forgeExpand.addOrder(Order::TrainWorkers, CB(ID_1, CallBackType::onDispatched));
			forgeExpand.addOrder(Order::SupplyManager, CB(ID_1, CallBackType::onDispatched));

	ID_1 =	forgeExpand.addItem(Protoss_Photon_Cannon, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::BaseChoke);
	ID_1 =	forgeExpand.addItem(Protoss_Photon_Cannon, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::BaseChoke);
	ID_1 =	forgeExpand.addItem(Protoss_Nexus, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::ExpansionGas);
	ID_1 =	forgeExpand.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::BaseChoke);

	// Add
	mBuildOrders[BuildOrderID::ForgeExpand] = forgeExpand;

	/************************************************************************/
	/* Stargate Arcon                                                       */
	/************************************************************************/

	BuildOrder starArcon(Protoss, BuildOrderID::StargateArcon, "Stargate into Arcon");

	starArcon.addNextBuild(BuildOrderID::ArconTiming, 0, Condition(ConditionTest::myPlannedUnitTotalGreaterEqualThan, Protoss_Archon, 3));//go to timing attack if i have 2 arcons
	starArcon.addNextBuild(BuildOrderID::ArconTiming, 24*60*4);//or 4 mins have passed

	starArcon.addArmyBehaviour(ArmyBehaviour::Defensive);

	// Constants
	starArcon.addOrder(Order::TrainWorkers);
	starArcon.addOrder(Order::MacroArmyProduction);
	starArcon.addOrder(Order::SupplyManager);
	starArcon.addOrder(Order::Scout);

	// Squads
	starArcon.addSquad(SquadType::CorsairSquad);

	//Units to Produce
	starArcon.addProduce(Protoss_High_Templar, 2, 100, Condition(ConditionTest::isResearching, Psionic_Storm));
	starArcon.addProduce(Protoss_Corsair, 3, 100, corsairBuildCondition, corsairStargateCondition);
	starArcon.addProduce(Protoss_Zealot, 1);
	starArcon.addProduce(Protoss_Dragoon, 1);

	// Build Order
			starArcon.addItem(Protoss_Zealot, 6, TaskType::Army);
			starArcon.addItem(Protoss_Assimilator);
	ID_1 =	starArcon.addItem(Protoss_Cybernetics_Core);
			starArcon.addItem(Protoss_Stargate, 1, TaskType::Highest);
			starArcon.addItem(Protoss_Corsair, 2, TaskType::Highest);

			starArcon.addOrder(Order::RefineryManager, CB(ID_1, CallBackType::onDispatched));
			starArcon.addItem(Protoss_Ground_Weapons, 1, CB(ID_1, CallBackType::onDispatched));
			starArcon.addItem(Protoss_Gateway, TaskType::MacroExtraProduction, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	starArcon.addItem(Protoss_Citadel_of_Adun, TaskType::Army, CB(ID_1, CallBackType::onDispatched));
			starArcon.addItem(Protoss_Photon_Cannon, TaskType::Army, CB(ID_1, CallBackType::onDispatched), 4);
			

			starArcon.addItem(Leg_Enhancements, 1, TaskType::MacroUrgent, CB(ID_1, CallBackType::onDispatched));
			starArcon.addItem(Protoss_Templar_Archives, TaskType::Army, CB(ID_1, CallBackType::onDispatched));
			starArcon.addItem(Psionic_Storm, TaskType::MacroUrgent, CB(ID_1, CallBackType::onDispatched));

			starArcon.addOrder(Order::MacroProductionFacilities,  CB(ID_1, CallBackType::onDispatched));
	

	// Add
	mBuildOrders[BuildOrderID::StargateArcon] = starArcon;

	/************************************************************************/
	/* Arcon Timing                                                         */
	/************************************************************************/

	BuildOrder ArconTiming(Protoss, BuildOrderID::ArconTiming, "Arcon Timing Attack");

	// Constants
	ArconTiming.addOrder(Order::TrainWorkers);
	ArconTiming.addOrder(Order::SupplyManager);
	ArconTiming.addOrder(Order::MacroArmyProduction);
	ArconTiming.addOrder(Order::RefineryManager);
	ArconTiming.addOrder(Order::MacroProductionFacilities);
	ArconTiming.addOrder(Order::MacroCanTech);
	ArconTiming.addOrder(Order::ExpansionManager);
	ArconTiming.addOrder(Order::Scout);

	ArconTiming.addNextBuild(BuildOrderID::PvZEndGame, 24*60*1);

	// Squads
	ArconTiming.addSquad(SquadType::CorsairSquad);

	//Units to Produce
	ArconTiming.addProduce(Protoss_High_Templar, 2, 100, Condition(ConditionTest::isResearching, Psionic_Storm));
	ArconTiming.addProduce(Protoss_Corsair, 3, 100, corsairBuildCondition, corsairStargateCondition);
	ArconTiming.addProduce(Protoss_Zealot, 1);
	ArconTiming.addProduce(Protoss_Dragoon, 1);

	ArconTiming.addArmyBehaviour(ArmyBehaviour::Aggresive);

	// Add
	mBuildOrders[BuildOrderID::ArconTiming] = ArconTiming;

	/************************************************************************/
	/* PvZ End Game                                                         */
	/************************************************************************/

	BuildOrder pvzEndGame(Protoss, BuildOrderID::PvZEndGame, "PvZ End Game");

	// Constants
	pvzEndGame.addOrder(Order::TrainWorkers);
	pvzEndGame.addOrder(Order::Scout);
	pvzEndGame.addOrder(Order::SupplyManager);
	pvzEndGame.addOrder(Order::RefineryManager);
	pvzEndGame.addOrder(Order::MacroArmyProduction);
	pvzEndGame.addOrder(Order::CanRemoveSquads);
	pvzEndGame.addOrder(Order::ExpansionManager);
	pvzEndGame.addOrder(Order::MacroProductionFacilities);
	pvzEndGame.addOrder(Order::MacroCanTech);

	// Squads
	pvzEndGame.addSquad(SquadType::CorsairSquad);

	//Units to Produce
	pvzEndGame.addProduce(Protoss_High_Templar, 2, 110, Condition(ConditionTest::isResearching, Psionic_Storm));
	pvzEndGame.addProduce(Protoss_Corsair, 3, 110, corsairBuildCondition, corsairStargateCondition);
	pvzEndGame.addProduce(Protoss_Zealot, 1, 100);
	pvzEndGame.addProduce(Protoss_Dragoon, 1, 110);

	// Add
	mBuildOrders[BuildOrderID::PvZEndGame] = pvzEndGame;

	/************************************************************************/
	/* Two Gate                                                             */
	/************************************************************************/

	BuildOrder twoGate(Protoss, BuildOrderID::TwoGate, "2 Gate");

	//Starting Conditions
	twoGate.setStartingCondition(Condition(ConditionTest::isEnemyZerg) || Condition(ConditionTest::isEnemyProtoss) || Condition(ConditionTest::isEnemyUnknown));

	// Follow Ups
	twoGate.addNextBuild(BuildOrderID::CoreIntoStargate, 0, Condition(ConditionTest::isEnemyZerg));
	twoGate.addNextBuild(BuildOrderID::FourGateGoon, 0, Condition(ConditionTest::isEnemyProtoss) || Condition(ConditionTest::isEnemyTerran) || Condition(ConditionTest::isEnemyUnknown));

	//Units to Produce
	twoGate.addProduce(Protoss_Zealot, 1);

	//Army Behaviour
	twoGate.addArmyBehaviour(ArmyBehaviour::Aggresive);

	// Build Order
	ID_1 =	twoGate.addItem(Protoss_Probe, 4);
	ID_1 =	twoGate.addItem(Protoss_Pylon, CB(ID_1, CallBackType::onDispatched)); //Pylon on 8
			twoGate.addOrder(Order::Scout, CB(ID_1, CallBackType::onStarted));

	ID_1 =	twoGate.addItem(Protoss_Probe, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	twoGate.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //Gateway on 10
	ID_1 =	twoGate.addItem(Protoss_Probe, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	twoGate.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //Gateway on 12
	ID_1 =	twoGate.addItem(Protoss_Probe, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	twoGate.addItem(Protoss_Zealot, CB(ID_1, CallBackType::onDispatched));//Zealot on 13
	ID_1 =	twoGate.addItem(Protoss_Pylon, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	twoGate.addItem(Protoss_Probe, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	twoGate.addItem(Protoss_Zealot, CB(ID_1, CallBackType::onDispatched), 2);

	twoGate.addOrder(Order::TrainWorkers, CB(ID_1, CallBackType::onDispatched));
	twoGate.addOrder(Order::SupplyManager, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::TwoGate] = twoGate;

	/************************************************************************/
	/* Core into Stargate                                                   */
	/************************************************************************/

	BuildOrder coreStar(Protoss, BuildOrderID::CoreIntoStargate, "Cybernetics Core into Stargate");

	// Follow Ups
	coreStar.addNextBuild(BuildOrderID::PvZEndGame, 24*60*1);

	// Constants
	coreStar.addOrder(Order::TrainWorkers);
	coreStar.addOrder(Order::SupplyManager);
	coreStar.addOrder(Order::MacroArmyProduction);
	coreStar.addOrder(Order::Scout);

	// Squads
	coreStar.addSquad(SquadType::CorsairSquad);

	//Units to Produce
	coreStar.addProduce(Protoss_Zealot, 1);
	coreStar.addProduce(Protoss_Dragoon, 1);
	coreStar.addProduce(Protoss_Corsair, 2, 100, corsairBuildCondition, corsairStargateCondition);

	//Army Behaviour
	coreStar.addArmyBehaviour(ArmyBehaviour::Aggresive);

	// Build Order
	ID_1 =	coreStar.addItem(Protoss_Assimilator);
			coreStar.addOrder(Order::RefineryManager, CB(ID_1, CallBackType::onDispatched));

	ID_1 =	coreStar.addItem(Protoss_Cybernetics_Core);
			coreStar.addItem(Protoss_Stargate, CB(ID_1, CallBackType::onDispatched));

	coreStar.addOrder(Order::MacroProductionFacilities, CB(ID_1, CallBackType::onDispatched));
	

	// Add
	mBuildOrders[BuildOrderID::CoreIntoStargate] = coreStar;

	/**************************************************************************************************/
	/* Against Terran                                                                                 */
	/**************************************************************************************************/

	/************************************************************************/
	/* 14 Nexus                                                             */
	/************************************************************************/

	BuildOrder fourteenNexus(Protoss, BuildOrderID::FourteenNexus, "14 Nexus");

	//Starting Conditions
	fourteenNexus.setStartingCondition(Condition(ConditionTest::isEnemyTerran) && Condition(ConditionTest::numberOfEnemies, 1) && Condition(ConditionTest::mapSize, 4));

	fourteenNexus.addNextBuild(BuildOrderID::CitadelFirst, 24*60*2);

	//Units to Produce
	fourteenNexus.addProduce(Protoss_Dragoon, 6);
	fourteenNexus.addProduce(Protoss_Zealot, 1);

	//Army Behaviour
	fourteenNexus.addArmyBehaviour(ArmyBehaviour::Defensive);

	// Build Order
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, 4, TaskType::Highest);
	ID_1 =	fourteenNexus.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_1, CallBackType::onDispatched)); //Pylon on 8
			fourteenNexus.addOrder(Order::Scout, CB(ID_1, CallBackType::onStarted));
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 5);
	ID_1 =	fourteenNexus.addItem(Protoss_Nexus, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::Expansion);
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	fourteenNexus.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //Gateway on 14
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	fourteenNexus.addItem(Protoss_Assimilator, CB(ID_1, CallBackType::onDispatched)); //Gas on 15
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	fourteenNexus.addItem(Protoss_Cybernetics_Core, CB(ID_1, CallBackType::onDispatched)); //core on 17
	ID_1 =	fourteenNexus.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //gate on 17
	ID_1 =	fourteenNexus.addItem(Protoss_Zealot, TaskType::Army, CB(ID_1, CallBackType::onDispatched)); //zealot on 17
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	fourteenNexus.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_1, CallBackType::onDispatched)); //pylon on 21
	ID_1 =	fourteenNexus.addItem(Protoss_Dragoon, TaskType::Army, CB(ID_1, CallBackType::onDispatched), 2);//2 Dragoon on 21
	ID_1 =	fourteenNexus.addItem(Singularity_Charge, 1, CB(ID_1, CallBackType::onDispatched));//range on 25
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	fourteenNexus.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_1, CallBackType::onDispatched)); //pylon on 27
	ID_1 =	fourteenNexus.addItem(Protoss_Dragoon, TaskType::Army, CB(ID_1, CallBackType::onDispatched), 2);//2 Dragoon on 27
	ID_1 =	fourteenNexus.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	fourteenNexus.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_1, CallBackType::onDispatched)); //pylon on 33
	ID_1 =	fourteenNexus.addItem(Protoss_Dragoon, TaskType::Army, CB(ID_1, CallBackType::onDispatched), 2);//2 Dragoon on 35

	fourteenNexus.addOrder(Order::TrainWorkers, CB(ID_1, CallBackType::onDispatched));
	fourteenNexus.addOrder(Order::SupplyManager, CB(ID_1, CallBackType::onDispatched));
	fourteenNexus.addOrder(Order::MacroArmyProduction, CB(ID_1, CallBackType::onDispatched));
	fourteenNexus.addOrder(Order::MacroProductionFacilities, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::FourteenNexus] = fourteenNexus;

	/************************************************************************/
	/* One Gate Core                                                        */
	/************************************************************************/

	BuildOrder oneGateCore(Protoss, BuildOrderID::OneGateCore, "1 Gate Core");

	//Starting Conditions
	oneGateCore.setStartingCondition(Condition(ConditionTest::isEnemyTerran) || Condition(ConditionTest::isEnemyProtoss));

	//oneGateCore.addNextBuild(buildRoboVsTerran, Condition(ConditionType::isEnemyTerran));
	oneGateCore.addNextBuild(BuildOrderID::AdditionalGateWays, 0, Condition(ConditionTest::isEnemyTerran));
	oneGateCore.addNextBuild(BuildOrderID::RoboVsProtoss, 0, Condition(ConditionTest::isEnemyProtoss));

	//Army Behaviour
	oneGateCore.addArmyBehaviour(ArmyBehaviour::Aggresive);

	// Build Order
			oneGateCore.addItem(Protoss_Probe, 4, TaskType::Highest);
	ID_1 =	oneGateCore.addItem(Protoss_Pylon, 1); //Pylon on 8
			oneGateCore.addOrder(Order::TrainWorkers, CB(ID_1, CallBackType::onDispatched));
			oneGateCore.addOrder(Order::Scout, CB(ID_1, CallBackType::onStarted));
	ID_1 =	oneGateCore.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //Gateway on 10
	//ID_1 =	oneGateCore.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 =	oneGateCore.addItem(Protoss_Assimilator, CB(ID_1, CallBackType::onDispatched)); //gas on 12
	//ID_1 =	oneGateCore.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 1);//build a zealot based on some conditions? gas steal or is pvp
	//ID_1 =	oneGateCore.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched), 1);
	ID_1 =	oneGateCore.addItem(Protoss_Cybernetics_Core, CB(ID_1, CallBackType::onDispatched)); //core on 14
	//ID_1 =	oneGateCore.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	oneGateCore.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_1, CallBackType::onDispatched)); //pylon on 15
	//ID_1 =	oneGateCore.addItem(Protoss_Probe, TaskType::Worker, CB(ID_1, CallBackType::onDispatched));

	oneGateCore.addOrder(Order::SupplyManager, CB(ID_1, CallBackType::onDispatched));
	oneGateCore.addOrder(Order::RefineryManager, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::OneGateCore] = oneGateCore;

	/************************************************************************/
	/* Robo First                                                           */
	/************************************************************************/

	BuildOrder roboPvT(Protoss, BuildOrderID::RoboVsTerran, "Robotics Facility First");

	// Constants
	roboPvT.addOrder(Order::TrainWorkers);
	roboPvT.addOrder(Order::RefineryManager);
	roboPvT.addOrder(Order::Scout);

	roboPvT.addArmyBehaviour(ArmyBehaviour::Defensive);

	roboPvT.addNextBuild(BuildOrderID::CitadelFirst, 0, Condition(ConditionTest::myPlannedUnitTotalGreaterEqualThan, Protoss_Reaver, 1) && Condition(ConditionTest::myPlannedUnitTotalGreaterEqualThan, Protoss_Shuttle, 1));
	roboPvT.addNextBuild(BuildOrderID::CitadelFirst, 24*60*2);

	// Squads
	roboPvT.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	roboPvT.addProduce(Protoss_Dragoon, 1);

	// Build Order
	ID_3 =	roboPvT.addItem(Protoss_Dragoon, 4, TaskType::Army);
			roboPvT.addOrder(Order::MacroArmyProduction, CB(ID_3, CallBackType::onDispatched));
	ID_1 =	roboPvT.addItem(Protoss_Pylon, 1, TaskType::Supply);
	ID_1 =	roboPvT.addItem(Protoss_Robotics_Facility, CB(ID_1, CallBackType::onStarted));
			roboPvT.addItem(Singularity_Charge, 1, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	roboPvT.addItem(Protoss_Observatory, CB(ID_1, CallBackType::onCompleted));
	ID_2 =	roboPvT.addItem(Protoss_Observer, CB(ID_1, CallBackType::onStarted));
	ID_1 =	roboPvT.addItem(Protoss_Nexus, TaskType::Expansion, CB(ID_1, CallBackType::onDispatched), 1, BuildingLocation::Expansion);
			roboPvT.addOrder(Order::MacroProductionFacilities, CB(ID_1, CallBackType::onDispatched));
			roboPvT.addOrder(Order::SupplyManager, CB(ID_1, CallBackType::onDispatched));
	ID_2 =	roboPvT.addItem(Protoss_Shuttle, CB(ID_2, CallBackType::onDispatched));
			roboPvT.addItem(Protoss_Reaver, CB(ID_2, CallBackType::onDispatched));
	ID_1 =	roboPvT.addItem(Protoss_Robotics_Support_Bay, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::RoboVsTerran] = roboPvT;

	/************************************************************************/
	/* Branch 1: Additional Gateways                                        */
	/************************************************************************/

	BuildOrder gateways(Protoss, BuildOrderID::AdditionalGateWays, "Additional Gateways");

	// Constants
	gateways.addOrder(Order::TrainWorkers);
	gateways.addOrder(Order::SupplyManager);
	gateways.addOrder(Order::Scout);

	gateways.addArmyBehaviour(ArmyBehaviour::Aggresive);

	// Follow Ups
	gateways.addNextBuild(BuildOrderID::Nexus, 24*60*4);
	gateways.addNextBuild(BuildOrderID::Nexus, 0, Condition(ConditionTest::enemyHasResearched, BWAPI::TechTypes::Tank_Siege_Mode));

	//Units to Produce
	gateways.addProduce(Protoss_Dragoon, 1);

	// Build Order
	ID_1 =	gateways.addItem(Protoss_Dragoon, 1, TaskType::Army);
			gateways.addOrder(Order::MacroArmyProduction, CB(ID_1, CallBackType::onDispatched, CB(ID_1, CallBackType::onDispatched)));
			
	ID_1 =	gateways.addItem(Protoss_Gateway, 1, TaskType::MacroExtraProduction);
			gateways.addItem(Singularity_Charge, 1, TaskType::MacroUrgent, CB(ID_1, CallBackType::onDispatched));
			gateways.addOrder(Order::MacroProductionFacilities, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::AdditionalGateWays] = gateways;

	/************************************************************************/
	/* Branch 2: Nexus                                                      */
	/************************************************************************/

	BuildOrder nexus(Protoss, BuildOrderID::Nexus, "Nexus");

	// Constants
	nexus.addOrder(Order::TrainWorkers);
	nexus.addOrder(Order::SupplyManager);
	nexus.addOrder(Order::MacroArmyProduction);
	nexus.addOrder(Order::MacroProductionFacilities);
	nexus.addOrder(Order::Scout);

	// Follow Ups
	nexus.addNextBuild(BuildOrderID::PvPMidGame, 0, Condition(ConditionTest::isEnemyProtoss));
	nexus.addNextBuild(BuildOrderID::PvZEndGame, 0, Condition(ConditionTest::isEnemyZerg));
	nexus.addNextBuild(BuildOrderID::CitadelFirst, 24*50*2);

	//Units to Produce
	nexus.addProduce(Protoss_Dragoon, 6, 110);
	nexus.addProduce(Protoss_Zealot, 1, 100);
	nexus.addProduce(Protoss_Dark_Templar, 4, 100, Condition(ConditionTest::myUnitCountGreaterEqualThan, Protoss_Templar_Archives, 1));

	// Build Order
	ID_1 =	nexus.addItem(Protoss_Nexus, 1, TaskType::Expansion, BuildingLocation::Expansion);

	// Add
	mBuildOrders[BuildOrderID::Nexus] = nexus;

	/************************************************************************/
	/* Citadel First                                                        */
	/************************************************************************/

	BuildOrder citadel(Protoss, BuildOrderID::CitadelFirst, "Citadel First");

	// Constants
	citadel.addOrder(Order::TrainWorkers);
	citadel.addOrder(Order::SupplyManager);
	citadel.addOrder(Order::RefineryManager);
	citadel.addOrder(Order::MacroArmyProduction);
	citadel.addOrder(Order::MacroProductionFacilities);
	citadel.addOrder(Order::ExpansionManager);
	citadel.addOrder(Order::CanRemoveSquads);
	citadel.addOrder(Order::Scout);

	// Follow Ups
	citadel.addNextBuild(BuildOrderID::PvTMidGame, 24*60);
	citadel.addNextBuild(BuildOrderID::PvTCarrierSwitch, 0, Condition(ConditionTest::enemyUnitCountLessThan, BWAPI::UnitTypes::Terran_Goliath, 2) && Condition(ConditionTest::myUnitCountGreaterEqualThan, BWAPI::UnitTypes::Protoss_Nexus, 2) && Condition(ConditionTest::randomChance, 0.2));

	// Squads
	citadel.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	citadel.addProduce(Protoss_Dragoon, 2);
	citadel.addProduce(Protoss_Zealot, 6);

	// Build Order
	ID_1 =	citadel.addItem(Protoss_Citadel_of_Adun);
			citadel.addItem(Leg_Enhancements, 1, TaskType::MacroUrgent, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::CitadelFirst] = citadel;

	/************************************************************************/
	/* Mid Game Temp                                                        */
	/************************************************************************/

	BuildOrder midGame(Protoss, BuildOrderID::PvTMidGame, "Mid Game");

	// Constants
	midGame.addOrder(Order::TrainWorkers);
	midGame.addOrder(Order::SupplyManager);
	midGame.addOrder(Order::RefineryManager);
	midGame.addOrder(Order::MacroArmyProduction);
	midGame.addOrder(Order::MacroProductionFacilities);
	midGame.addOrder(Order::ExpansionManager);
	midGame.addOrder(Order::CanRemoveSquads);
	midGame.addOrder(Order::Scout);

	// Follow Ups
	midGame.addNextBuild(BuildOrderID::PvTEndGame, 24*60);

	// Squads
	midGame.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	midGame.addProduce(Protoss_Dragoon, 10);
	midGame.addProduce(Protoss_Zealot, 10);
	midGame.addProduce(Protoss_High_Templar, 1, 100, Condition(ConditionTest::isResearching, Psionic_Storm));

	ID_1 =	midGame.addItem(Protoss_Templar_Archives);
	ID_1 =	midGame.addItem(Psionic_Storm, TaskType::MacroUrgent, CB(ID_1, CallBackType::onCompleted));

	midGame.addOrder(Order::MacroCanTech, CB(ID_1, CallBackType::onCompleted));

	// Add
	mBuildOrders[BuildOrderID::PvTMidGame] = midGame;

	/************************************************************************/
	/* PvT End Game                                                             */
	/************************************************************************/

	BuildOrder pvtEndGame(Protoss, BuildOrderID::PvTEndGame, "PvT End Game");

	// Constants
	pvtEndGame.addOrder(Order::TrainWorkers);
	pvtEndGame.addOrder(Order::Scout);
	pvtEndGame.addOrder(Order::SupplyManager);
	pvtEndGame.addOrder(Order::RefineryManager);
	pvtEndGame.addOrder(Order::MacroArmyProduction);
	pvtEndGame.addOrder(Order::CanRemoveSquads);
	pvtEndGame.addOrder(Order::ExpansionManager);
	pvtEndGame.addOrder(Order::MacroProductionFacilities);
	pvtEndGame.addOrder(Order::MacroCanTech);

	// Squads
	pvtEndGame.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	pvtEndGame.addProduce(Protoss_Dragoon, 14);
	pvtEndGame.addProduce(Protoss_Zealot, 14);
	pvtEndGame.addProduce(Protoss_High_Templar, 3, 100, Condition(ConditionTest::isResearching, Psionic_Storm));
	pvtEndGame.addProduce(Protoss_Arbiter, 1);

	// Add
	mBuildOrders[BuildOrderID::PvTEndGame] = pvtEndGame;

	/************************************************************************/
	/* Carrier Switch                                                       */
	/************************************************************************/

	BuildOrder carriers(Protoss, BuildOrderID::PvTCarrierSwitch, "Carriers");

	// Constants
	carriers.addOrder(Order::TrainWorkers);
	carriers.addOrder(Order::SupplyManager);
	carriers.addOrder(Order::MacroArmyProduction);
	carriers.addOrder(Order::RefineryManager);
	carriers.addOrder(Order::MacroProductionFacilities);
	carriers.addOrder(Order::MacroCanTech);
	carriers.addOrder(Order::ExpansionManager);
	carriers.addOrder(Order::CanRemoveSquads);
	carriers.addOrder(Order::Scout);

	// Squads
	carriers.addSquad(SquadType::ReaverDropSquad);

	ID_1 = carriers.addItem(Protoss_Stargate, 1);
	ID_1 = carriers.addItem(Protoss_Fleet_Beacon, CB(ID_1, CallBackType::onDispatched));
	       carriers.addItem(Protoss_Carrier, CB(ID_1, CallBackType::onDispatched), 1);
	ID_1 = carriers.addItem(Protoss_Stargate, CB(ID_1, CallBackType::onDispatched), 2);
	ID_1 = carriers.addItem(Protoss_Carrier, CB(ID_1, CallBackType::onDispatched), 7);

	//Units to Produce
	carriers.addProduce(Protoss_Dragoon, 1);
	carriers.addProduce(Protoss_Zealot, 8);
	carriers.addProduce(Protoss_Carrier, 12);

	// Add
	mBuildOrders[BuildOrderID::PvTCarrierSwitch] = carriers;

	/************************************************************************/
	/* Four Gate Goon                                                       */
	/************************************************************************/

	BuildOrder gatewayGoon(Protoss, BuildOrderID::FourGateGoon, "Four Gate Goon");

	// Constants
	gatewayGoon.addOrder(Order::TrainWorkers);
	gatewayGoon.addOrder(Order::SupplyManager);
	gatewayGoon.addOrder(Order::Scout);
	gatewayGoon.addOrder(Order::MacroArmyProduction);
	gatewayGoon.addOrder(Order::RefineryManager);

	gatewayGoon.addArmyBehaviour(ArmyBehaviour::Default);

	// Follow Ups
	gatewayGoon.addNextBuild(BuildOrderID::Nexus, 24*60*3);

	//Units to Produce
	gatewayGoon.addProduce(Protoss_Dragoon, 8, 110);
	gatewayGoon.addProduce(Protoss_Zealot, 1, 100);

	// Build Order
	ID_1 =	gatewayGoon.addItem(Protoss_Cybernetics_Core);
			gatewayGoon.addItem(Singularity_Charge, 1, TaskType::MacroUrgent, CB(ID_1, CallBackType::onDispatched));

			gatewayGoon.addOrder(Order::MacroProductionFacilities, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::FourGateGoon] = gatewayGoon;

	/************************************************************************/
	/* Robo First                                                           */
	/************************************************************************/

	BuildOrder robo(Protoss, BuildOrderID::RoboVsProtoss, "Robotics Facility");

	// Constants
	robo.addOrder(Order::TrainWorkers);
	robo.addOrder(Order::SupplyManager);
	robo.addOrder(Order::RefineryManager);
	robo.addOrder(Order::MacroArmyProduction);
	robo.addOrder(Order::Scout);

	robo.addArmyBehaviour(ArmyBehaviour::Defensive);

	robo.addNextBuild(BuildOrderID::PvPMidGame, 0, Condition(ConditionTest::myPlannedUnitTotalGreaterEqualThan, Protoss_Reaver, 1) /* && Condition(ConditionType::myUnitCount, Protoss_Shuttle, 1) */);

	// Follow Ups
	robo.addNextBuild(BuildOrderID::PvPMidGame, 24*60);

	// Squads
	robo.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	robo.addProduce(Protoss_Zealot, 1, 100);
	robo.addProduce(Protoss_Dragoon, 10, 110);
	robo.addProduce(Protoss_Reaver, 1, 120, Condition(ConditionTest::myPlannedUnitTotalLessThan, Protoss_Reaver, 3), Condition(ConditionTest::myPlannedUnitTotalLessThan, Protoss_Robotics_Facility, 1));

	// Build Order
			robo.addItem(Protoss_Dragoon, 4, TaskType::Army);
	ID_1 =	robo.addItem(Protoss_Gateway, 1, TaskType::MacroExtraProduction);
	ID_1 =	robo.addItem(Protoss_Robotics_Facility, CB(ID_1, CallBackType::onDispatched));
			robo.addItem(Singularity_Charge, 1, CB(ID_1, CallBackType::onDispatched));
	ID_1 =	robo.addItem(Protoss_Observatory, CB(ID_1, CallBackType::onStarted));
	ID_2 =	robo.addItem(Protoss_Observer, TaskType::Highest, CB(ID_1, CallBackType::onStarted));
	//ID_2 =	robo.addItem(Protoss_Shuttle, TaskType::Highest, CB(ID_2, CallBackType::onStarted));
	ID_2 =	robo.addItem(Protoss_Robotics_Support_Bay, TaskType::Highest, CB(ID_2, CallBackType::onDispatched));
			robo.addItem(Protoss_Reaver, TaskType::Highest, CB(ID_2, CallBackType::onDispatched), 2);

	ID_1 =	robo.addItem(Protoss_Gateway, TaskType::MacroExtraProduction, CB(ID_1, CallBackType::onDispatched));
			robo.addOrder(Order::MacroProductionFacilities, CB(ID_1, CallBackType::onDispatched));

	// Add
	mBuildOrders[BuildOrderID::RoboVsProtoss] = robo;

	/************************************************************************/
	/* PvP Mid Game                                                         */
	/************************************************************************/

	BuildOrder PvPMid(Protoss, BuildOrderID::PvPMidGame, "Mid Game");

	// Constants
	PvPMid.addOrder(Order::TrainWorkers);
	PvPMid.addOrder(Order::SupplyManager);
	PvPMid.addOrder(Order::ExpansionManager);
	PvPMid.addOrder(Order::RefineryManager);
	PvPMid.addOrder(Order::MacroArmyProduction);
	PvPMid.addOrder(Order::MacroProductionFacilities);
	PvPMid.addOrder(Order::MacroCanTech);
	PvPMid.addOrder(Order::CanRemoveSquads);
	PvPMid.addOrder(Order::Scout);

	PvPMid.addArmyBehaviour(ArmyBehaviour::Default);

	PvPMid.addNextBuild(BuildOrderID::PvPEndGame, 24*60*2);

	// Squads
	PvPMid.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	PvPMid.addProduce(Protoss_Zealot, 1, 100);
	PvPMid.addProduce(Protoss_Dragoon, 10, 110);
	PvPMid.addProduce(Protoss_High_Templar, 1, 110, Condition(ConditionTest::isResearching, Psionic_Storm));
	PvPMid.addProduce(Protoss_Reaver, 1, 110, Condition(ConditionTest::myPlannedUnitTotalLessThan, Protoss_Reaver, 3), Condition(ConditionTest::myPlannedUnitTotalLessThan, Protoss_Robotics_Facility, 1));

	// Add
	mBuildOrders[BuildOrderID::PvPMidGame] = PvPMid;

	/************************************************************************/
	/* PvP End Game                                                         */
	/************************************************************************/

	BuildOrder pvpEndGame(Protoss, BuildOrderID::PvPEndGame, "PvP End Game");

	// Constants
	pvpEndGame.addOrder(Order::TrainWorkers);
	pvpEndGame.addOrder(Order::Scout);
	pvpEndGame.addOrder(Order::SupplyManager);
	pvpEndGame.addOrder(Order::RefineryManager);
	pvpEndGame.addOrder(Order::MacroArmyProduction);
	pvpEndGame.addOrder(Order::CanRemoveSquads);
	pvpEndGame.addOrder(Order::ExpansionManager);
	pvpEndGame.addOrder(Order::MacroProductionFacilities);
	pvpEndGame.addOrder(Order::MacroCanTech);

	// Squads
	pvpEndGame.addSquad(SquadType::ReaverDropSquad);

	//Units to Produce
	pvpEndGame.addProduce(Protoss_Zealot, 1);
	pvpEndGame.addProduce(Protoss_Dragoon, 14);
	pvpEndGame.addProduce(Protoss_High_Templar, 2, 100, Condition(ConditionTest::isResearching, Psionic_Storm));
	pvpEndGame.addProduce(Protoss_Arbiter, 1);

	// Add
	mBuildOrders[BuildOrderID::PvPEndGame] = pvpEndGame;

	/************************************************************************/
	/* DT Rush                                                              */
	/************************************************************************/

	BuildOrder dtRush(Protoss, BuildOrderID::DTRush, "DT Rush");

	//Starting Conditions
	dtRush.setStartingCondition(Condition(ConditionTest::isEnemyZerg) || Condition(ConditionTest::isEnemyProtoss));

	dtRush.addNextBuild(BuildOrderID::Nexus, 24*60);

	//Army Behaviour
	dtRush.addArmyBehaviour(ArmyBehaviour::Defensive);

	// Build Order
			dtRush.addItem(Protoss_Probe, 4, TaskType::Highest);
	ID_1 =	dtRush.addItem(Protoss_Pylon, 1); //Pylon on 8
			dtRush.addItem(Protoss_Probe, CB(ID_1, CallBackType::onDispatched), 2); //probage

	ID_1 =	dtRush.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //Gateway on 10
			dtRush.addOrder(Order::TrainWorkers, CB(ID_1, CallBackType::onDispatched));
			dtRush.addOrder(Order::Scout, CB(ID_1, CallBackType::onStarted));

	ID_2 =	dtRush.addItem(Protoss_Assimilator, CB(ID_1, CallBackType::onDispatched)); //gas on 12
			dtRush.addItem(Protoss_Zealot, CB(ID_1, CallBackType::onDispatched)); //zealot

	ID_1 =	dtRush.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_2, CallBackType::onDispatched)); //pylon on 16
	ID_2 =	dtRush.addItem(Protoss_Cybernetics_Core, CB(ID_1, CallBackType::onDispatched)); //core on 18
			dtRush.addItem(Protoss_Zealot, CB(ID_1, CallBackType::onDispatched)); //zealot
	ID_1 =	dtRush.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_2, CallBackType::onDispatched)); //pylon on 22
			dtRush.addItem(Protoss_Dragoon, CB(ID_1, CallBackType::onDispatched)); //dragoon
	ID_1 =	dtRush.addItem(Protoss_Citadel_of_Adun, CB(ID_1, CallBackType::onDispatched)); //citadel on 26
			dtRush.addItem(Protoss_Dragoon, CB(ID_1, CallBackType::onDispatched)); //dragoon
	ID_1 =	dtRush.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched)); //gateway
	ID_1 =	dtRush.addItem(Protoss_Pylon, TaskType::Supply, CB(ID_1, CallBackType::onDispatched)); //pylon
	ID_1 =	dtRush.addItem(Protoss_Templar_Archives, CB(ID_1, CallBackType::onDispatched)); //archives
			dtRush.addItem(Protoss_Zealot, CB(ID_1, CallBackType::onDispatched)); //zealot
			dtRush.addItem(Protoss_Zealot, CB(ID_1, CallBackType::onDispatched)); //zealot
	ID_1 =	dtRush.addItem(Protoss_Dark_Templar, CB(ID_1, CallBackType::onDispatched), 2); //templar
			dtRush.addArmyBehaviour(ArmyBehaviour::Aggresive, CB(ID_1, CallBackType::onCompleted));
			dtRush.addItem(Protoss_Gateway, CB(ID_1, CallBackType::onDispatched), 2); //gateway
			dtRush.addItem(Protoss_Dark_Templar, CB(ID_1, CallBackType::onDispatched), 2); //templar

			dtRush.addOrder(Order::SupplyManager, CB(ID_1, CallBackType::onDispatched));
			dtRush.addOrder(Order::RefineryManager, CB(ID_1, CallBackType::onDispatched));
			dtRush.addOrder(Order::MacroArmyProduction, CB(ID_1, CallBackType::onDispatched));

	dtRush.addProduce(Protoss_Zealot, 2);
	dtRush.addProduce(Protoss_Dragoon, 1);
	dtRush.addProduce(Protoss_Dark_Templar, 10);

	// Add
	mBuildOrders[BuildOrderID::DTRush] = dtRush;
}