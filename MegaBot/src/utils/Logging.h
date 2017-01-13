#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>
#include <BWAPI.h>
#include <boost\format.hpp>
#include "../data/Configuration.h"
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

class Logging {
	
	/** Level of messages to log (NONE, INFO, DEBUG) */
	int logLevel;

	//uses WINAPI functions to retrieve an error message
	void logWindowsError(LPTSTR lpszFunction);

	//sets the log filename to the specified number
	//void setLogFileNumber(string prefix, int number);
	
	
public:
    ~Logging() { }

    static Logging* getInstance();

    void log(const char * msg, ...);

	static const int LOSS = 0;

	/** Logging level */
	static const int NONE = 0;

	/** Logs information messages */
	static const int INFO = 1;

	/** Registers debug messages */
	//static const int DEBUG = 2;

protected:
    FILE* file_ptr;
    string filename;

    Logging();
};

#endif