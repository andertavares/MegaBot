#include "GameMemory.h"
#include "PlayerTracker.h"
#include "Logger.h"

#include <fstream>
#include <sstream>

#if defined(COMPETITION_MODE)
const char * pathPrefixRead = "bwapi-data\\Read\\";
const char * pathPrefixWrite = "bwapi-data\\Write\\";
#else
const char * pathPrefixRead = "bwapi-data\\Skynet\\Memory";
const char * pathPrefixWrite = "bwapi-data\\Skynet\\Memory";
#endif

void GameMemoryClass::onBegin()
{
	if(PlayerTracker::Instance().getEnemies().size() == 1 && PlayerTracker::Instance().getAllies().size() == 0)
	{
		Player enemy = *PlayerTracker::Instance().getEnemies().begin();
		if(enemy->getType() == BWAPI::PlayerTypes::Computer)
		{
			mFileName = "AI " + enemy->getRace().getName() + ".txt";
		}
		else
		{
			mFileName = enemy->getName() + " " + enemy->getRace().getName() + ".txt";
		}
		
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
				}
			}
		}
	}
}

void GameMemoryClass::onEnd()
{
	if(!mFileName.empty())
	{
		std::string fileName = pathPrefixWrite + mFileName;
		std::ofstream file(fileName.c_str());

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
		else
		{
			LOGMESSAGEERROR(String_Builder() << "Failed to write match data to " << fileName);
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