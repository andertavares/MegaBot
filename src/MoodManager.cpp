#include <MoodManager.h>

MoodManager::MoodManager()
{
	randomMood							= new Random(5);
	unitPoints							= new Points();

	moodData.mood						= (MoodData::Mood)randomMood->nextInt();

	unitPoints->zealot			= 100;
	unitPoints->dragoon			= 0;
	unitPoints->corsair			= 0;
	unitPoints->darkTemplar = 0;
}

MoodManager::~MoodManager()
{
	delete unitPoints;
	delete randomMood;
}

MoodManager::MoodData::Mood MoodManager::getMood()
{
	return moodData.mood;
}

MoodManager::Points* MoodManager::getPoints()
{
	return unitPoints;
}

void MoodManager::setMood(MoodManager::MoodData::Mood mood)
{
	moodData.mood = mood;
}

void MoodManager::setRandomMood()
{
	moodData.mood = (MoodData::Mood)randomMood->nextInt();
}

void MoodManager::setAnotherRandomMood()
{
	moodData.mood = (MoodData::Mood)randomMood->nextAnotherInt();
}

void MoodManager::setInformationManager(InformationManager *informationManager)
{
	this->informationManager = informationManager;
}

void MoodManager::update()
{
	if ((lastFrameCheck == 0) || (BWAPI::Broodwar->getFrameCount() > lastFrameCheck + 72))
	{
		lastFrameCheck = BWAPI::Broodwar->getFrameCount();
		if ((BWAPI::Broodwar->getFrameCount() > 5200) && (unitPoints->dragoon == 0))
			unitPoints->dragoon = 50;
	
		if ((BWAPI::Broodwar->getFrameCount() > 8500) && (unitPoints->corsair == 0) && (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Zerg))
			unitPoints->corsair = 50;

		if ((BWAPI::Broodwar->getFrameCount() > 10000) && (unitPoints->darkTemplar == 0))
		{
			if (BWAPI::Broodwar->enemy()->getRace() == BWAPI::Races::Zerg)
				unitPoints->darkTemplar = 20;
			else	
				unitPoints->darkTemplar = 30;
		}

		if ((informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Wraith)) ||
			(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Corsair)) ||
			(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Scout)) ||
			(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Terran_Starport)) ||
			(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Protoss_Stargate)))
		{
			unitPoints->corsair = 30;
		}

		if ((informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Mutalisk)) ||
			(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Spire)) ||
			(informationManager->enemyHasBuilt(BWAPI::UnitTypes::Zerg_Greater_Spire)))
		{
			unitPoints->corsair = 70;
		}
	}	
}