#include "GameMemory.h"
#include "PlayerTracker.h"
#include "Logger.h"

#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

const std::string filePrefix = "skynet_";
const std::string filePostfix = ".txt";
#if defined(COMPETITION_MODE)
const std::string pathRead = "bwapi-data/read/";
const std::string pathWrite = "bwapi-data/write/";
#else
const std::string pathRead = "bwapi-data/Skynet/Memory/";
const std::string pathWrite = "bwapi-data/Skynet/Memory/";
#endif

std::string fixup_name(std::string str)
{
	for(size_t i = 0; i < str.length(); ++i)
	{
		str[i] = ::tolower( str[i] );
		if( !::isalnum( str[i] ) )
			str[i] = '_';
	}

	return str;
}

void GameMemoryClass::onBegin()
{
	if(PlayerTracker::Instance().getEnemies().size() == 1 && PlayerTracker::Instance().getAllies().size() == 0)
	{
		Player enemy = *PlayerTracker::Instance().getEnemies().begin();
		if(enemy->getType() == BWAPI::PlayerTypes::Computer)
		{
			mFileName = filePrefix + "ai_" + fixup_name( enemy->getRace().getName() ) + filePostfix;
		}
		else
		{
			mFileName = filePrefix + fixup_name( enemy->getName() + "_" + enemy->getRace().getName() ) + filePostfix;
		}
		
		std::ifstream file((pathRead + mFileName).c_str());

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
		boost::filesystem::create_directories(pathWrite);

		std::string fileName = pathWrite + mFileName;
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