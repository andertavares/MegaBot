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
	//uses WINAPI functions to retrieve an error message
	void logWindowsError(LPTSTR lpszFunction);

	//sets the log filename to the specified number
	//void setLogFileNumber(string prefix, int number);

public:
    ~Logging() { }

    static Logging* getInstance();

    void log(const char * msg, ...);

protected:
    FILE* file_ptr;
    string filename;

    Logging();
};

#endif