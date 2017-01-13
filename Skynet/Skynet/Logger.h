#pragma once

#include "Singleton.h"

#include "Interface.h"
#include <string>
#include <sstream>
#include <fstream>

#if !defined(NDEBUG)
#define LOG_TO_FILE
#endif

class String_Builder
{
public:
    template<typename T>
    String_Builder &operator <<(const T &value)
    {
        builder << value;

        return (*this);
    }

    operator std::string()
    {
        return (builder.str());
    }

	std::string getString()
	{
		return (builder.str());
	}

private:
    std::ostringstream builder; 
};

class LoggerClass
{
public:
	LoggerClass()
	{
#if defined(LOG_TO_FILE)
		std::ofstream output("bwapi-data\\Skynet\\SkynetLog.txt", std::ios::trunc);
		output.close();
#endif
	}

	inline void log(const std::string &message, int level = 0)
	{
		if(level > 1)
			BWAPI::Broodwar->sendText(message.c_str());
		else if(level > 0)
			BWAPI::Broodwar->printf(message.c_str());

#if defined(LOG_TO_FILE)
		std::ofstream output("bwapi-data\\Skynet\\SkynetLog.txt", std::ios::ate | std::ios::app);
		output << BWAPI::Broodwar->getFrameCount() << " : " << message << std::endl;
		output.close();
#endif
	}

private:
};
typedef Singleton<LoggerClass> Logger;

#if !defined(COMPETITION_MODE)
	#define LOGMESSAGEWARNING(message) Logger::Instance().log(message, 1)
#else
	#define LOGMESSAGEWARNING(ignore) ((void) 0)
#endif

#if defined(LOG_TO_FILE)
	#define LOGMESSAGE(message) Logger::Instance().log(message)
#else
	#define LOGMESSAGE(ignore) ((void) 0)
#endif

#define LOGMESSAGEERROR(message) Logger::Instance().log(message, 2)