#include "MatchData.h"
#include <ctime>
#include <BWAPI.h>
#include <fstream>
#include <sstream>

#include "Configuration.h"

using namespace BWAPI;
using namespace std;

MatchData* MatchData::instance = NULL;

MatchData::MatchData() {
}


MatchData::~MatchData() {
}

void MatchData::registerMatchBegin() {
	startTime = currentDateTime();
}

void MatchData::registerMatchFinish(int result) {
	gameResult = result;

	//attempts to retrieve enemy score information (not working, currently BWAPI doesn't allow retrieval of enemy info even at match end)
	Broodwar->enableFlag(Flag::CompleteMapInformation);

	//registers scores of both players
	Player* me = Broodwar->self();
	Player* enemy = Broodwar->enemy();

	myUnitScore = me->getUnitScore();
	myKillScore = me->getKillScore();
	myBuildingScore = me->getBuildingScore();
	myRazingScore = me->getRazingScore();
	myGatheredMinerals = me->gatheredMinerals();
	myGatheredGas = me->gatheredGas();
	myTotal = myUnitScore + myKillScore + myBuildingScore + myRazingScore + myGatheredMinerals + myGatheredGas;

	enemyUnitScore = enemy->getUnitScore();
	enemyKillScore = enemy->getKillScore();
	enemyBuildingScore = enemy->getBuildingScore();
	enemyRazingScore = enemy->getRazingScore();
	enemyGatheredMinerals = enemy->gatheredMinerals();
	enemyGatheredGas = enemy->gatheredGas();
	enemyTotal = enemyUnitScore + enemyKillScore + enemyBuildingScore + enemyRazingScore + enemyGatheredMinerals + enemyGatheredGas;

	//registers game duration and finish timestamp
	frameCount = Broodwar->getFrameCount();
	duration = Broodwar->elapsedTime();
	endTime = currentDateTime();
}


void MatchData::writeDetailedResult() {
	string resFile = "output.res.xml";
	string resPath = "bwapi-data/AI/" + resFile;
	ofstream statsFile(resPath.c_str(), ios_base::out);		//c_str is needed for VC++2008

	statsFile << "<results>" << endl <<
		"\t<result value='" << resultToString(gameResult) << "'/>" << endl <<
		"\t<start value='" << startTime << "'/>" << endl <<
		"\t<end value='" << endTime << "'/>" << endl <<
		"\t<frames value='" << frameCount << "'/>" << endl <<
		"\t<gameDuration value='" << duration << "'/>" << endl <<
		"\t<map value='" << Broodwar->mapFileName() << "'/>" << endl <<
		endl << "\t<player>" << endl <<
		"\t\t<race value='" << Broodwar->self()->getRace().getName() << "'/>" << endl <<
		"\t\t<unitScore value='" << myUnitScore << "'/>" << endl <<
		"\t\t<killScore value='" << myKillScore << "'/>" << endl <<
		"\t\t<buildingScore value='" << myBuildingScore << "'/>" << endl <<
		"\t\t<razingScore value='" << myRazingScore << "'/>" << endl <<
		"\t\t<mineralScore value='" << myGatheredMinerals << "'/>" << endl <<
		"\t\t<gasScore value='" << myGatheredGas << "'/>" << endl <<
		"\t\t<totalScore value='" << myTotal << "'/>" << endl <<
		"\t</player>" << endl <<
		endl << "\t<enemy>" << endl <<
		"\t\t<race value='" << Broodwar->enemy()->getRace().getName() << "'/>" << endl <<
		"\t\t<unitScore value='" << enemyUnitScore << "'/>" << endl <<
		"\t\t<unitScore value='" << enemyUnitScore << "'/>" << endl <<
		"\t\t<killScore value='" << enemyKillScore << "'/>" << endl <<
		"\t\t<buildingScore value='" << enemyBuildingScore << "'/>" << endl <<
		"\t\t<razingScore value='" << enemyRazingScore << "'/>" << endl <<
		"\t\t<mineralScore value='" << enemyGatheredMinerals << "'/>" << endl <<
		"\t\t<gasScore value='" << enemyGatheredGas << "'/>" << endl <<
		"\t\t<totalScore value='" << enemyTotal << "'/>" << endl <<
		"\t</enemy>" << endl <<
		endl << "\t<scoreRatio value='" << myTotal / float(enemyTotal) << "'/>" << endl <<
		"</results>" << endl;
	statsFile.close();
}

MatchData* MatchData::getInstance() {
	if (instance == NULL) {
		instance = new MatchData();
	}
	return instance;
}

string MatchData::resultToString(int result) {
	switch (result) {
	case WIN:
		return "win";
		
	case LOSS:
		return "loss";

	case DRAW:
		return "draw";

	default:
		Broodwar->printf("Invalid game result %d!", result);
		return "invalid";
	}
}

string MatchData::getSummaryFilename() {
	stringstream ss;
	ss << Configuration::OUTPUT_DIR;	//bwapi-data/AI or /write
	//ss << "bwapi-data\\write\\"; //Tournament persistent storage version
	ss << "megabot_matchsummary.csv";

	return ss.str();
}

void MatchData::writeSummary() {

	stringstream ss;
	ss << Broodwar->self()->getRace().getName() << ";";
	//ss << StrategySelector::getInstance()->getStrategyID() << ";";
	ss << Broodwar->enemy()->getRace().getName() << ";";
	ss << Broodwar->mapFileName() << ";";
	if (gameResult == WIN) ss << "Won";
	if (gameResult == LOSS) ss << "Lost";
	if (gameResult == DRAW) ss << "Draw";
	ss << ";";
	ss << Broodwar->self()->getUnitScore() << ";";
	ss << Broodwar->self()->getBuildingScore() << ";";
	ss << Broodwar->self()->getKillScore() << ";";
	ss << Broodwar->enemy()->getUnitScore() << ";";
	ss << Broodwar->enemy()->getBuildingScore() << ";";
	ss << Broodwar->enemy()->getKillScore();
	ss << "\n";

	//Save the file
	string filename = getSummaryFilename();

	ofstream outFile;
	outFile.open(filename.c_str(), ios::out | ios::app);
	if (!outFile) {
		Broodwar->printf("Error writing to stats file!\n");
	}
	else {
		outFile << ss.str();
		outFile.close();
	}
}

const string MatchData::currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);//   localtime_s(&tm, &now);

	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}