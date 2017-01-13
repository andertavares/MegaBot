#pragma once

#include "Interface.h"

#include "Singleton.h"

class GameMemoryClass
{
public:
	GameMemoryClass(){}

	void onBegin();

	void onEnd();

	const std::vector<std::string> &getData(const std::string &dataName);
	void setData(const std::string &dataName, const std::vector<std::string> &data);

private:
	std::string mFileName;
	std::map<std::string, std::vector<std::string>> mData;
};

typedef Singleton<GameMemoryClass> GameMemory;