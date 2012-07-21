#include "GameMemory.h"
#include "PlayerTracker.h"

#include <fstream>
#include <sstream>

const char * pathPrefixRead = "bwapi-data\\Skynet\\Memory\\";
const char * pathPrefixWrite = "bwapi-data\\Skynet\\Memory\\";

void GameMemoryClass::onBegin()
{
	if(PlayerTracker::Instance().getEnemies().size() == 1 && PlayerTracker::Instance().getAllies().size() == 0)
	{
		Player enemy = *PlayerTracker::Instance().getEnemies().begin();
		mFileName = enemy->getName() + " " + enemy->getRace().getName() + ".txt";
		BWAPI::Broodwar->printf(mFileName.c_str());
		std::ifstream file((pathPrefixRead + mFileName).c_str());

		if(file)
		{
			std::string line;
			while(std::getline(file, line))
			{
				std::stringstream liness(line);

				std::istream_iterator<std::string> it(liness);
				std::istream_iterator<std::string> end;
				std::vector<std::string> results(it, end);

				if(results.size() >= 2)
				{
					mData[results[0]] = std::vector<std::string>(++results.begin(), results.end());
					BWAPI::Broodwar->printf(line.c_str());
				}
			}
		}
	}
}

void GameMemoryClass::onEnd()
{
	if(!mFileName.empty())
	{
		std::ofstream file((pathPrefixWrite + mFileName).c_str());

		if(file)
		{
			for(std::map<std::string, std::vector<std::string>>::iterator it = mData.begin(); it != mData.end(); ++it)
			{
				if(!it->second.empty())
				{
					file << it->first;
					for(std::vector<std::string>::iterator dataIt = it->second.begin(); dataIt != it->second.end(); ++dataIt)
					{
						file << " " << *dataIt;
					}
					file << "\n";
				}
			}
		}
	}
}

const std::vector<std::string> & GameMemoryClass::getData(const std::string &dataName)
{
	return mData[dataName];
}

void GameMemoryClass::setData(const std::string &dataName, const std::vector<std::string> &data)
{
	mData[dataName] = data;
}